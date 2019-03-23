#ifndef HANDLER_H
#define HANDLER_H

#include "common.h"

typedef void handler_t(int);

handler_t *signal_handler(int signo, handler_t *handler);

void sigtstp_handler(int sig);
void sigstop_handler(int sig);
void sigcont_handler(int sig);
void sigint_handler(int sig);

#endif