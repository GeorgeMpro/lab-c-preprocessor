#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef LINE_BUILDER

#include "../helpers/line_builders.h"

#endif

#define METHODS

#define AMOUNT_OF_METHODS (16)
#define MAX_LINE_LENGTH (80)

typedef struct Method {
    char *name;
    int name_size;
    int group_num;
    int opcode;
    int func;
    int src_addressing_method;
    int dest_addressing_method;
} Method;

typedef enum Addressing_Methods {
    NO_OPERAND = -1,
    IMMEDIATE = 0,
    DIRECT = 1,
    INDEX = 2,
    REG_DIRECT = 3
} Addressing_Methods;

typedef struct OpperandInfo {
    Addressing_Methods addressing_method;
    short reg_num;
    int additional_first_word;
    int additional_second_word;
    bool return_to_me;
} OpperandInfo;

Method *init_methods_list();

void destroy_methods_list(Method *methods_list);

int method_index(Method *methods_list, char *word);

bool is_legal_label(Method *command_list, char *label, int line_number);

bool is_valid_addressing(Method *method, Addressing_Methods operand_type, bool is_source);

bool check_index(char *str);

bool get_index_or_direct_addressing(char *operand, OpperandInfo *ret_info, int line_number);

bool get_addresing_method(char *operand, OpperandInfo *ret_info, int line_number);

bool
check_operands_number(Method *method, char origin_operand[MAX_LINE_LENGTH + 1], char dest_operand[MAX_LINE_LENGTH + 1]);

void clean_info(OpperandInfo *info);

void get_operand_labels(char *line, char *orig_op, char *dest_op, int line_number);
