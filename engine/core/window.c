#include <stdlib.h>
#include <stdio.h>
#include "context.h"

/**
   TODO: This can fire an event to notify of resize in the loop to recreate swapchain.
   Or it could call a function pointer provided on init.
*/
void framebufferResizeCallback(GLFWwindow *window, int width, int height) {
  // framebufferResized = true;
  printf("Resized\n");
}

void create_window(const char *title, int width, int height, GROEI_context *context) {
  if (!glfwInit()) {
    printf("ERROR: could not initialise window context\n");
    exit(GROEI_ERROR_INIT_CODE);
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

  GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);

  glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

  if (window == NULL) {
    glfwTerminate();
    printf("ERROR: could not create a window\n");
    exit(GROEI_ERROR_WINDOW_CODE);
  }

  // Focus the window
  glfwMakeContextCurrent(window);

  context->window = window;
}
