#include "logging.h"

#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

/* Logging implementation */
static log_level_t current_log_level = LOG_INFO;
static log_output_t current_log_output = LOG_OUTPUT_CONSOLE;
static char* log_filename = NULL;
static char* log_format = NULL;
static FILE* log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static const char* level_strings[] = {
    "TRACE", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL"};

static const char* default_format =
    "[%Y-%m-%d %H:%M:%S.%03ld] [%s] [%s:%d:%s] %s\n";

static const char* get_process_name(void)
{
  static char process_name[64] = {0};

  if (process_name[0] == '\0') {
    FILE* f = fopen("/proc/self/comm", "r");
    if (f) {
      if (fgets(process_name, sizeof(process_name), f)) {
        char* newline = strchr(process_name, '\n');
        if (newline) {
          *newline = '\0';
        }
      }
      fclose(f);
    } else {
      strncpy(process_name, "unknown", sizeof(process_name) - 1);
    }
    process_name[8] = '\0';
  }

  return process_name;
}

static void get_timestamp(char* buffer,
                          size_t size,
                          timestamp_precision_t precision)
{
  struct timespec ts;
  struct tm* tm_info;

  clock_gettime(CLOCK_REALTIME, &ts);
  tm_info = localtime(&ts.tv_sec);

  const char* format;
  long precision_value;

  switch (precision) {
    case TIMESTAMP_PRECISION_MILLI:
      format = "%04d-%02d-%02d %02d:%02d:%02d.%03ld";
      precision_value = ts.tv_nsec / 1000000;
      break;
    case TIMESTAMP_PRECISION_MICRO:
      format = "%04d-%02d-%02d %02d:%02d:%02d.%06ld";
      precision_value = ts.tv_nsec / 1000;
      break;
    case TIMESTAMP_PRECISION_NANO:
    default:
      format = "%04d-%02d-%02d %02d:%02d:%02d.%09ld";
      precision_value = ts.tv_nsec;
      break;
  }

  snprintf(buffer,
           size,
           format,
           tm_info->tm_year + 1900,
           tm_info->tm_mon + 1,
           tm_info->tm_mday,
           tm_info->tm_hour,
           tm_info->tm_min,
           tm_info->tm_sec,
           precision_value);
}

void log_set_level(log_level_t level)
{
  pthread_mutex_lock(&log_mutex);
  current_log_level = level;
  pthread_mutex_unlock(&log_mutex);
}

void log_set_output(log_output_t output)
{
  pthread_mutex_lock(&log_mutex);
  current_log_output = output;
  pthread_mutex_unlock(&log_mutex);
}

void log_set_file(const char* filename)
{
  pthread_mutex_lock(&log_mutex);

  if (log_file && log_file != stderr) {
    fclose(log_file);
    log_file = NULL;
  }

  if (log_filename) {
    free(log_filename);
    log_filename = NULL;
  }

  if (filename) {
    log_filename = strdup(filename);
    log_file = fopen(filename, "a");
    if (!log_file) {
      fprintf(stderr,
              "Failed to open log file %s: %s\n",
              filename,
              strerror(errno));
      log_file = stderr;
    }
  }

  pthread_mutex_unlock(&log_mutex);
}

void log_set_format(const char* format)
{
  pthread_mutex_lock(&log_mutex);

  if (log_format) {
    free(log_format);
    log_format = NULL;
  }

  if (format) {
    log_format = strdup(format);
  }

  pthread_mutex_unlock(&log_mutex);
}

int log_init(void)
{
  pthread_mutex_lock(&log_mutex);

  if (!log_format) {
    log_format = strdup(default_format);
  }

  if (!log_file) {
    log_file = stderr;
  }

  pthread_mutex_unlock(&log_mutex);
  return 0;
}

void log_cleanup(void)
{
  pthread_mutex_lock(&log_mutex);

  if (log_file && log_file != stderr) {
    fclose(log_file);
    log_file = NULL;
  }

  if (log_filename) {
    free(log_filename);
    log_filename = NULL;
  }

  if (log_format) {
    free(log_format);
    log_format = NULL;
  }

  pthread_mutex_unlock(&log_mutex);
}

void log_write(log_level_t level,
               const char* file,
               int line,
               const char* func,
               const char* fmt,
               ...)
{
  if (level < current_log_level) {
    return;
  }

  pthread_mutex_lock(&log_mutex);

  va_list args;
  char message[1024];
  char timestamp[64];
  char log_entry[2048];

  va_start(args, fmt);
  vsnprintf(message, sizeof(message), fmt, args);
  va_end(args);

  get_timestamp(timestamp, sizeof(timestamp), LOGGING_TIMER_PRECISION);

  if (current_log_output & LOG_OUTPUT_CONSOLE) {
    fprintf(stderr,
            "[%s] [%s] [%s] %s\n",
            timestamp,
            level_strings[level],
            get_process_name(),
            message);
  }

  if ((current_log_output & LOG_OUTPUT_FILE) && log_file && log_file != stderr)
  {
    fprintf(log_file,
            "[%s] [%s] [%s] [%s:%d:%s] %s\n",
            timestamp,
            level_strings[level],
            get_process_name(),
            file,
            line,
            func,
            message);
    fflush(log_file);
  }

  pthread_mutex_unlock(&log_mutex);

  if (level >= LOG_FATAL) {
    exit(EXIT_FAILURE);
  }
}
