#include <stdlib.h>
#include <stdio.h>
#include "renderer.h"
#include "swap_chain.h"
#include "command_buffers.h"

void draw_frame(GROEI_context *context, vertices *vertices_data) {
  vkWaitForFences(context->device, 1, &context->in_flight_fences[context->current_frame], VK_TRUE, UINT64_MAX);

  vkResetFences(context->device, 1, &context->in_flight_fences[context->current_frame]);

  uint32_t image_index;
  VkResult result = vkAcquireNextImageKHR(context->device, context->swap_chain, UINT64_MAX, context->image_available_semaphores[context->current_frame], VK_NULL_HANDLE, &image_index);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreate_swap_chain(context);
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    printf("Failed to acquire swap chain image!\n");
    exit(GROEI_ERROR_RENDERER_CODE);
  }

  // Only reset the fence if we are submitting work
  vkResetFences(context->device, 1, &context->in_flight_fences[context->current_frame]);

  vkResetCommandBuffer(context->command_buffers[context->current_frame], 0);
  record_command_buffer(context, *vertices_data, context->command_buffers[context->current_frame], image_index);

  VkSubmitInfo submit_info = {0};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = { context->image_available_semaphores[context->current_frame] };
  VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;

  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &context->command_buffers[context->current_frame];

  VkSemaphore signal_semaphores[] = { context->render_finished_semaphores[context->current_frame] };
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;

  if (vkQueueSubmit(context->graphics_queue, 1, &submit_info, context->in_flight_fences[context->current_frame]) != VK_SUCCESS) {
    printf("Failed to submit draw command buffer!\n");
    exit(GROEI_ERROR_RENDERER_CODE);
  }

  VkPresentInfoKHR present_info = {0};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;

  VkSwapchainKHR swap_chains[] = { context->swap_chain };
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swap_chains;
  present_info.pImageIndices = &image_index;

  present_info.pResults = NULL; // Optional

  VkResult queue_result = vkQueuePresentKHR(context->present_queue, &present_info);

  if (queue_result == VK_ERROR_OUT_OF_DATE_KHR || queue_result == VK_SUBOPTIMAL_KHR || context->frame_buffer_resized) {
    context->frame_buffer_resized = false;
    recreate_swap_chain(context);
  } else if (queue_result != VK_SUCCESS) {
    printf("Failed to present swap chain image!\n");
    exit(GROEI_ERROR_RENDERER_CODE);
  }

  context->current_frame = (context->current_frame + 1) % context->max_frames_in_flight;
}
