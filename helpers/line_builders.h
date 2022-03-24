#ifndef HELPERS

#include "helpers.h"

#endif

#define LINE_BUILDER

#define ALL_OPCODE_BITS_ON (65535)
#define A_LOCATION (18)
#define R_LOCATION (17)
#define E_LOCATION (16)
#define FUNC_LOCATION (12)
#define ORIG_REG_LOCATION (8)
#define ORIG_ADD_LOCATION (6)
#define DEST_REG_LOCATION (2)
#define DEST_ADD_LOCATION (0)

#define MAX_WORD_COUNT (6)
#define ADDRESS_STEPS (16)

#define HEX_STRING_LENGTH (15)
#define HEX_STRING_SEP '-'
#define LINE_BITS_COUNT (20)
#define GROUP_SIZE (4)
#define GORUPS_COUNT (LINE_BITS_COUNT / GROUP_SIZE)

#define HEX_1111 (0xf)
#define HEX_11 (3)

/* Word structure
 *
 *   In case of first word should be using opcode
 *   second word the other values
 *   third data (data of additional words) is in opcode (to save space)
 */
typedef struct Word {
    bool is_func_word;
    bool A;
    bool R;
    bool E;
    short func;
    short orig_reg;
    short orig_addressing;
    short dest_reg;
    short dest_addressing;
    int opcode;
    int index;
    struct Word *next;
} Word;

typedef struct WordsList {
    Word *words;
    int size;
} WordsList;

typedef struct WordToReturnTo {
    Word *word;
    int line_number;
    struct WordToReturnTo *next;
} WordToReturnTo;

typedef struct WordsToReturnToList {
    WordToReturnTo *first;
} WordsToReturnToList;

int get_ARE(Word *word);

int convert_func_word_to_int(Word *word);

void convert_int_to_hex_line(int line_value, char hex_value[HEX_STRING_LENGTH]);

void convert_words_to_hex_line(Word *word, char hex_value[HEX_STRING_LENGTH]);

void write_word_to_file(FILE *output, Word *word);

int write_all_words_to_file(FILE *output, WordsList *words_list, int word_num);

int get_base_and_offset(int address, int *base, int *offset);

WordsList *init_words_list();

void push_to_words_list(WordsList *words_list, Word *new_words);

void destroy_words_list(WordsList *words_list);

bool create_data_word(WordsList *words_list, bool A, bool R, bool E, int data, int line_number,
                      WordsToReturnToList *returnTo, bool *returnToWordAdded);

bool create_func_word(WordsList *words_list, bool A, bool R, bool E, short func, short orig_reg, short orig_addressing,
                      short dest_reg, short dest_addressing);

WordsToReturnToList *init_words_to_return_list();

void push_to_words_to_return_list(WordsToReturnToList *words_to_return, WordToReturnTo *new_words_to_return_to);

void destroy_words_to_return_list(WordsToReturnToList *words_to_return_to_list);

bool create_word_to_return_to(WordsToReturnToList *words_to_return_to_list, Word *return_to, int line_number);
/*//todo del*/
/* Debuging and Shit */
void print_word(Word *current);

void print_words_list(WordsList *words_list);

void print_return_to_words_list(WordsToReturnToList *words_list);
