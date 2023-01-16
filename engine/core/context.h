#ifndef _GROEI_CONTEXT_H_
#define _GROEI_CONTEXT_H_

#include <stdbool.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../definitions.h"

typedef struct swap_chain_support_details {
  VkSurfaceCapabilitiesKHR capabilities;
  u32 format_count;
  VkSurfaceFormatKHR *formats;
  u32 present_mode_count;
  VkPresentModeKHR *present_modes;
} swap_chain_support_details;

typedef struct queue_family_indices {
  u32 graphics_family;
  bool is_graphics_family_set;
  u32 present_family;
  bool is_present_family_set;
} queue_family_indices;

typedef struct gpu_device {
  u32 device_extension_count;
  const char** device_extensions;
  VkPhysicalDevice device;
} gpu_device;

typedef struct GROEI_context {
  GLFWwindow *window;
  u32 window_width;
  u32 window_height;
  // TODO: Thest Vulkan specific things could be behind a simpler API
  // A lot of these will be moved
  VkInstance instance;
  VkSurfaceKHR surface;
  gpu_device gpu;
  queue_family_indices queue_family_indices;
  VkDevice device; // Logical device
  VkQueue graphics_queue;
  VkQueue present_queue;
  VkSwapchainKHR swap_chain;
  u32 swap_chain_image_count;
  VkImage *swap_chain_images;
  VkFormat swap_chain_image_format;
  VkExtent2D swap_chain_extent;
  VkImageView *swap_chain_image_views;
  VkRenderPass render_pass;
  VkPipelineLayout pipeline_layout;
  VkPipeline graphics_pipeline;
  VkFramebuffer *swap_chain_frame_buffers;
  u8 max_frames_in_flight;
  VkCommandPool command_pool;
  VkCommandBuffer *command_buffers;
  u32 command_buffer_count;
  VkSemaphore *image_available_semaphores;
  VkSemaphore *render_finished_semaphores;
  VkFence *in_flight_fences;
  VkBuffer vertex_buffer;
  VkDeviceMemory vertex_buffer_memory;
  VkDebugUtilsMessengerEXT debug_messenger;
  bool validation_layers_enabled;
  u32 validation_layer_count;
  const char* const *validation_layers;
  bool frame_buffer_resized;
  u32 current_frame;
} GROEI_context;

#endif
