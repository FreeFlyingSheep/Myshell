#ifndef COMMON_H
#define COMMON_H

/* Comment out the next line to trun off debugging */
#define DEBUG

#ifndef DEBUG
#define NDEBUG // for <assert.h>
#endif

/* Misc manifest constants */
#define MAXLINE    1024 // max line size
#define MAXARGS     128 // max args on a command line
#define MAXHISTORY 1000 // max histroy
#define MAXNAME      32 // max length of username
#define MAXJOBS      16 // max jobs at any point in time
#define MAXJID    65536 // max job ID 

#define TRUE  1
#define FALSE 0

typedef int BOOL;

#endif