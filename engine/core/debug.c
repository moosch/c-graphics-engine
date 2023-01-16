#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "../logger.h"
#include "../definitions.h"

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
                                              VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                              VkDebugUtilsMessageTypeFlagsEXT message_type,
                                              const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                              void *user_data);

VkResult create_debug_utils_messenger_ext(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *create_info,
                                          const VkAllocationCallbacks *allocator,
                                          VkDebugUtilsMessengerEXT *debug_messenger);

void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT *create_info) {
  create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info->pfnUserCallback = debug_callback;
}

void setup_debug_messenger(GROEI_context *context) {
  if (!context->validation_layers_enabled) return;

  VkDebugUtilsMessengerCreateInfoEXT create_info = {0};
  populate_debug_messenger_create_info(&create_info);
  // createInfo.pUserData = NULL;

  if (create_debug_utils_messenger_ext(context->instance, &create_info, NULL, &context->debug_messenger) != VK_SUCCESS) {
    GROEI_ERROR("Failed to setup debug messenger!\n");
    exit(GROEI_ERROR_DEBUG_MESSENGER_CODE);
  }
}

void destroy_debug_utils_messenger_ext(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {
  PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != NULL) {
    func(instance, debugMessenger, pAllocator);
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data) {

  GROEI_DEBUG("Type: %d\n", message_type);
  GROEI_DEBUG("Severity: %d\n", message_severity);
  if (user_data!= NULL) {
    GROEI_DEBUG("Data %p\n", user_data);
  }
  GROEI_DEBUG("validation layer: %s\n", callback_data->pMessage);

  return VK_FALSE;
}

VkResult create_debug_utils_messenger_ext(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *create_info,
                                          const VkAllocationCallbacks *allocator,
                                          VkDebugUtilsMessengerEXT *debug_messenger) { PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

  if (func != NULL) {
    return func(instance, create_info, allocator, debug_messenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}
