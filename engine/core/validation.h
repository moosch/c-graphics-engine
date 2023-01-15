#include <string.h>
#include "context.h"

// TODO(moosch): This should be dynamically set on startup with these defaults
const u32 validation_layers_enabled = true;
const u32 validation_layer_count = 1;
const char *validation_layers[] = { "VK_LAYER_KHRONOS_validation" };

bool check_validation_layer_support() {
  u32 layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, NULL);

  VkLayerProperties available_layers[layer_count];
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers);

  for (u32 i = 0; i < validation_layer_count; i++) {
    bool layer_found = false;
    for (u32 j = 0; j < layer_count; j++) {
      if (strcmp(available_layers[j].layerName, validation_layers[i]) == 0) {
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
