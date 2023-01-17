#include <stdlib.h>
#include <stdio.h>
#include "window.h"

void on_frame_buffer_resize(GLFWwindow *window, int width, int height);

void create_window(const char *title, int width, int height, GROEI_context *context) {
  if (!glfwInit()) {
    GROEI_ERROR("ERROR: could not initialise window context");
    exit(GROEI_ERROR_WINDOW_CODE);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

  GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (window == NULL) {
    glfwTerminate();
    printf("ERROR: could not create a window\n");
    exit(GROEI_ERROR_WINDOW_CODE);
  }

  // Focus the window
  glfwMakeContextCurrent(window);

  context->window = window;

  glfwSetWindowUserPointer(context->window, context);
  glfwSetFramebufferSizeCallback(context->window, on_frame_buffer_resize);
}

bool was_window_resized(GLFWwindow *window) {
  GROEI_context *context = glfwGetWindowUserPointer(window);
  return context->frame_buffer_resized;
}
void reset_window_resized(GROEI_context *context) {
  context->frame_buffer_resized = false;
}

void on_frame_buffer_resize(GLFWwindow *window, int width, int height) {
  GROEI_context *context = glfwGetWindowUserPointer(window);
  context->frame_buffer_resized = true;
  context->window_width = width;
  context->window_height = height;
}
