#ifndef SCHEDULE_H
#define SCHEDULE_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

#define MAX_PROCESSES 100
#define MAX_ARGUEMENTS 10

void alarm_handler(int sig);

#endif
