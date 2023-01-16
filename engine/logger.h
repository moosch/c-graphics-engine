#ifndef _GROEI_LOGGER_H_
#define _GROEI_LOGGER_H_
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// TODO(moosch): change behavious based on build type (debug/profile/prod etc)

void GROEI_INFO(const char *format, ...) {
  printf("GROEI_INFO: ");
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
void GROEI_DEBUG(const char *format, ...) {
  printf("GROEI_DEBUG: ");
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
void GROEI_WARN(const char *format, ...) {
  printf("GROEI_WARN: ");
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}
void GROEI_ERROR(const char *format, ...) {
  printf("GROEI_ERROR: ");
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

#endif
