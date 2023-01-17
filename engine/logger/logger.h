#ifndef _GROEI_LOGGER_H_
#define _GROEI_LOGGER_H_
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void GROEI_INFO(const char *format, ...);
void GROEI_DEBUG(const char *format, ...);
void GROEI_WARN(const char *format, ...);
void GROEI_ERROR(const char *format, ...);

#endif
