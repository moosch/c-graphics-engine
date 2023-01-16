#include <stdlib.h>
#include <stdio.h>
#include "command_pool.h"
#include "../definitions.h"
#include "queue_families.h"

void create_command_pool(GROEI_context *context) {
  queue_family_indices indices = find_queue_families(context->gpu.device, context->surface);

  VkCommandPoolCreateInfo pool_info = {};
  pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  pool_info.queueFamilyIndex = indices.graphics_family;

  if (vkCreateCommandPool(context->device, &pool_info, NULL, &context->command_pool) != VK_SUCCESS) {
    printf("failed to create command pool!\n");
    exit(GROEI_ERROR_COMMAND_POOL_CODE);
  }
}
