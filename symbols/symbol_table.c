#include "symbol_table.h"

Symbol *new_symbol(char *name, int value, short attribute, bool entry) {
    int base = 0, offset = 0;

    Symbol *tmp = NULL;
    tmp = (Symbol *) malloc(sizeof(Symbol));
    if (tmp != NULL) {
        strcpy(tmp->label_name, name);
        tmp->value = value;
        get_base_and_offset(value, &base, &offset);
        tmp->base_add = base;
        tmp->offset = offset;
        tmp->attribute = attribute;
        tmp->entry = entry;
        tmp->next = NULL;
    }

    return tmp;
}

Symbol *insert_symbol(Symbol *symbol_table, char *name, int value, short attribute, bool entry) {
    if (symbol_table == NULL) {
        return new_symbol(name, value, attribute, entry); /* create new symbol table */
    }

    if (symbol_table->next == NULL) {
        symbol_table->next = new_symbol(name, value, attribute, entry);
        return symbol_table;
    }

    insert_symbol(symbol_table->next, name, value, attribute, entry);
    return symbol_table;
}

void destroy_symbol_table(Symbol *symbol_table) {
    if (symbol_table == NULL) {
        return;
    }

    destroy_symbol_table(symbol_table->next);
    free(symbol_table);
}

Symbol *search_symbol(Symbol *symbol_table, char *name) {
    while (symbol_table != NULL) {
        if (strcmp(symbol_table->label_name, name) == 0) {
            return symbol_table;
        }
        symbol_table = symbol_table->next;
    }
    return NULL; /* symbol not exist */
}

Symbol *fix_symbol_table(Symbol *symbol_table, int ic) {
    int base = 0, offset = 0;

    while (symbol_table != NULL) {
        if (symbol_table->attribute == DATA) {
            symbol_table->value += ic;
            get_base_and_offset(symbol_table->value, &base, &offset);
            symbol_table->base_add = base;
            symbol_table->offset = offset;
        }
        symbol_table = symbol_table->next;
    }
    return symbol_table;
}

void print_symbols(Symbol *symbol_table) {
    while (symbol_table != NULL) {
        printf("%s	%d	%d	%d	%d	%d\n", symbol_table->label_name, symbol_table->value,
               symbol_table->base_add,
               symbol_table->offset, symbol_table->attribute, symbol_table->entry);
        symbol_table = symbol_table->next;
    }
}
