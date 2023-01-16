#include <stdlib.h>
#include <stdio.h>
#include "image_views.h"

void create_image_views(GROEI_context *context) {
  context->swap_chain_image_views = (VkImageView*)malloc(sizeof(VkImageView) * context->swap_chain_image_count);

  for (u32 i = 0; i < context->swap_chain_image_count; i++) {
    VkImageViewCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .image = context->swap_chain_images[i],
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = context->swap_chain_image_format,
      .components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
      .components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
      .components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
      .components.a = VK_COMPONENT_SWIZZLE_IDENTITY,
      .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
      .subresourceRange.baseMipLevel = 0,
      .subresourceRange.levelCount = 1,
      .subresourceRange.baseArrayLayer = 0,
      .subresourceRange.layerCount = 1
    };

    if (vkCreateImageView(context->device, &createInfo, NULL, &context->swap_chain_image_views[i]) != VK_SUCCESS) {
      printf("Failed to create image views!\n");
      exit(GROEI_ERROR_IMAGE_VIEWS_CODE);
    }
  }
}
