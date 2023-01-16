#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instance.h"
#include "debug.h"
#include "../definitions.h"

bool check_validation_layer_support(GROEI_context *context);
bool verify_extension_support(u32 extension_count,
                              VkExtensionProperties *extensions,
                              u32 glfw_extension_count,
                              const char** glfw_extensions);

void create_instance(const char *title, GROEI_context *context) {

  if (context->validation_layers_enabled && !check_validation_layer_support(context)) {
    printf("Validation layers requested but not available!\n");
    exit(GROEI_ERROR_VALIDATION_LAYERS);
  }

  VkApplicationInfo app_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = title,
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "No Engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    .apiVersion = VK_API_VERSION_1_0,
    .pNext = NULL
  };

  u32 glfw_extension_count = 0;
  const char** glfw_extensions;

  glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

  const char *glfw_extensions_with_debug[glfw_extension_count+1];

  for (u32 i = 0; i < glfw_extension_count; i++) {
    glfw_extensions_with_debug[i] = glfw_extensions[i];
  }
  if (context->validation_layers_enabled) {
    glfw_extensions_with_debug[glfw_extension_count] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
  }

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {0};
  VkInstanceCreateInfo create_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &app_info,
  };
  if (context->validation_layers_enabled) {
    create_info.enabledLayerCount = context->validation_layer_count;
    create_info.ppEnabledLayerNames = context->validation_layers;
    create_info.enabledExtensionCount = glfw_extension_count + 1;
    create_info.ppEnabledExtensionNames = glfw_extensions_with_debug;

    populate_debug_messenger_create_info(&debug_create_info);
    create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
  } else {
    create_info.enabledLayerCount = 0;
    create_info.enabledExtensionCount = glfw_extension_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;
    create_info.pNext = NULL;
  }

  if (vkCreateInstance(&create_info, NULL, &context->instance) != VK_SUCCESS) {
    printf("Failed to create Vulkan Instance\n");
    exit(GROEI_ERROR_VALIDATION_LAYERS);
  }

  // Vulkan extensions
  u32 extension_count = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &extension_count, NULL);

  VkExtensionProperties extensions[extension_count];
  vkEnumerateInstanceExtensionProperties(NULL, &extension_count, extensions);

  if (!verify_extension_support(extension_count, extensions, glfw_extension_count, glfw_extensions)) {
    printf("Missing extension support!\n");
    exit(GROEI_ERROR_VALIDATION_LAYERS);
  }
}

bool verify_extension_support(u32 extension_count,
                              VkExtensionProperties *extensions,
                              u32 glfw_extension_count,
                              const char** glfw_extensions) {
  for (u32 i = 0; i < glfw_extension_count; i++) {
    bool extensionFound = false;
    for (u32 j = 0; j < extension_count; j++) {
      if (strcmp(extensions[j].extensionName, glfw_extensions[i]) == 0) {
        extensionFound = true;
        break;
      }
    }
    if (!extensionFound) {
      return false;
    }
  }

  return true;
}

bool check_validation_layer_support(GROEI_context *context) {
  u32 layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, NULL);

  VkLayerProperties available_layers[layer_count];
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

  for (u32 i = 0; i < context->validation_layer_count; i++) {
    bool layer_found = false;
    for (u32 j = 0; j < layer_count; j++) {
      if (strcmp(available_layers[j].layerName, context->validation_layers[i]) == 0) {
        layer_found = true;
        break;
      }
    }
    if (!layer_found) {
      return false;
    }
  }

  return true;
}
