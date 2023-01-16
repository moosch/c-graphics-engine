#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "context.h"
#include "queue_families.h"
#include "image_views.h"
#include "frame_buffer.h"

VkSurfaceFormatKHR choose_swap_surface_format(u32 format_count, VkSurfaceFormatKHR *available_formats);
VkPresentModeKHR choose_swap_present_mode(u32 present_mode_count, VkPresentModeKHR *available_present_modes);
VkExtent2D choose_swap_extent(GLFWwindow *window, VkSurfaceCapabilitiesKHR capabilities);
u32 clamp_u32(u32 n, u32 min, u32 max);

swap_chain_support_details query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface) {
  swap_chain_support_details details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

  u32 format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, NULL);
  details.format_count = format_count;
  VkSurfaceFormatKHR formats[format_count];
  details.formats = formats;
  if (format_count != 0) {
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats);
  }

  u32 present_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_count, NULL);
  details.present_mode_count = present_count;
  VkPresentModeKHR present_modes[present_count];
  details.present_modes = present_modes;
  if (present_count != 0) {
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_count, details.present_modes);
  }

  return details;
}

void create_swap_chain(GROEI_context *context) {
  swap_chain_support_details swap_chain_support = query_swap_chain_support(context->gpu.device, context->surface);

  VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(swap_chain_support.format_count, swap_chain_support.formats);
  VkPresentModeKHR present_mode = choose_swap_present_mode(swap_chain_support.present_mode_count, swap_chain_support.present_modes);
  VkExtent2D extent = choose_swap_extent(context->window, swap_chain_support.capabilities);

  u32 image_count = swap_chain_support.capabilities.minImageCount + 1;
  if (swap_chain_support.capabilities.maxImageCount > 0 && image_count > swap_chain_support.capabilities.maxImageCount) {
    image_count = swap_chain_support.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR create_info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .surface = context->surface,
    .minImageCount = image_count,
    .imageFormat = surfaceFormat.format,
    .imageColorSpace = surfaceFormat.colorSpace,
    .imageExtent = extent,
    .imageArrayLayers = 1,
    .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
  };

  queue_family_indices indices = find_queue_families(context->gpu.device, context->surface);
  u32 queueFamilyIndices[] = { indices.graphics_family, indices.present_family };

  if (indices.graphics_family != indices.present_family) {
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.queueFamilyIndexCount = 0; // Optional
    create_info.pQueueFamilyIndices = NULL; // Optional
  }

  create_info.preTransform = swap_chain_support.capabilities.currentTransform;
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE;
  create_info.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(context->device, &create_info, NULL, &context->swap_chain) != VK_SUCCESS) {
    printf("Failed to create Swap Chain!\n");
    exit(GROEI_ERROR_SWAP_CHAIN_CODE);
  }

  vkGetSwapchainImagesKHR(context->device, context->swap_chain, &image_count, NULL);
  context->swap_chain_images = (VkImage*)malloc(sizeof(VkImage) * image_count);

  vkGetSwapchainImagesKHR(context->device, context->swap_chain, &image_count, context->swap_chain_images);
  context->swap_chain_image_count = image_count;

  context->swap_chain_image_format = surfaceFormat.format;
  context->swap_chain_extent = extent;
}

void cleanup_swap_chain(GROEI_context *context) {
  for (u32 i = 0; i < context->swap_chain_image_count; i++) {
    vkDestroyFramebuffer(context->device, context->swap_chain_frame_buffers[i], NULL);
  }

  for (u32 i = 0; i < context->swap_chain_image_count; i++) {
    vkDestroyImageView(context->device, context->swap_chain_image_views[i], NULL);
  }

  // TODO(moosch): Move this free into a memory arena
  free(context->swap_chain_images);

  vkDestroySwapchainKHR(context->device, context->swap_chain, NULL);
}

void recreate_swap_chain(GROEI_context *context) {
  int width = 0;
  int height = 0;
  glfwGetFramebufferSize(context->window, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(context->window, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(context->device);

  cleanup_swap_chain(context);

  create_swap_chain(context);
  create_image_views(context);
  create_frame_buffer(context);
}

VkSurfaceFormatKHR choose_swap_surface_format(u32 format_count, VkSurfaceFormatKHR *available_formats) {
  for (u32 i = 0; i < format_count; i++) {
    if (available_formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && available_formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return available_formats[i];
    }
  }
  return available_formats[0];
}

VkPresentModeKHR choose_swap_present_mode(u32 present_mode_count, VkPresentModeKHR *available_present_modes) {
  for (u32 i = 0; i < present_mode_count; i++) {
    if (available_present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      return available_present_modes[i];
    }
  }
  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D choose_swap_extent(GLFWwindow *window, VkSurfaceCapabilitiesKHR capabilities) {
  if (capabilities.currentExtent.width != UINT_MAX) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actual_extent = { (u32)width, (u32)height };

    actual_extent.width = clamp_u32(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actual_extent.height = clamp_u32(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actual_extent;
  }
}

u32 clamp_u32(u32 n, u32 min, u32 max) {
  if (n < min) return min;
  if (n > max) return max;
  return n;
}
