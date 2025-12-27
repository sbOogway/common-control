#ifndef COMMON_CONTROL_H
#define COMMON_CONTROL_H

#include <sys/types.h>

#include "logging.h"
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Existing functions */
int get_int_from_file(const char* file);
int get_pid_from_file(const char* pid_file);
float get_float_from_file(const char* file);
void write_float_to_file(const char* file, float value);
void write_char_to_file(const char* file, char value);
void write_pid_to_file(const char* pid_file);
int sign(int value);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_CONTROL_H */
