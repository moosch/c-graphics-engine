#ifndef _GROEI_SWAP_CHAIN_H_
#define _GROEI_SWAP_CHAIN_H_
#include "context.h"

swap_chain_support_details query_swap_chain_support(VkPhysicalDevice device, VkSurfaceKHR surface);

void create_swap_chain(GROEI_context *context, VkSwapchainKHR old_swap_chain);

void cleanup_swap_chain(GROEI_context *context);

void recreate_swap_chain(GROEI_context *context);

#endif
