#ifndef _GROEI_WINDOW_H_
#define _GROEI_WINDOW_H_

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

GLFWwindow* GROEI_create_window(const char *title, int width, int height);

#endif
