#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "context.h"
#include "queue_families.h"
#include "swap_chain.h"

bool check_device_extension_support(VkPhysicalDevice device, GROEI_context *context);

u32 rate_device_suitability(VkPhysicalDevice device, GROEI_context *context);

void pick_physical_device(GROEI_context *context) {
  // TODO(moosch): move malloc to a memory arena
  const char **device_extensions = (const char**)malloc(sizeof(char*));
  device_extensions[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
  context->gpu = (gpu_device){
    .device_extension_count = 1,
    .device_extensions = device_extensions,
    .device = (VkPhysicalDevice){0},
  };

  u32 device_count = 0;
  vkEnumeratePhysicalDevices(context->instance, &device_count, NULL);

  if (device_count == 0) {
    printf("Failed to find a GPU with Vulkan support!\n");
    exit(GROEI_ERROR_PHYSICAL_DEVICE_CODE);
  }

  VkPhysicalDevice devices[device_count];
  vkEnumeratePhysicalDevices(context->instance, &device_count, devices);

  VkPhysicalDevice device;
  u32 device_score = 0;
  for (u32 i = 0; i < device_count; i++) {
    u32 score = rate_device_suitability(devices[i], context);
    if (score > device_score) {
      device_score = score;
      device = devices[i];
    }
  }

  if (device == NULL) {
    printf("Failed to find a stuitable GPU!\n");
    exit(GROEI_ERROR_PHYSICAL_DEVICE_CODE);
  }
  context->gpu.device = device;
  printf("GPU selected\n");

  context->queue_family_indices = find_queue_families(device, context->surface);
}

u32 rate_device_suitability(VkPhysicalDevice device, GROEI_context *context) {
  VkPhysicalDeviceProperties device_properties;
  VkPhysicalDeviceFeatures device_features;
  vkGetPhysicalDeviceProperties(device, &device_properties);
  vkGetPhysicalDeviceFeatures(device, &device_features);

  u32 score = 0;

  // Discrete GPUs have a significant performance advantage
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  // Maximum possible size of textures affects graphics quality
  score += device_properties.limits.maxImageDimension2D;

  // Applications can't function without geometry shaders
  if (!device_features.geometryShader) {
    return 0;
  }

  // Check device supports required queue families
  // Note: to improve performance, we could favour queue families that have both graphcs and present support.
  // We could check the returned indices and if they are the same, increase the score.
  queue_family_indices indices = find_queue_families(device, context->surface);
  if (!indices.is_graphics_family_set) {
    printf("Queue Family not supported!\n");
    return 0;
  }

  bool extensions_supported = check_device_extension_support(device, context);
  if (!extensions_supported) {
    printf("Required device extensions not supported!\n");
    return 0;
  }

  swap_chain_support_details swap_chain_support = query_swap_chain_support(device, context->surface);
  if (swap_chain_support.format_count == 0 || swap_chain_support.present_mode_count == 0) {
    printf("Swap chain not adequately supported!\n");
    return 0;
  }

  return score;
}

bool check_device_extension_support(VkPhysicalDevice device, GROEI_context *context) {
  u32 extension_count;
  vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, NULL);
  VkExtensionProperties available_extensions[extension_count];
  vkEnumerateDeviceExtensionProperties(device, NULL, &extension_count, available_extensions);

  printf("# of devices: %d\n", context->gpu.device_extension_count);

  for (u32 i = 0; i < context->gpu.device_extension_count; i++) {
    bool extension_found = false;
    for (u32 j = 0; j < extension_count; j++) {
      if (strcmp(context->gpu.device_extensions[i], available_extensions[j].extensionName) == 0) {
        extension_found = true;
        break;
      }
    }
    if (!extension_found) {
      printf("GPU extension %s not found\n", context->gpu.device_extensions[i]);
      return false;
    }
  }

  return true;
}
