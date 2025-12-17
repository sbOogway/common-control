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
    fprintf(stderr, "Error: NULL parameters passed to write_value_to_file\n");
    return -1;
  }

  FILE* fd = fopen(filename, "w");
  if (!fd) {
    fprintf(stderr,
            "Failed to open file %s for writing: %s\n",
            filename,
            strerror(errno));
    return -1;
  }

  va_list args;
  va_start(args, format);
  int result = vfprintf(fd, format, args);
  va_end(args);

  if (result < 0) {
    fprintf(
        stderr, "Error writing to file %s: %s\n", filename, strerror(errno));
    fclose(fd);
    return -1;
  }

  if (fclose(fd) != 0) {
    fprintf(stderr, "Error closing file %s: %s\n", filename, strerror(errno));
    return -1;
  }

  return 0;
}

static int get_pid_from_file_single(const char* pid_file)
{
  FILE* pid_fd = fopen(pid_file, "r");
  if (!pid_fd) {
    fprintf(stderr, "Failed to open PID file %s for reading\n", pid_file);
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
    fprintf(stdout, "Waiting for pid file %s to be created...\n", pid_file);
    sleep(1);
  }
  return get_pid_from_file_single(pid_file);
}

void write_pid_to_file(const char* pid_file)
{
  if (write_value_to_file(pid_file, "%d\n", getpid()) != 0) {
    fprintf(stderr, "Failed to write PID to file %s\n", pid_file);
  }
}

void write_float_to_file(const char* file, float value)
{
  if (write_value_to_file(file, "%.2f\n", value) != 0) {
    fprintf(stderr, "Failed to write float to file %s\n", file);
  }
}

void write_char_to_file(const char* file, char value)
{
  if (write_value_to_file(file, "%d\n", value) != 0) {
    fprintf(stderr, "Failed to write char to file %s\n", file);
  }
}

int sign(int value)
{
  if (value >= 0) {
    return 1;
  }
  return -1;
}
