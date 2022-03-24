
#ifndef METHODS

/*//todo change*/
#include "../methods/methods.h"

#endif
#ifndef LINE_BUILDER
#include "../helpers/line_builders.h"
#endif

#define CONVERSION

#define MAX_INTEGER (2048)
#define MIN_INTEGER (-2047)

bool is_proper_num_in_command(char *str, int *value);

bool is_proper_string_format(char *line);

int
conv_method(char *line, char *method, bool is_label_first, Method *methods_list, int line_number, WordsList *words_list,
            WordsToReturnToList *returnTo);

int conv_command(char *line, int command_kind, int line_number, WordsList *words_list);

bool add_additional_words(OpperandInfo *info, WordsList *words_list, WordsToReturnToList *returnTo, int line_number,
                          int *words_added_count, bool *return_to_word_added);

bool handle_operands_info(Method *method, OpperandInfo *orig_info, OpperandInfo *dest_info, WordsList *words_list,
                          WordsToReturnToList *returnTo, int line_number, int *words_added_count);
