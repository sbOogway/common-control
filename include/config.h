#define ROOT_DIR "/opt/amel"

#define PID_SUFFIX "-pid"

#define TEMP_CONTROL_PROCESS_NAME "temp-control"
#define PID_CONTROL_PROCESS_NAME  "pid-control"

#define CURRENT_TEMPERATURE "current-temperature"
#define TARGET_TEMPERATURE "target-temperature"

#define NUMBER_OF_SENSORS "number-sensors"

#define LOG_DIR "logs"
// %s is a placeholder for s0,s1 
#define LOG_CSV_HEADER "timestamp,%starget_temperature,pid_output\n"

#define CURRENT_TEMPERATURE_FILE ROOT_DIR "/" CURRENT_TEMPERATURE
#define TARGET_TEMPERATURE_FILE  ROOT_DIR "/" TARGET_TEMPERATURE
#define TEMP_CONTROL_PID_FILE    ROOT_DIR "/" TEMP_CONTROL_PROCESS_NAME PID_SUFFIX
#define PID_CONTROL_PID_FILE     ROOT_DIR "/" PID_CONTROL_PROCESS_NAME  PID_SUFFIX
#define NUMBER_OF_SENSORS_FILE   ROOT_DIR "/" NUMBER_OF_SENSORS
#define LOG_CSV_DIR              ROOT_DIR "/" LOG_DIR "/"

#define DEFAULT_TARGET_TEMPERATURE 18.00
