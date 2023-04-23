#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "token.h"
//#include "tokenizer.h"
#include "formatController.h"
#include "postfixCalculator.h"

#define MAX_LENGTH 256
#define MAX_VARIABLES 128


int main() {
    int num_variables = 0;
    Token *variables = malloc(sizeof(Token) * MAX_VARIABLES);
    char input[256];
    int bok = 0;
    while (bok < 1) {
        int num_tokens = 0; //  keep track of the number of tokens
        int index = 0;     //  keep track of the index of the tokens
        int output_count = 0;
        int error = 0; // boolean for errors

        printf(">");
        fgets(input, 256, stdin);

        //  if the input is blank
        int len = strlen(input);
        int is_empty = 1;
        for (int i = 0; i < len; i++) {
            if (!isspace(input[i])) {
                is_empty = 0;
                break;
            }
        }

        if (is_empty) {
            continue;
        } else {

            // tokenize the input
            Token *tokens = tokenizer(input, &num_tokens, variables, &num_variables);

            // if function returns null, there is an error
            if (tokens == NULL) {
                printf("Error!\n");
            } else if (num_tokens == 0) {
                continue;
            } else {

                // if the first token is a comment, skip the rest
                if (strcmp(tokens[0].name, "Comment_line") == 0) {
                   // printf("\n");
                    continue;
                } else {
                    Token *formatted = formatController(tokens, num_tokens, 0, &index, &output_count);

                    // if formatController returns null, there is an error
                    if (formatted == NULL) {
                        printf("Error!\n");
                        continue;
                    } else {
                        // If the expression is an equation
                        if (formatted[1].type == TOKEN_TYPE_EQUALS) {

                            // create a pointer to the first element of the array
                            Token *ptr = formatted;
                            // access the element at index 0
                            Token variable = *(ptr);

                            // the expression after the equal sign will be our value, so we take formatted form second element
                            Token *postfix = infixToPostfix(&formatted[2], num_tokens - 2, &error);

                            // if there is error in converting to postfix
                            if (error) {
                                printf("Error!\n");
                                continue;
                            } else {
                                long long int result = evaluatePostfix(postfix, num_tokens - 2, variables,
                                                                       num_variables, &error);

                                // if there is an error in evaluating the postfix
                                if (error) {
                                    printf("Error!\n");
                                    continue;
                                } else {
                                    int var_index = returnIndex(variables, num_variables, variable.name);
                                    variables[var_index].value = result;

                                }
                            }
                            int i = 0;
                            if (postfix != NULL)
                            while (postfix[i].name != NULL) {
                                postfix[i].name = NULL;
                                postfix[i].value = 0;
                                postfix[i].type = 0;
                            }
                        }
                            // if there is no equal sign in the input
                        else {
                            Token *postfix = infixToPostfix(formatted, num_tokens, &error);

                            // if there is error in converting to postfix
                            if (error) {
                                printf("Error!\n");
                                continue;
                            } else {

                                long long int result = evaluatePostfix(postfix, num_tokens, variables, num_variables,
                                                                       &error);

                                // if there is an error in evaluating the postfix
                                if (error) {
                                    printf("Error!\n");
                                    continue;
                                } else {
                                    printf("%lld\n", result);
                                }
                            }
                            int i = 0;
                            if (postfix != NULL)
                            while (postfix[i].type != 0) {
                                postfix[i].name = NULL;
                                postfix[i].type = 0;
                                postfix[i].value = 0;
                                ++i;
                            }
                        }
                    }
                    int i = 0;
                    if (formatted != NULL)
                    while (formatted[index].type != 0) {
                        formatted[i].name = NULL;
                        formatted[i].type = 0;
                        formatted[i].value = 0;
                        ++i;
                    }
                }
            }
            int i = 0;
            if (tokens != NULL)
                while (tokens[i].type != 0) {
                    tokens[i].name = NULL;
                    tokens[i].type = 0;
                    tokens[i].value = 0;
                }
        }
    }
    return 0;
}
