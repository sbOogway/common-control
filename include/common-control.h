#ifndef COMMON_CONTROL_H
#  define COMMON_CONTROL_H

#  include <sys/types.h>

#  include "logging.h"

#  ifdef __cplusplus
extern "C" {
#  endif

/* Existing functions */
int get_int_from_file(const char* file);
int get_pid_from_file(const char* pid_file);
float get_float_from_file(const char* file);
void write_float_to_file(const char* file, float value);
void write_char_to_file(const char* file, char value);
void write_pid_to_file(const char* pid_file);
int sign(int value);


#  ifdef __cplusplus
}
#  endif

#endif /* COMMON_CONTROL_H */

#define ROOT_DIR "/opt/amel"

#define PID_SUFFIX "-pid"

#define TEMP_CONTROL_PROCESS_NAME "temp-control"
#define PID_CONTROL_PROCESS_NAME  "pid-control"

#define CURRENT_TEMPERATURE_FILE ROOT_DIR "/current-temperature"
#define TARGET_TEMPERATURE_FILE  ROOT_DIR "/target-temperature"
#define TEMP_CONTROL_PID_FILE    ROOT_DIR "/" TEMP_CONTROL_PROCESS_NAME PID_SUFFIX
#define PID_CONTROL_PID_FILE     ROOT_DIR "/" PID_CONTROL_PROCESS_NAME  PID_SUFFIX
#define NUMBER_OF_SENSORS_FILE   ROOT_DIR "/number-sensors"


#define SAMPLE_RATE 1  // in hertz
#define SERIAL_PORT "/dev/ttyUSB0"