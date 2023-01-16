#include <stdlib.h>
#include <stdio.h>
#include "system.h"
#include "../core/types.h"

void read_file(const char *filename, file_info *file) {
  FILE *f;

  f = fopen(filename, "rb");
  if (f == NULL) {
    printf("Failed to open %s\n", filename);
    exit(GROEI_ERROR_LOADING_FILE_CODE);
  }

  fseek(f, 0L, SEEK_END);
  file->size = ftell(f);

  fseek(f, 0L, SEEK_SET);

  file->content = (char*)malloc(sizeof(char) * file->size);
  size_t readCount = fread(file->content, file->size, sizeof(char), f);
  printf("ReadCount: %ld\n", readCount);

  fclose(f);
}
