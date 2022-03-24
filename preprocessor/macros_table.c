#include "macros_table.h"

/* create new Macro */
Macro *new_macro(char *name, char *text) {
    Macro *tmp = NULL;
    tmp = (Macro *) malloc(sizeof(Macro));
    if (tmp != NULL) {
        strcpy(tmp->macro_name, name);
        strcpy(tmp->macro_text, text);
        tmp->next_macro = NULL;
    }

    return tmp;
}

/* insert Macro to the macros table */
Macro *insert_macro(Macro *m, char *name, char *text) {
    if (m == NULL) {
        return new_macro(name, text);
    }

    if (m->next_macro == NULL) {
        m->next_macro = new_macro(name, text);
        return m;
    }

    insert_macro(m->next_macro, name, text);
    return m;
}

/* return the Macro with the name "str" or null if Macro "str" is not exist in the macros table */
Macro *is_macro_name(Macro *m, char *str) {
    if (m == NULL) {
        return NULL;
    }

    if (strcmp(m->macro_name, str) == 0) {
        return m;
    } else {
        return is_macro_name(m->next_macro, str);
    }
}

/* delete macros table */
/*todo cleanup/shutdown*/
void destroy_macro_table(Macro *m) {
    if (m == NULL) {
        return;
    }

    destroy_macro_table(m->next_macro);
    free(m);
}
