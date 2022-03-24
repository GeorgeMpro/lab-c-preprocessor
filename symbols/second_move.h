#ifndef SYMBOL

#include "symbol_table.h"

#endif
#ifndef METHODS

#include "../methods/methods.h"

#endif
#ifndef CONVERSION

#include "../conversion/conversion.h"

#endif
#define SECOND_MOVE

bool write_to_externals_file(Symbol *symbol, char *file_name, bool is_first_extern, int word_num);

bool write_to_entries_file(Symbol *symbol, char *file_name, bool is_first_entry);

bool second_move(FILE *inputf, Symbol *symbol_table, char *file_name, WordsToReturnToList *returnTo);
