#include <stdio.h>
#include <stdlib.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "window.h"
#include "debug.h"
#include "instance.h"
#include "surface.h"
#include "physical_device.h"
#include "logical_device.h"
#include "swap_chain.h"

void cleanup_swap_chain(GROEI_context *context);

void engine_setup(GROEI_context *context, const char *title, u32 width, u32 height) {
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

  create_instance(title, context);

  setup_debug_messenger(context);

  create_surface(context);

  pick_physical_device(context);

  create_logical_device(context);

  create_swap_chain(context);

  // [x] create window
  // [x] setup debug messenger
  // [x] create instance
  // [x] create surface
  // [x] pick physical device
  // [x] create logical device
  // [x] create swapchain
  // [ ] create image views
  // [ ] create Render Pass
  // [ ] create Graphics Pipeline
  // [ ] create Frame Buffers
  // [ ] create Command Pool
  // [ ] create Vertex Buffer
  // [ ] create Command Buffers
  // [ ] create Sync Objects

  printf("Engine setup successfully\n");
}

void engine_teardown(GROEI_context *context) {
  cleanup_swap_chain(context);

  /* vkDestroyBuffer(context->device, context->vertex_buffer, NULL); */
  /* vkFreeMemory(context->device, context->vertex_buffer_memory, NULL); */

  /* for (u8 i = 0; i < context->max_frames_in_flight; i++) { */
  /*   vkDestroySemaphore(context->device, context->image_available_semaphores[i], NULL); */
  /*   vkDestroySemaphore(context->device, context->render_finished_semaphores[i], NULL); */
  /*   vkDestroyFence(context->device, context->in_flight_fences[i], NULL); */
  /* } */

  /* vkDestroyCommandPool(context->device, context->command_pool, NULL); */

  /* vkDestroyPipeline(context->device, context->graphics_pipeline, NULL); */
  /* vkDestroyPipelineLayout(context->device, context->pipeline_layout, NULL); */
  /* vkDestroyRenderPass(context->device, context->render_pass, NULL); */

  if (context->validation_layers_enabled) {
    destroy_debug_utils_messenger_ext(context->instance, context->debug_messenger, NULL);
  }
  vkDestroyDevice(context->device, NULL);

  vkDestroySurfaceKHR(context->instance, context->surface, NULL);
  vkDestroyInstance(context->instance, NULL);

  glfwDestroyWindow(context->window);

  glfwTerminate();
}

void cleanup_swap_chain(GROEI_context *context) {
  for (u32 i = 0; i < context->swap_chain_image_count; i++) {
    vkDestroyFramebuffer(context->device, context->swap_chain_framebuffers[i], NULL);
  }

  for (u32 i = 0; i < context->swap_chain_image_count; i++) {
    vkDestroyImageView(context->device, context->swap_chain_image_views[i], NULL);
  }

  vkDestroySwapchainKHR(context->device, context->swap_chain, NULL);
}
