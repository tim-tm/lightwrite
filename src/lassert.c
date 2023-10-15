#include "lassert.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lassert_fail(char* expr, char* file, int line) {
    LOG_FATAL("Assertion failed: %s (%s:%i)", expr, file, line);
    exit(1);
}
