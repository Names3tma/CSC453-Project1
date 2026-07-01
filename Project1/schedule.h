#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

#define MAX_PROCESSES 100
#define MAX_ARGUEMENTS 10

void alarm_handler(int sig);

#endif
