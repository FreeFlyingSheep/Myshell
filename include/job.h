#ifndef JOB_H
#define JOB_H

#include <sys/types.h>

#include "common.h"

/* Job states */
#define UNDEF 0 // undefined
#define FG    1 // running in foreground
#define BG    2 // running in background
#define ST    3 // stopped

typedef int jid_t;

typedef struct {
    jid_t jid;
    pid_t pid;
    int state;
    char command[MAXLINE];
} job_t;

extern job_t jobs[MAXJOBS];

#endif