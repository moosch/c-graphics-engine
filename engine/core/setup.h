#ifndef _C_GROEI_ENGINE_SETUP_H_
#define _C_GROEI_ENGINE_SETUP_H_

#include "context.h"

void engine_setup(GROEI_context *context, const char *title, u32 width, u32 height);
void engine_teardown(GROEI_context *context);

#endif
