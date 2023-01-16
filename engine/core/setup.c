#include <stdio.h>
#include <stdlib.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "setup.h"
#include "window.h"
#include "debug.h"
#include "instance.h"
#include "surface.h"
#include "physical_device.h"
#include "logical_device.h"
#include "swap_chain.h"
#include "image_views.h"
#include "render_pass.h"
#include "graphics_pipeline.h"
#include "frame_buffer.h"
#include "command_pool.h"
#include "vertex_buffer.h"
#include "command_buffers.h"
#include "synchronization.h"
#include "renderer.h"

void engine_setup(GROEI_context *context,
                  const char *title,
                  u32 width, u32 height,
                  const char *vertext_shader_path,
                  const char *fragment_shader_path,
                  vertices vertices_data) {
  context->window_width = width;
  context->window_height = height;

  // TODO(moosch): This should be dynamically set on startup with these defaults
  context->validation_layers_enabled = true;
  context->validation_layer_count = 1;
  context->max_frames_in_flight = 2;
  const char **validation_layers = (const char**)malloc(sizeof(char**));
  validation_layers[0] = "VK_LAYER_KHRONOS_validation";
  context->validation_layers = validation_layers;

  create_window(title, width, height, context);
  printf("Window created\n");

  create_instance(title, context);
  printf("Instace created\n");

  setup_debug_messenger(context);
  printf("Debug messenger setup\n");

  create_surface(context);
  printf("Surface created\n");

  pick_physical_device(context);
  printf("GPU selected\n");

  create_logical_device(context);
  printf("Logical Device created\n");

  create_swap_chain(context);
  printf("Swap Chain created\n");

  create_image_views(context);
  printf("Image Views created\n");

  create_render_pass(context);
  printf("Render Pass created\n");

  create_graphics_pipeline(context, vertext_shader_path, fragment_shader_path);
  printf("Graphics Pipeline created\n");

  create_frame_buffer(context);
  printf("Frame Buffers created\n");

  create_command_pool(context);
  printf("Command Pool created\n");

  create_vertex_buffer(context, vertices_data);
  printf("Vertex Buffer created\n");

  create_command_buffers(context);
  printf("Command Buffer created\n");

  setup_synchronization(context);
  printf("Synchronization setup\n");

  printf("Engine setup successfully\n");
}

void on_update(GROEI_context *context, vertices vertices_data) {
  draw_frame(context, vertices_data);
}

void engine_teardown(GROEI_context *context) {
  vkDeviceWaitIdle(context->device);

  cleanup_swap_chain(context);

  vkDestroyBuffer(context->device, context->vertex_buffer, NULL);
  vkFreeMemory(context->device, context->vertex_buffer_memory, NULL);

  for (u8 i = 0; i < context->max_frames_in_flight; i++) {
    vkDestroySemaphore(context->device, context->image_available_semaphores[i], NULL);
    vkDestroySemaphore(context->device, context->render_finished_semaphores[i], NULL);
    vkDestroyFence(context->device, context->in_flight_fences[i], NULL);
  }

  vkDestroyCommandPool(context->device, context->command_pool, NULL);

  vkDestroyPipeline(context->device, context->graphics_pipeline, NULL);
  vkDestroyPipelineLayout(context->device, context->pipeline_layout, NULL);
  vkDestroyRenderPass(context->device, context->render_pass, NULL);

  if (context->validation_layers_enabled) {
    destroy_debug_utils_messenger_ext(context->instance, context->debug_messenger, NULL);
  }
  vkDestroyDevice(context->device, NULL);

  vkDestroySurfaceKHR(context->instance, context->surface, NULL);
  vkDestroyInstance(context->instance, NULL);

  glfwDestroyWindow(context->window);

  glfwTerminate();
}
