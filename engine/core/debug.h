#ifndef _GROEI_DEBUG_MESSENGER_H_
#define _GROEI_DEBUG_MESSENGER_H_
#include "context.h"

void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT *create_info);

void setup_debug_messenger(GROEI_context *context) ;

void destroy_debug_utils_messenger_ext(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);

#endif
