#ifndef LOGGING_H
#define LOGGING_H

#include <stdarg.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Logging levels */
typedef enum {
  LOG_TRACE = 0,
  LOG_DEBUG = 1,
  LOG_INFO = 2,
  LOG_WARN = 3,
  LOG_ERROR = 4,
  LOG_FATAL = 5,
  LOG_OFF = 6
} log_level_t;

typedef enum {
  TIMESTAMP_PRECISION_MILLI = 3,
  TIMESTAMP_PRECISION_MICRO = 6,
  TIMESTAMP_PRECISION_NANO = 9
} timestamp_precision_t;

#define LOGGING_TIMER_PRECISION TIMESTAMP_PRECISION_NANO

/* Log output destinations */
typedef enum {
  LOG_OUTPUT_CONSOLE = 1 << 0,
  LOG_OUTPUT_FILE = 1 << 1,
  LOG_OUTPUT_SYSLOG = 1 << 2,
  LOG_OUTPUT_ALL = LOG_OUTPUT_CONSOLE | LOG_OUTPUT_FILE | LOG_OUTPUT_SYSLOG
} log_output_t;

/* Logging functions */
void log_set_level(log_level_t level);
void log_set_output(log_output_t output);
void log_set_file(const char* filename);
void log_set_format(const char* format);
int log_init(void);
void log_cleanup(void);
void log_write(log_level_t level,
               const char* file,
               int line,
               const char* func,
               const char* fmt,
               ...);

/* Logging macros */
#define LOG_TRACE(fmt, ...) \
  log_write(LOG_TRACE, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) \
  log_write(LOG_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) \
  log_write(LOG_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) \
  log_write(LOG_WARN, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) \
  log_write(LOG_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) \
  log_write(LOG_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* LOGGING_H */
