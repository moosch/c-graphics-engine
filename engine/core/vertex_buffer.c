#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vertex_buffer.h"

u32 find_memory_type(VkPhysicalDevice physical_device,
                     u32 type_filter,
                     VkMemoryPropertyFlags properties);

void create_vertex_buffer(GROEI_context *context, vertices vertices_data) {
  VkBufferCreateInfo buffer_info = {0};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = sizeof(vertices_data.list[0]) * vertices_data.size;
  buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(context->device, &buffer_info, NULL, &context->vertex_buffer) != VK_SUCCESS) {
    printf("failed to create vertex buffer!\n");
    exit(GROEI_ERROR_VERTEX_BUFFER_CODE);
  }

  VkMemoryRequirements mem_requirements;
  vkGetBufferMemoryRequirements(context->device, context->vertex_buffer, &mem_requirements);

  VkMemoryAllocateInfo alloc_info = {0};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = mem_requirements.size;
  alloc_info.memoryTypeIndex = find_memory_type(context->gpu.device, mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  if (vkAllocateMemory(context->device, &alloc_info, NULL, &context->vertex_buffer_memory) != VK_SUCCESS) {
    printf("failed to allocate vertex buffer memory!\n");
    exit(GROEI_ERROR_VERTEX_BUFFER_CODE);
  }

  vkBindBufferMemory(context->device, context->vertex_buffer, context->vertex_buffer_memory, 0);

  void* data;
  vkMapMemory(context->device, context->vertex_buffer_memory, 0, buffer_info.size, 0, &data);
  memcpy(data, vertices_data.list, buffer_info.size);
  vkUnmapMemory(context->device, context->vertex_buffer_memory);
}

u32 find_memory_type(VkPhysicalDevice physical_device,
                        u32 type_filter,
                        VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(physical_device, &mem_properties);

  for (u32 i = 0; i < mem_properties.memoryTypeCount; i++) {
    if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  printf("failed to create vertex buffer!\n");
  exit(GROEI_ERROR_VERTEX_BUFFER_CODE);
}
