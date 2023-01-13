#include <stdio.h>
#include <stdlib.h>

#include "core/window.h"

/**
   The engine is the main application.
   It then loads another application (game).
   After engine setup, it asks the game what it wants to render, given some input.
   Input incluse:
     time passing - an update(dt) function
     user input - keyboard and controller events
*/

#define WIN_W 500
#define WIN_H 500

int main(void) {
  printf("Starting engine...\n");

  GLFWwindow *window = GROEI_create_window("title", WIN_W, WIN_H);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap buffers
    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
