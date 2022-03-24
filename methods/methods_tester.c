#include "methods_tester.h"

//todo del
char *words_to_test[20] =
        {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts",
         "stop", "ofek", "yarin", "mov1", "mov "};

void test_commands_list() {
    int i = 0;
    Method *list = init_methods_list();
    char *word;

    for (; i < 20; i++) {
        word = words_to_test[i];
        printf("%s: %d\n", word, method_index(list, word));
    }
    return;
}