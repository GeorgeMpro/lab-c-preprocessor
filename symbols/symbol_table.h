#ifndef LINE_BUILDER

#include "../helpers/line_builders.h"

#endif

#define CODE (1)
#define DATA (2)
#define EXTERNAL (3)
#define SYMBOL
#define MAX_LABEL_LENGTH (31)

typedef struct Symbol {
    short attribute; /* CODE, DATA or EXTERNAL */
    int value;
    int base_add;
    int offset;
    bool entry;
    char label_name[MAX_LABEL_LENGTH];
    struct Symbol *next;

} Symbol;

Symbol *new_symbol(char *label_name, int value, short attribute, bool entry);

Symbol *insert_symbol(Symbol *symbol_table, char *name, int value, short attribute, bool entry);

void destroy_symbol_table(Symbol *symbol_table);

Symbol *fix_symbol_table(Symbol *symbol_table, int ic);

void print_symbols(Symbol *symbol_table);

Symbol *search_symbol(Symbol *symbol_table, char *name);
