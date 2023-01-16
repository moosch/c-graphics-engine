#include <stdlib.h>
#include <stdio.h>
#include "../definitions.h"
#include "frame_buffer.h"
#include "types.h"

void create_frame_buffer(GROEI_context *context) {
  context->swap_chain_frame_buffers = (VkFramebuffer*)malloc(context->swap_chain_image_count * sizeof(VkFramebuffer));

  for (u32 i = 0; i < context->swap_chain_image_count; i++) {
    VkImageView attachments[] = { context->swap_chain_image_views[i] };

    VkFramebufferCreateInfo frame_buffer_info = {};
    frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frame_buffer_info.renderPass = context->render_pass;
    frame_buffer_info.attachmentCount = 1;
    frame_buffer_info.pAttachments = attachments;
    frame_buffer_info.width = context->swap_chain_extent.width;
    frame_buffer_info.height = context->swap_chain_extent.height;
    frame_buffer_info.layers = 1;

    if (vkCreateFramebuffer(context->device, &frame_buffer_info, NULL, &context->swap_chain_frame_buffers[i]) != VK_SUCCESS) {
      printf("failed to create frame buffer!\n");
      exit(GROEI_ERROR_FRAME_BUFFER_CODE);
    }
  }
}
