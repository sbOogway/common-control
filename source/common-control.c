#include "common-control.h"

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int write_value_to_file(const char* filename, const char* format, ...)
{
  if (!filename || !format) {
    LOG_ERROR("Error: NULL parameters passed to write_value_to_file");
    return -1;
  }

  FILE* fd = fopen(filename, "w");
  if (!fd) {
    LOG_ERROR(
        "Failed to open file %s for writing: %s", filename, strerror(errno));
    return -1;
  }

  va_list args;
  va_start(args, format);
  int result = vfprintf(fd, format, args);
  va_end(args);

  if (result < 0) {
    LOG_ERROR("Error writing to file %s: %s", filename, strerror(errno));
    fclose(fd);
    return -1;
  }

  if (fclose(fd) != 0) {
    LOG_ERROR("Error closing file %s: %s", filename, strerror(errno));
    return -1;
  }

  return 0;
}

static int get_pid_from_file_single(const char* pid_file)
{
  FILE* pid_fd = fopen(pid_file, "r");
  if (!pid_fd) {
    LOG_ERROR("Failed to open PID file %s for reading", pid_file);
    return -1;
  }
  char buf[16];
  read(fileno(pid_fd), buf, sizeof(buf) - 1);
  close(fileno(pid_fd));
  return atoi(buf);
}

pid_t get_pid_from_file(const char* pid_file)
{
  while (get_pid_from_file_single(pid_file) == -1) {
    LOG_INFO("Waiting for pid file %s to be created...", pid_file);
    sleep(1);
  }
  return get_pid_from_file_single(pid_file);
}

void write_pid_to_file(const char* pid_file)
{
  if (write_value_to_file(pid_file, "%d\n", getpid()) != 0) {
    LOG_ERROR("Failed to write PID to file %s", pid_file);
  }
}

void write_float_to_file(const char* file, float value)
{
  if (write_value_to_file(file, "%.2f\n", value) != 0) {
    LOG_ERROR("Failed to write float to file %s", file);
  }
}

void write_char_to_file(const char* file, char value)
{
  if (write_value_to_file(file, "%d\n", value) != 0) {
    LOG_ERROR("Failed to write char to file %s", file);
  }
}

int sign(int value)
{
  if (value >= 0) {
    return 1;
  }
  return -1;
}

static char* read_value_from_file(const char* file,
                                  char* buffer,
                                  size_t buffer_size)
{
  if (!file || !buffer) {
    return NULL;
  }

  FILE* fd = fopen(file, "r");
  if (!fd) {
    LOG_ERROR("Failed to open file %s for reading: %s", file, strerror(errno));
    return NULL;
  }

  if (fgets(buffer, buffer_size, fd) == NULL) {
    LOG_ERROR("Error reading from file %s", file);
    fclose(fd);
    return NULL;
  }

  fclose(fd);
  return buffer;
}

int get_int_from_file(const char* file)
{
  if (!file) {
    LOG_ERROR("Error: NULL file parameter passed to get_int_from_file");
    return 0;
  }

  char buf[8];
  if (read_value_from_file(file, buf, sizeof(buf)) == NULL) {
    return 0;
  }

  return atoi(buf);
}

float get_float_from_file(const char* file)
{
  if (!file) {
    LOG_ERROR("Error: NULL file parameter passed to get_float_from_file");
    return 0.0f;
  }

  char buf[8];
  if (read_value_from_file(file, buf, sizeof(buf)) == NULL) {
    return 0.0f;
  }

  return atof(buf);
}
