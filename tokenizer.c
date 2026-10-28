#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>


char **tokenize_input(char *input) {
    char **tokens = malloc(1024 * sizeof(char*));
    int token_count = 0;

    int in_single_quotes = 0;
    int in_double_quotes = 0;
    char curr_token[1024];
    int char_idx = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        char nc = input[i + 1];
        if (c == '\\' && !in_single_quotes) {
            curr_token[char_idx++] = nc;
            i++;

        } else if (c == '\"') {
            in_double_quotes = !in_double_quotes;

        }else if (c == '\'' && !in_double_quotes) {
            in_single_quotes = !in_single_quotes;
        } else if (c == ' ' && !in_single_quotes && !in_double_quotes) {
            if (char_idx > 0) {
                curr_token[char_idx] = '\0';
                tokens[token_count++] = strdup(curr_token);
                char_idx = 0;
            }
        } else {
            curr_token[char_idx++] = c;


        }
    }
    if (char_idx > 0) {
        curr_token[char_idx] = '\0';
        tokens[token_count++] = strdup(curr_token);
    }

    tokens[token_count] = NULL;
    return tokens;

}

void rendl(char input[]) {
    char *input_end = strchr(input, '\n');
    if (input_end != NULL) {
        *input_end = '\0';
    }
}
