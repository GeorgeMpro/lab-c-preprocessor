#ifndef SYMBOL
#include "symbol_table.h"
#endif
#ifndef CONVERSION
#include "../conversion/conversion.h"
#endif
#ifndef SECOND_MOVE
#include "second_move.h"
#endif
#ifndef METHODS
#include "../methods/methods.h"
#endif

#define FIRST_MOVE

bool first_move(char *file_name);

void clean(WordsList *code_img, WordsList *data_img, Symbol *symbol_table, FILE *inputf);
