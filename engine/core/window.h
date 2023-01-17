#ifndef _GROEI_WINDOW_H_
#define _GROEI_WINDOW_H_
#include <stdbool.h>
#include "context.h"

void create_window(const char *title, int width, int height, GROEI_context *context);

bool was_window_resized(GLFWwindow *window);

void reset_window_resized(GROEI_context *context);

#endif
