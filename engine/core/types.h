#ifndef _GROEI_TYPES_H_
#define _GROEI_TYPES_H_
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include "../definitions.h"

typedef struct vertex {
  vec2 pos;
  vec3 color;
} vertex;

typedef struct file_info {
  size_t size;
  char *content;
} file_info;

typedef struct vertices {
  vertex *list;
  u32 size;
} vertices;

#endif
