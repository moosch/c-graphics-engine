#include <stdlib.h>
#include <stdio.h>
#include "command_buffers.h"
#include "../definitions.h"

void create_command_buffers(GROEI_context *context) {
  context->command_buffer_count = context->max_frames_in_flight;
  context->command_buffers = (VkCommandBuffer*)malloc(sizeof(VkCommandBuffer) * context->max_frames_in_flight);

  VkCommandBufferAllocateInfo alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool = context->command_pool;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = context->max_frames_in_flight;

  if (vkAllocateCommandBuffers(context->device, &alloc_info, context->command_buffers) != VK_SUCCESS) {
    printf("failed to allocate command buffers!\n");
    exit(GROEI_ERROR_COMMAND_BUFFERS_CODE);
  }
}

void record_command_buffer(GROEI_context *context,
                           vertices vertex_data,
                           VkCommandBuffer command_buffer,
                           u32 image_index) {
  VkCommandBufferBeginInfo begin_info = {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = 0; // Optional
  begin_info.pInheritanceInfo = NULL; // Optional

  if (vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS) {
    printf("failed to begin recording command buffer!\n");
    exit(GROEI_ERROR_COMMAND_BUFFER_RECORD_CODE);
  }

  VkRenderPassBeginInfo renderPass_info = {};
  renderPass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPass_info.renderPass = context->render_pass;
  renderPass_info.framebuffer = context->swap_chain_frame_buffers[image_index];
  renderPass_info.renderArea.offset.x = 0;
  renderPass_info.renderArea.offset.y = 0;
  renderPass_info.renderArea.extent = context->swap_chain_extent;

  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPass_info.clearValueCount = 1;
  renderPass_info.pClearValues = &clear_color;

  vkCmdBeginRenderPass(command_buffer, &renderPass_info, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, context->graphics_pipeline);


  VkViewport viewport = {0};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)context->swap_chain_extent.width;
  viewport.height = (float)context->swap_chain_extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  VkRect2D scissor = {0};
  scissor.offset.x = 0;
  scissor.offset.y = 0;
  scissor.extent = context->swap_chain_extent;
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);

  VkBuffer vertexBuffers[] = {context->vertex_buffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);

  vkCmdDraw(command_buffer, vertex_data.size, 1, 0, 0);

  vkCmdEndRenderPass(command_buffer);

  if (vkEndCommandBuffer(command_buffer) != VK_SUCCESS) {
    printf("failed to record command buffer!\n");
    exit(GROEI_ERROR_COMMAND_BUFFER_RECORD_CODE);
  }
}
