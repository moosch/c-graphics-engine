#include <stdlib.h>
#include <stdio.h>
#include "context.h"

void on_frame_buffer_resize(GLFWwindow *window, int width, int height);

void create_window(const char *title, int width, int height, GROEI_context *context) {
  if (!glfwInit()) {
    printf("ERROR: could not initialise window context\n");
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

  glfwSetFramebufferSizeCallback(context->window, on_frame_buffer_resize);
}

void on_frame_buffer_resize(GLFWwindow *window, int width, int height) {
  printf("Window resized...handle later\n");
}
