#ifndef MIM_ERR_H
#define MIM_ERR_H

#include <stdbool.h>
#include <stdnoreturn.h>

#include "utils.h"

#define MAX_ERR_MSG_LEN 10

// Print information about a system error and quits.
noreturn void syserr(const char* fmt, ...);

// Print information about an error and quits.
noreturn void fatal(const char* fmt, ...);

// Print information about an error and return.
void error(const char* fmt, ...);

// Print verbose messages if verbose mode is enabled.
void info(bool verbose, const char* fmt, ...);

// Print error message
void error_msg(data* mes);

#endif