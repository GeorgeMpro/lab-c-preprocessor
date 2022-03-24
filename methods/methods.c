#include "methods.h"

#define OPCODE(val) (1 << (val))

Method *init_methods_list() {
    int i = 0;
    Method *commands_list;
    Method *current;

    char *commands_names[AMOUNT_OF_METHODS] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp",
                                               "bne", "jsr", "red", "prn", "rts", "stop"};
    int group_num[AMOUNT_OF_METHODS] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3};
    int opcodes[AMOUNT_OF_METHODS] = {OPCODE(0), OPCODE(1), OPCODE(2), OPCODE(2), OPCODE(4), OPCODE(5), OPCODE(5),
                                      OPCODE(5), OPCODE(5), OPCODE(9), OPCODE(9), OPCODE(9), OPCODE(12), OPCODE(13),
                                      OPCODE(14), OPCODE(15)};
    int funcs[AMOUNT_OF_METHODS] = {0, 0, 10, 11, 0, 10, 11, 12, 13, 10, 11, 12, 0, 0, 0, 0};
    int src_addressing_methods[AMOUNT_OF_METHODS] = {15, 15, 15, 15, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int dest_addressing_methods[AMOUNT_OF_METHODS] = {14, 15, 14, 14, 14, 14, 14, 14, 14, 6, 6, 6, 14, 15, -1, -1};

    commands_list = (Method *) calloc(sizeof(Method), AMOUNT_OF_METHODS);
    if (commands_list != NULL) {
        for (; i < AMOUNT_OF_METHODS; i++) {
            current = (commands_list + i);

            current->name_size = strlen(commands_names[i]);
            current->name = (char *) malloc(strlen(commands_names[i]));
            if (current->name != NULL) {

                strcpy(current->name, commands_names[i]);
            }
            current->group_num = group_num[i];
            current->opcode = opcodes[i];
            current->func = funcs[i];
            current->src_addressing_method = src_addressing_methods[i];
            current->dest_addressing_method = dest_addressing_methods[i];
        }
    }

    return commands_list;
}

void destroy_methods_list(Method *commands_list) {
    int i = 0;

    for (; i < AMOUNT_OF_METHODS; i++) {
        if ((commands_list + i) != NULL) {
            if ((commands_list + i)->name != NULL) {
                free((commands_list + i)->name);
            }
        }
    }
    free(commands_list);
}

int method_index(Method *command_list, char *word) {
    Method current;
    char last_char;
    int i = 0;

    if (command_list == NULL) {
        printf("method list is null, returning -1");
        return -1;
    }
    for (; i < AMOUNT_OF_METHODS; i++) {
        current = command_list[i];
        if (memcmp(word, current.name, current.name_size) == 0) {
            last_char = *(word + current.name_size);
            if (isspace(last_char) || last_char == '\0') {
                return i;
            }
        }
    }
    return -1;
}

bool is_legal_label(Method *command_list, char *label, int line_number) {
    int i = 0;

    while (label[i] != '\0') {
        if (isalnum(label[i]) == 0) {
            printf("Line %d- Error: Label name has only alpha-numeric characters\n", line_number);
            return false;
        }

        i++;
    }

    if (method_index(command_list, label) > 0) {
        printf("Line %d- Error: Label name cannot be the same as method name\n", line_number);
        return false;
    }

    if (get_reg_number(label, &i, line_number)) {
        printf("Line %d- Error: Label name cannot be the same as register name\n", line_number);
        return false;
    }

    return true;
}

bool is_valid_addressing(Method *method, Addressing_Methods operand_type, bool is_source) {
    int bit_wise_addressing = 1 << operand_type;

    if (operand_type == NO_OPERAND)
        return true;

    return (bool) (is_source ? (method->src_addressing_method & bit_wise_addressing) : (method->dest_addressing_method &
                                                                                        bit_wise_addressing));
}

bool check_index(char *str) {
    int i = 0;

    if (str[0] != 'r')
        return false;

    for (; str[i] != ']'; i++) {
        if (str[i] == '\0')
            return false;
    }
    if (i != 3)
        return false;
    if (str[i + 1] != '\0')
        return false;

    return true;
}

bool get_index_or_direct_addressing(char *operand, OpperandInfo *ret_info, int line_number) {
    int braces_index = 0, reg_number = -1;
    char reg[4];
    char *temp = operand;

    while (*(temp)) {
        if (*(temp) == '[')
            break;
        temp++;
        braces_index++;
    }

    if ((*temp)) {
        if (!check_index(temp + 1)) {
            printf("Line %d- Error: Index format is unvalid\n", line_number);
            return false;
        }

        memcpy(reg, temp + 1, 3);
        reg[3] = '\0';

        if (get_reg_number(reg, &reg_number, line_number)) {
            if (10 <= reg_number || reg_number <= 15) {
                ret_info->reg_num = reg_number;
                ret_info->addressing_method = INDEX;
                ret_info->additional_first_word = 0;
                ret_info->return_to_me = true;
                return true;
            }
        }
        return false;
    } else {
        ret_info->addressing_method = DIRECT;
        ret_info->additional_first_word = 0;
        ret_info->return_to_me = true;
    }
    return true;
}

bool get_addresing_method(char *operand, OpperandInfo *ret_info, int line_number) {
    signed int reg_number = -1;
    int temp;
    if (is_empty(operand)) {
        ret_info->addressing_method = NO_OPERAND;
        ret_info->return_to_me = false;
        return true;
    }

    if (get_reg_number(operand, (signed int *) &reg_number, line_number)) {
        ret_info->reg_num = reg_number;
        ret_info->addressing_method = REG_DIRECT;
        ret_info->return_to_me = false;
        return true;
    }

    if (*operand == '#') {
        if (get_number_from_string(operand + 1, (signed int *) (&temp), true)) {
            ret_info->additional_first_word = temp;
            ret_info->addressing_method = IMMEDIATE;
            ret_info->return_to_me = false;
            temp = 0;
            return true;
        }
    }

    if (get_index_or_direct_addressing(operand, ret_info, line_number)) {
        return true;
    }

    return false;
}

bool check_operands_number(Method *method, char origin_operand[MAX_LINE_LENGTH + 1],
                           char dest_operand[MAX_LINE_LENGTH + 1]) {
    bool noErrors = true;

    if (is_empty(origin_operand))
        noErrors &= (bool) (method->src_addressing_method == -1);
    else
        noErrors &= (bool) (method->src_addressing_method != -1);

    if (is_empty(dest_operand))
        noErrors &= (bool) (method->dest_addressing_method == -1);
    else
        noErrors &= (bool) (method->dest_addressing_method != -1);

    return noErrors;
}

/* change orig_op and dest_op to string of the operand if its label or empty string if not */
void get_operand_labels(char *line, char *orig_op, char *dest_op, int line_number) {
    int i = 0;
    bool is_label_first;
    char label_name[MAX_LABEL_LENGTH];
    char temp_orig[MAX_LINE_LENGTH], temp_dest[MAX_LINE_LENGTH];
    OpperandInfo *orig_info = (OpperandInfo *) malloc(sizeof(OpperandInfo));
    OpperandInfo *dest_info = (OpperandInfo *) malloc(sizeof(OpperandInfo));
    clean_info(orig_info);
    clean_info(dest_info);

    is_label_first = is_label_def(line, label_name, line_number);

    split_operands(line, is_label_first, temp_orig, temp_dest, line_number);
    strcpy(orig_op, trim(temp_orig));
    strcpy(dest_op, trim(temp_dest));

    get_addresing_method(orig_op, orig_info, line_number);
    get_addresing_method(dest_op, dest_info, line_number);

    if (!(orig_info->addressing_method == DIRECT || orig_info->addressing_method == INDEX))
        orig_op[0] = '\0';

    if (!(dest_info->addressing_method == DIRECT || dest_info->addressing_method == INDEX))
        dest_op[0] = '\0';

    if (orig_info->addressing_method == INDEX) {
        for (; orig_op[i] != '['; i++);
        orig_op[i] = '\0';
    }

    if (dest_info->addressing_method == INDEX) {
        for (i = 0; dest_op[i] != '['; i++);
        dest_op[i] = '\0';
    }

    free(orig_info);
    free(dest_info);
}

void clean_info(OpperandInfo *info) {
    info->return_to_me = false;
    info->reg_num = 0;
    info->addressing_method = 0;
    info->additional_first_word = 0;
    info->additional_second_word = 0;
}
