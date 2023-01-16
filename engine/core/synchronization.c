#include <stdlib.h>
#include <stdio.h>
#include "synchronization.h"
#include "../definitions.h"

void setup_synchronization(GROEI_context *context) {
  context->image_available_semaphores = (VkSemaphore*)malloc(sizeof(VkSemaphore) * context->max_frames_in_flight);
  context->render_finished_semaphores = (VkSemaphore*)malloc(sizeof(VkSemaphore) * context->max_frames_in_flight);
  context->in_flight_fences = (VkFence*)malloc(sizeof(VkFence) * context->max_frames_in_flight);

  VkSemaphoreCreateInfo semaphoreInfo = {};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo = {};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (int i = 0; i < context->max_frames_in_flight; i++) {
    if (vkCreateSemaphore(context->device, &semaphoreInfo, NULL, &context->image_available_semaphores[i]) != VK_SUCCESS) {
      printf("Failed to create imageAvailableSemaphore!\n");
      exit(GROEI_ERROR_SYNCHRONIZATION_CODE);
    }
    if (vkCreateSemaphore(context->device, &semaphoreInfo, NULL, &context->render_finished_semaphores[i]) != VK_SUCCESS) {
      printf("Failed to create renderFinishedSemaphore!\n");
      exit(GROEI_ERROR_SYNCHRONIZATION_CODE);
    }
    if (vkCreateFence(context->device, &fenceInfo, NULL, &context->in_flight_fences[i]) != VK_SUCCESS) {
      printf("Failed to create fence!\n");
      exit(GROEI_ERROR_SYNCHRONIZATION_CODE);
    }
  }
}
