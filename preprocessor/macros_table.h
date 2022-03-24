#ifndef HELPERS

#include "../helpers/helpers.h"

#endif

typedef struct Macro {
    char macro_name[MAX_MACRO_NAME_LENGTH];
    char macro_text[6 * MAX_LINE_LENGTH];
    struct Macro *next_macro;

} Macro;

Macro *new_macro(char *name, char *text);

Macro *insert_macro(Macro *m, char *name, char *text);

Macro *is_macro_name(Macro *m, char *str);

void destroy_macro_table(Macro *m);
