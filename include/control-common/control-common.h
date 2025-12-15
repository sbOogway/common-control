/* source/control-common.c */
char const* exported_function(void);
void write_pid_to_file(const char* pid_file);
int get_pid_from_file(const char* pid_file);
void write_float_to_file(char* file, float value);

#define CURRENT_TEMPERATURE_FILE "/opt/amel/current-temperature"
#define TARGET_TEMPERATURE_FILE "/opt/amel/target-temperature"
#define TEMP_CONTROL_PID_FILE "/opt/amel/temp-control-pid"
#define PID_CONTROL_PID_FILE "/opt/amel/pid-control-pid"
