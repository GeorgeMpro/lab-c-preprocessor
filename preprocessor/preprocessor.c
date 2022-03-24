#include "preprocessor.h"

/* return true if string is a macro definition line or false if not */
bool is_macro_def(char *line, char *name) {
    char clean_line[MAX_LINE_LENGTH];

    clean_whitespace_chars(line, clean_line);

    if (strncmp("macro", clean_line, 5) == 0) {
        strcpy(name, &clean_line[5]);
        return true;
    }
    return false;
}

/* copy the macro contest to macro_text */
void get_macro_text(char *macro_text, FILE *inputf) {
    char line[MAX_LINE_LENGTH] = "";
    char clean_line[MAX_LINE_LENGTH] = "";

    while (strcmp("endm", clean_line) != 0) {
        strcat(macro_text, line);
        fgets(line, MAX_LINE_LENGTH + 1, inputf);
        clean_whitespace_chars(line, clean_line);
    }
}

/* return the macro that called in the current line or null if its not macro call line */
Macro *is_macro_call(Macro *m, char *line) {
    char clean_line[MAX_LINE_LENGTH];

    clean_whitespace_chars(line, clean_line);

    return is_macro_name(m, clean_line);
}

/* main function of the preprocessor. get a file.as and creat file.am with spread macros */
bool span_macros(char *file_name) {
    Macro *macros_table = NULL;
    Macro *m = NULL;
    FILE *inputf = NULL;
    FILE *outputf = NULL;
    char line[MAX_LINE_LENGTH + 1];
    char macro_name[MAX_MACRO_NAME_LENGTH];
    char macro_text[6 * MAX_LINE_LENGTH];
    char file_name_copy[MAX_FILE_NAME_LENGTH];
    char c;

    strcpy(file_name_copy, file_name);

    inputf = fopen(strcat(file_name, ".as"), "r");

    if (inputf == NULL) {
        printf("cannot open file: %s\n", file_name);
        return false;
    } else {
        outputf = fopen(strcat(file_name_copy, ".am"), "w");

        while ((c = getc(inputf)) != EOF) {
            if (c == '\n') {
                fprintf(outputf, " \n");
                continue;
            }

            line[0] = c;
            fgets(&line[1], MAX_LINE_LENGTH, inputf); /* get line */

            m = is_macro_call(macros_table, line);

            /* line is macro call- copy the macro text to the output file */
            if (m != NULL) {
                fprintf(outputf, "%s", m->macro_text);
            }

                /* line is macro defenition- copy the macro text to the macros table */
            else if (is_macro_def(line, macro_name)) {
                strcpy(macro_text, "");
                get_macro_text(macro_text, inputf);
                macros_table = insert_macro(macros_table, macro_name, macro_text);
            }

                /* regular line. copy it to the output file */
            else {
                fprintf(outputf, "%s", line);
            }
        }

        destroy_macro_table(macros_table);
        fclose(inputf);
        fclose(outputf);
        return true;
    }
}
