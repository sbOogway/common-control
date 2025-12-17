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
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

static const char* default_format =
    "[%Y-%m-%d %H:%M:%S.%03ld] [%s] [%s:%d:%s] %s\n";

static void get_timestamp(char* buffer, size_t size)
{
  struct timeval tv;
  struct tm* tm_info;

  gettimeofday(&tv, NULL);
  tm_info = localtime(&tv.tv_sec);

  snprintf(buffer,
           size,
           "%04d-%02d-%02d %02d:%02d:%02d.%03ld",
           tm_info->tm_year + 1900,
           tm_info->tm_mon + 1,
           tm_info->tm_mday,
           tm_info->tm_hour,
           tm_info->tm_min,
           tm_info->tm_sec,
           tv.tv_usec / 1000);
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

  get_timestamp(timestamp, sizeof(timestamp));

  if (current_log_output & LOG_OUTPUT_CONSOLE) {
    fprintf(stderr, "[%s] [%s] %s\n", timestamp, level_strings[level], message);
  }

  if ((current_log_output & LOG_OUTPUT_FILE) && log_file && log_file != stderr)
  {
    fprintf(log_file,
            "[%s] [%s] [%s:%d:%s] %s\n",
            timestamp,
            level_strings[level],
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
