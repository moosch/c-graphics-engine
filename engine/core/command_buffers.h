#ifndef _GROEI_COMMAND_BUFFERS_H_
#define _GROEI_COMMAND_BUFFERS_H_
#include "context.h"
#include "types.h"

void create_command_buffers(GROEI_context *context);

void record_command_buffer(GROEI_context *context,
                           vertices vertex_data,
                           VkCommandBuffer command_buffer,
                           u32 image_index);

#endif
