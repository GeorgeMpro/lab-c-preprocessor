#include <stdlib.h>
#include <stdio.h>
#include "macros_table.h"

#ifndef HELPERS
#include "../helpers/helpers.h"
#endif

#define PREPROCESSOR

bool span_macros(char *file_name);

bool is_macro_def(char *line, char *name);

void get_macro_text(char *macro_text, FILE *inputf);

Macro *is_macro_call(Macro *m, char *line);
