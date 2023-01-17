#ifndef _GROEI_ENGINE_SETUP_H_
#define _GROEI_ENGINE_SETUP_H_
#include "context.h"
#include "types.h"

void engine_setup(GROEI_context *context,
                  const char *title,
                  u32 width, u32 height,
                  const char *vertext_shader_path,
                  const char *fragment_shader_path,
                  vertices *vertices_data);

void on_update(GROEI_context *context, vertices *vertices_data);

void engine_teardown(GROEI_context *context);

#endif
