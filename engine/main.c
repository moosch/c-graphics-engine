#include <stdio.h>
#include <stdlib.h>

#include "core/setup.h"

/**
   The engine is the main application.
   It then loads another application (game).

   After engine setup, it asks the game what it wants to render, given some input.
   Input incluse:
     time passing - an update(dt) function
     user input - keyboard and controller events

   The interface will likely be something similar to Love2D.
   This would be a super simple game loop

   setup() {
     GREOI_context ctx = {0};
     GREOI_setup(&ctx);
     // Load vertices
     vert vertices[];
     GREOI_load_vertices(&ctx, "path-to", &vertices);
   }

   update() {
     float dt = GREOI_prepare();
     Event event;
     while (GROEI_poll_events(&event)) {
       // do some data updates
     }
     // do other data updates
     GREOI_render(&ctx);
   }

   cleanup() {
     GREOI_teardown(&ctx);
   }
*/

int main(void) {
  printf("Starting engine...\n");

  const u32 window_width = 500;
  const u32 window_height = 500;
  GROEI_context context = {0};
  // TODO(moosch): get the window name from the application
  engine_setup(&context, "Engine testing", window_width, window_height);

  while (!glfwWindowShouldClose(context.window)) {
    glfwPollEvents();
  }

  engine_teardown(&context);

  glfwTerminate();

  return 0;
}
