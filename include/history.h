#ifndef HISTORY_H
#define HISTORY_H

#include "common.h"

typedef struct {
    char command[MAXLINE];
} histroy_t;

extern histroy_t histroy[MAXHISTORY];

#endif