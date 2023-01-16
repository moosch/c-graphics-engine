#include <stdlib.h>
#include <stdio.h>
#include "logical_device.h"
#include "queue_families.h"

void get_family_device_queues(VkDeviceQueueCreateInfo *queues, queue_family_indices indices);

void create_logical_device(GROEI_context *context) {
  queue_family_indices indices = find_queue_families(context->gpu.device, context->surface);

  VkPhysicalDeviceFeatures device_features;
  vkGetPhysicalDeviceFeatures(context->gpu.device, &device_features);

  VkDeviceQueueCreateInfo queues[2];
  get_family_device_queues(queues, indices);

  VkDeviceCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    //.pQueueCreateInfos = &queueCreateInfo,
    .pQueueCreateInfos = queues,
    .queueCreateInfoCount = 1,
    .pEnabledFeatures = &device_features,
    .enabledExtensionCount = context->gpu.device_extension_count,
    .ppEnabledExtensionNames = context->gpu.device_extensions
  };

  if (context->validation_layers_enabled) {
    create_info.enabledLayerCount = context->validation_layer_count;
    create_info.ppEnabledLayerNames = context->validation_layers;
  } else {
    create_info.enabledLayerCount = 0;
  }

  if (vkCreateDevice(context->gpu.device, &create_info, NULL, &context->device) != VK_SUCCESS) {
    printf("Failed to create logical device!\n");
    exit(GROEI_ERROR_LOGICAL_DEVICE_CODE);
  }

  vkGetDeviceQueue(context->device, context->queue_family_indices.graphics_family, 0, &context->graphics_queue);
  vkGetDeviceQueue(context->device, context->queue_family_indices.present_family, 0, &context->present_queue);
}

void get_family_device_queues(VkDeviceQueueCreateInfo *queues, queue_family_indices indices) {
  // GraphicsQueue
  VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .queueFamilyIndex = indices.graphics_family,
    .queueCount = 1
  };

  float graphicsQueuePriority = 1.0f;
  graphicsQueueCreateInfo.pQueuePriorities = &graphicsQueuePriority;
  queues[0] = graphicsQueueCreateInfo;

  // PresentQueue
  VkDeviceQueueCreateInfo presentQueueCreateInfo = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .queueFamilyIndex = indices.present_family,
    .queueCount = 1
  };

  float presentQueuePriority = 1.0f;
  graphicsQueueCreateInfo.pQueuePriorities = &presentQueuePriority;
  queues[1] = presentQueueCreateInfo;
}
