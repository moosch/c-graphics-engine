#ifndef _GROEI_GRAPHICS_PIPELINE_H_
#define _GROEI_GRAPHICS_PIPELINE_H_
#include "context.h"

void create_graphics_pipeline(GROEI_context *context,
                              const char *vertext_shader_path,
                              const char *fragment_shader_path);

#endif
