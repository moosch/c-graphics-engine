#include <stdlib.h>
#include <stdio.h>
#include "surface.h"

void create_surface(GROEI_context *context) {
  if (glfwCreateWindowSurface(context->instance, context->window, NULL, &context->surface) != VK_SUCCESS) {
    printf("Failed to create window surface!\n");
    exit(GROEI_ERROR_SURFACE_CODE);
  }
}
