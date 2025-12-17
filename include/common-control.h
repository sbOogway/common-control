/* source/common-control.c */
int get_pid_from_file(const char* pid_file);
void write_float_to_file(const char* file, float value);
void write_char_to_file(const char* file, char value);
void write_pid_to_file(const char* pid_file);

#define CURRENT_TEMPERATURE_FILE    "/opt/amel/current-temperature"
#define TARGET_TEMPERATURE_FILE     "/opt/amel/target-temperature"
#define TEMP_CONTROL_PID_FILE       "/opt/amel/temp-control-pid"
#define PID_CONTROL_PID_FILE        "/opt/amel/pid-control-pid"
#define NUMBER_OF_SENSORS_FILE      "/opt/amel/number-sensors"
