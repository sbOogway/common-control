#define ROOT_DIR "/opt/amel"

#define PID_SUFFIX "-pid"

#define TEMP_CONTROL_PROCESS_NAME "temp-control"
#define PID_CONTROL_PROCESS_NAME  "pid-control"

#define CURRENT_TEMPERATURE "current-temperature"
#define TARGET_TEMPERATURE "target-temperature"

#define NUMBER_OF_SENSORS "number-sensors"

#define CURRENT_TEMPERATURE_FILE ROOT_DIR "/" CURRENT_TEMPERATURE
#define TARGET_TEMPERATURE_FILE  ROOT_DIR "/" TARGET_TEMPERATURE
#define TEMP_CONTROL_PID_FILE    ROOT_DIR "/" TEMP_CONTROL_PROCESS_NAME PID_SUFFIX
#define PID_CONTROL_PID_FILE     ROOT_DIR "/" PID_CONTROL_PROCESS_NAME  PID_SUFFIX
#define NUMBER_OF_SENSORS_FILE   ROOT_DIR "/" NUMBER_OF_SENSORS


#define MIN_OUTPUT 0.0
#define MAX_OUTPUT 10000.0 // maximum value for the inverter is 20000

#define KP 1.0
#define KI 1.0
#define KD 0.0

#define DEFAULT_TARGET_TEMPERATURE 18.00


#define SAMPLE_RATE 0.2 // in hertz
#define SAMPLE_TIME  1.0/SAMPLE_RATE


#define SERIAL_PORT_INVERTER "/dev/ttyUSB0"
#define SERIAL_PORT_SENSORS  "/dev/ttyUSB1"