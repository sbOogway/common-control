#include "control-common/control-common.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

char const* exported_function(void)
{
  return "control-common";
}

void write_pid_to_file(const char * pid_file)
{
    FILE * pid_fd = fopen(pid_file, "w");
    if(!pid_fd) {
        fprintf(stderr, "Failed to open PID file %s for writing\n", pid_file);
        return;
    }
    fprintf(pid_fd, "%d\n", getpid());
    fclose(pid_fd);
}

static int get_pid_from_file_single(const char * pid_file)
{
    FILE * pid_fd = fopen(pid_file, "r");
    if(!pid_fd) {
        fprintf(stderr, "Failed to open PID file %s for reading\n", pid_file);
        return -1;
    }
    char buf[16];
    read(fileno(pid_fd), buf, sizeof(buf)-1);
    close(fileno(pid_fd));
    return atoi(buf);
}


pid_t get_pid_from_file(const char * pid_file)
{
    while (get_pid_from_file_single(pid_file) == -1) {
        fprintf(stdout, "Waiting for pid file %s to be created...\n", pid_file);
        sleep(1);
    }
    return get_pid_from_file_single(pid_file);
}