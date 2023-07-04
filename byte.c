#include "code.h"
#include "defines.h"
#include "io.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void code(Code *c) { *c = code_init(); }
