#ifndef USER_H
#define USER_H

#include "common.h"

typedef struct {
    char name[MAXNAME];
} user_t;

extern user_t user;

#endif