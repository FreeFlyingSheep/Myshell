#include <stdio.h>
#include <signal.h>

#include "handler.h"

handler_t *signal_handler(int signo, handler_t *handler)
{
    struct sigaction act, oact;

    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (sigaction(signo, &act, &oact) < 0)
        perror("Singal error");
    return (oact.sa_handler);
}

void sigtstp_handler(int sig)
{

}

void sigstop_handler(int sig)
{

}

void sigcont_handler(int sig)
{

}

void sigint_handler(int sig)
{

}