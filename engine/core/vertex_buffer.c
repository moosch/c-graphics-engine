#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "vertex_buffer.h"

u32 find_memory_type(VkPhysicalDevice physical_device,
                     u32 type_filter,
                     VkMemoryPropertyFlags properties);

void create_buffer(VkDevice device,
                   VkPhysicalDevice physical_device,
                   VkDeviceSize size,
                   VkBufferUsageFlags usage,
                   VkMemoryPropertyFlags properties,
                   VkBuffer *buffer,
                   VkDeviceMemory *buffer_memory);

void create_vertex_buffers(GROEI_context *context, vertices *vertices_data) {
  assert(vertices_data->size >= 3 && "Vertex count must be at least 3");

  VkDeviceSize buffer_size = sizeof(vertices_data->list[0]) * vertices_data->size;

  create_buffer(context->device,
                context->gpu.device,
                buffer_size,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                &context->vertex_buffer,
                &context->vertex_buffer_memory);

  void* data;
  vkMapMemory(context->device, context->vertex_buffer_memory, 0, buffer_size, 0, &data);
  memcpy(data, vertices_data->list, buffer_size);
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

  GROEI_ERROR("failed to create vertex buffer!");
  exit(GROEI_ERROR_VERTEX_BUFFER_CODE);
}

void create_buffer(VkDevice device,
                   VkPhysicalDevice physical_device,
                   VkDeviceSize size,
                   VkBufferUsageFlags usage,
                   VkMemoryPropertyFlags properties,
                   VkBuffer *buffer,
                   VkDeviceMemory *buffer_memory) {
  VkBufferCreateInfo buffer_info = {0};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = size;
  buffer_info.usage = usage;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device, &buffer_info, NULL, buffer) != VK_SUCCESS) {
    GROEI_ERROR("Failed to create Vertex Bufer");
  }

  VkMemoryRequirements memory_requirements;
  vkGetBufferMemoryRequirements(device, *buffer, &memory_requirements);

  VkMemoryAllocateInfo alloc_info = {0};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = memory_requirements.size;
  alloc_info.memoryTypeIndex = find_memory_type(physical_device, memory_requirements.memoryTypeBits, properties);

  if (vkAllocateMemory(device, &alloc_info, NULL, buffer_memory) != VK_SUCCESS) {
    GROEI_ERROR("failed to allocate vertex buffer memory!");
    exit(GROEI_ERROR_VERTEX_BUFFER_CODE);
  }

  vkBindBufferMemory(device, *buffer, *buffer_memory, 0);
}
