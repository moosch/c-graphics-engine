/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #define GLFW_INCLUDE_GLEXT */
/* #define GLFW_INCLUDE_VULKAN */
/* #include <GLFW/glfw3.h> */
/* #include "../definitions.h" */

/* GLFWwindow* GROEI_create_window(const char *title, int width, int height) { */
/*   if (!glfwInit()) { */
/*     printf("ERROR: could not initialise window context\n"); */
/*     exit(GROEI_ERROR_INIT_CODE); */
/*   } */

/*   /\* glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); *\/ */
/*   /\* glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); *\/ */
/*   /\* glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4); *\/ */

/*   // Moving to Vulkan */
/*   glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); */
/*   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); */
/*   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4); */

/*   GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL); */

/*   glfwSetFramebufferSizeCallback(window, framebufferResizeCallback); */

/*   if (window == NULL) { */
/*     glfwTerminate(); */
/*     printf("ERROR: could not create a window\n"); */
/*     exit(GROEI_ERROR_WINDOW_CODE); */
/*   } */

/*   // Focus the window */
/*   glfwMakeContextCurrent(window); */

/*   return window; */
/* } */
