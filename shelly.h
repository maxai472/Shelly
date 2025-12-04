#ifndef SHELLY_H
#define SHELLY_H

/*** includes ***/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

/*** definitions ***/
#define SHELLY_RL_BUFSIZE 1024
#define SHELLY_TOKEN_BUFSIZE 64
#define SHELLY_TOKEN_DELIMS " \t\r\n\a"

/*** macros ***/
/*** terminal escape sequences for changing colours ***/
#define RESET_COLOUR       "\x1b[0m"

#define BLACK       "\x1b[30m"
#define RED         "\x1b[31m"
#define GREEN       "\x1b[32m"
#define YELLOW      "\x1b[33m"
#define BLUE        "\x1b[34m"
#define MAGENTA     "\x1b[35m"
#define CYAN        "\x1b[36m"
#define WHITE       "\x1b[37m"


#endif
