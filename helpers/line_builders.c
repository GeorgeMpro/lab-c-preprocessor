#include "line_builders.h"

int get_ARE(Word *word) {
    int value = 0;
    if (word->A) {
        value |= 1 << A_LOCATION;
    }
    if (word->R) {
        value |= 1 << R_LOCATION;
    }
    if (word->E) {
        value |= 1 << E_LOCATION;
    }
    return value;
}

int convert_func_word_to_int(Word *word) {
    int return_value = 0;
    return_value |= (word->func & HEX_1111) << FUNC_LOCATION;
    return_value |= (word->orig_reg & HEX_1111) << ORIG_REG_LOCATION;
    return_value |= (word->orig_addressing & HEX_11) << ORIG_ADD_LOCATION;
    return_value |= (word->dest_reg & HEX_1111) << DEST_REG_LOCATION;
    return_value |= (word->dest_addressing & HEX_11) << DEST_ADD_LOCATION;
    return return_value;
}

void convert_int_to_hex_line(int line_value, char hex_value[HEX_STRING_LENGTH]) {
    int i = GORUPS_COUNT;
    char groups[GORUPS_COUNT] = {'A', 'B', 'C', 'D', 'E'};
    int groups_values[GORUPS_COUNT] = {0, 0, 0, 0, 0};
    /* For each group, get the group value */
    for (; 0 < i; i--) {
        groups_values[i - 1] = (line_value >> (4 * (GORUPS_COUNT - i))) & HEX_1111;
    }
    i = 0;
    for (; i < GORUPS_COUNT; i++) {
        hex_value[3 * i] = groups[i];
        sprintf(hex_value + (3 * i) + 1, "%x", groups_values[i]);
        if (i < (GORUPS_COUNT - 1))
            hex_value[(3 * i) + 2] = HEX_STRING_SEP;
    }
    hex_value[HEX_STRING_LENGTH - 1] = '\0';
}

void convert_words_to_hex_line(Word *word, char hex_value[HEX_STRING_LENGTH]) {
    int word_value = 0;
    word_value |= get_ARE(word);
    /* convert the func, dest and orig reg and addressing */
    if (word->is_func_word) {
        word_value |= convert_func_word_to_int(word);
    }
        /* convert data / opcode word */
    else {
        word_value |= (word->opcode & ALL_OPCODE_BITS_ON);
    }

    convert_int_to_hex_line(word_value, hex_value);
}

void write_word_to_file(FILE *output, Word *word) {
    char hex_value[HEX_STRING_LENGTH];
    convert_words_to_hex_line(word, hex_value);
    fprintf(output, "%s\n", hex_value);
}

int write_all_words_to_file(FILE *output, WordsList *words_list, int word_num) {
    Word *current = words_list->words;

    while (current) {
        fprintf(output, "%04d ", word_num);
        word_num++;

        write_word_to_file(output, current);
        current = current->next;
    }
    return word_num;
}

int get_base_and_offset(int address, int *base, int *offset) {
    *offset = address % ADDRESS_STEPS;
    *base = address - *offset;
    return 1;
}

bool create_data_word(WordsList *words_list, bool A, bool R, bool E, int data, int line_number,
                      WordsToReturnToList *returnTo, bool *returnToWordAdded) {
    Word *new_word = (Word *) malloc(sizeof(Word));
    if (new_word != NULL) {
        new_word->is_func_word = false;
        new_word->A = A;
        new_word->R = R;
        new_word->E = E;
        new_word->opcode = data;
        new_word->next = NULL;
    } else {
        return false;
    }
    push_to_words_list(words_list, new_word);
    if (returnTo && !(*returnToWordAdded)) {
        *returnToWordAdded = true;
        return create_word_to_return_to(returnTo, new_word, line_number);
    }
    return true;
}

bool create_func_word(WordsList *words_list, bool A, bool R, bool E, short func, short orig_reg, short orig_addressing,
                      short dest_reg, short dest_addressing) {
    Word *new_word = (Word *) malloc(sizeof(Word));
    if (new_word != NULL) {
        new_word->is_func_word = true;
        new_word->A = A;
        new_word->R = R;
        new_word->E = E;
        new_word->func = func;
        new_word->orig_reg = orig_reg;
        new_word->orig_addressing = orig_addressing;
        new_word->dest_reg = dest_reg;
        new_word->dest_addressing = dest_addressing;
        new_word->next = NULL;
    } else {
        return false;
    }
    push_to_words_list(words_list, new_word);
    return true;
}

WordsList *init_words_list() {
    WordsList *words_list = (WordsList *) malloc(sizeof(WordsList));
    if (words_list != NULL) {
        words_list->size = 0;
        words_list->words = NULL;
    } else {
        return NULL;
    }
    return words_list;
}

void push_to_words_list(WordsList *words_list, Word *new_word) {
    Word *current = words_list->words;
    new_word->index = words_list->size++;
    if (current == NULL) {
        words_list->words = new_word;
        return;
    }

    while (current->next)
        current = current->next;
    /* now current is pointing the last word in the list */
    current->next = new_word;
}

void destroy_words_list(WordsList *words_list) {
    Word *next, *current = words_list->words;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
    if (words_list != NULL)
        free(words_list);
}

void print_word(Word *current) {
    printf("Word index: %d\n", current->index);
    printf("A: %d, R: %d, E: %d\n", current->A, current->R, current->E);
    if (current->is_func_word) {
        printf("func: %d, orig_reg: %d, dest_reg: %d\n", current->func, current->orig_reg, current->dest_reg);
        printf("orig_addressing: %d, dest_addressing: %d\n", current->orig_addressing, current->dest_addressing);
    } else {
        printf("opcode / data: %d\n", current->opcode);
    }
}

void print_words_list(WordsList *words_list) {
    int i = 1;
    Word *current = words_list->words;
    printf("\n\n***** this is for debug, should not be in final code *** %d\n", words_list->size);
    while (current) {
        printf("printing word %d:\n", i);
        print_word(current);
        current = current->next;
        i++;
    }
}

WordsToReturnToList *init_words_to_return_list() {
    WordsToReturnToList *words_to_return = (WordsToReturnToList *) malloc(sizeof(WordsToReturnToList));
    if (words_to_return != NULL) {
        words_to_return->first = NULL;
    } else {
        return NULL;
    }
    return words_to_return;
}

void push_to_words_to_return_list(WordsToReturnToList *words_to_return, WordToReturnTo *new_words_to_return_to) {
    WordToReturnTo *current = words_to_return->first;
    if (current == NULL) {
        words_to_return->first = new_words_to_return_to;
        return;
    }

    while (current->next)
        current = current->next;
    /* now current is pointing the last word in the list */
    current->next = new_words_to_return_to;
}

void destroy_words_to_return_list(WordsToReturnToList *words_to_return) {
    WordToReturnTo *next, *current = words_to_return->first;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
    if (words_to_return != NULL)
        free(words_to_return);
}

bool create_word_to_return_to(WordsToReturnToList *words_to_return_to_list, Word *return_to, int line_number) {
    WordToReturnTo *new_word_to_return_to = (WordToReturnTo *) malloc(sizeof(WordToReturnTo));
    if (new_word_to_return_to) {
        new_word_to_return_to->word = return_to;
        new_word_to_return_to->line_number = line_number;
        new_word_to_return_to->next = NULL;
    } else {
        /* should print an error? */
        return false;
    }
    push_to_words_to_return_list(words_to_return_to_list, new_word_to_return_to);
    return true;
}

void print_word_to_return(WordToReturnTo *current, int index) {
    /* printf("printing word %d:\n", index);*/
    printf("line: %d\n", current->line_number);
    /*print_word(current->word);*/
}

void print_return_to_words_list(WordsToReturnToList *words_list) {
    int i = 1;
    WordToReturnTo *current = words_list->first;
    if (!current) {
        printf("No words to return to :(\n");
    }
    while (current) {
        print_word_to_return(current, i);
        current = current->next;
        i++;
    }
}
