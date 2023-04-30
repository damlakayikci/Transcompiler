
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
    int variableCount = 0;
    char *operations[256] ;
    int opCount = 0; // keep count of operations

    FILE *file;
    char filename[] = "file.ll";

    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating file.\n");
        return 1;
    }
    char beginning[] = "; ModuleID = 'advcalc2ir'\ndeclare i32 @printf(i8*, ...)\n\n@print.str = constant [4 x i8] c\"%d\\0A\\00\"\n\ndefine i32 @main() {\n";
    fprintf(file, "%s", beginning);

    while (bok < 7) {
        while (bok < 3) {
            int num_tokens = 0; //  keep track of the number of tokens
            int index = 0;     //  keep track of the index of the tokens
            int output_count = 0;
            int error = 0; // boolean for errors
            int lineCount = 0;
            int equalFlag = 0;


            printf(">");
            fgets(input, 256, stdin); //  read the input

            //  if the input is blank
            int len = strlen(input);
            int is_empty = 1;
            for (int i = 0; i < len; i++) {
                if (!isspace(input[i])) {
                    is_empty = 0;
                    break;
                } //  skip the rest
            }

            if (is_empty) {
                continue;
            } else {

                // tokenize the input
                Token *tokens = tokenizer(input, &num_tokens, variables, &num_variables);

//            for (int i = 0; i < num_variables; i++) {
//                printf("Variable %d\t\ttype: %u\t\tvalue: %lld\t\tname: %s\n", i, variables[i].type, variables[i].value,
//                       variables[i].name);
//            }

                // if function returns null, there is an error
                if (tokens == NULL) {
                    printf("Error!\n");
                } else if (num_tokens == 0) {
                    continue;
                } else {

                    // if the first token is a comment, skip the rest
                    if (strcmp(tokens[0].name, "Comment_line") == 0) {
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
                                equalFlag = 1;

                                // create a pointer to the first element of the array
                                Token *ptr = formatted;
                                // access the element at index 0
                                Token variable = *(ptr);

                                // the expression after the equal sign will be our value, so we take formatted form second element
                                Token *postfix = infixToPostfix(&formatted[2], num_tokens - 2, &error);

//                            for (int i = 0; i < num_tokens; i++) {
//                                printf("Postfix %d\t\ttype: %u\t\tvalue: %lld\t\tname: %s\n", i, postfix[i].type,
//                                       postfix[i].value, postfix[i].name);
//                            }
<<<<<<< Updated upstream
                            // if there is error in converting to postfix
                            if (error) {
                                printf("Error!\n");
                                continue;
                            } else {
                                long long int result = evaluatePostfix(postfix, num_tokens - 2, variables,
                                                                       num_variables, &error, file, &variableCount, operations, &opCount);


                                printf("Operation count in main: %d\n", opCount);
                                // print elements of operations
                                for (int i = 0; i < opCount; i++) {
                                    printf("Eq Operations %d\t\t%s\n", i, operations[i]);
                                }

                                // if there is an error in evaluating the postfix
=======
                                // if there is error in converting to postfix
>>>>>>> Stashed changes
                                if (error) {
                                    printf("Error!\n");
                                    continue;
                                } else {
                                    long long int result = evaluatePostfix(postfix, num_tokens - 2, variables,
                                                                           num_variables, &error, file, &variableCount);

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
                                equalFlag = 0;
                                Token *postfix = infixToPostfix(formatted, num_tokens, &error);

                                // print all elements of postfix
                                for (int i = 0; i < num_tokens; i++) {
                                    printf("Postfix %d\t\ttype: %u\t\tvalue: %lld\t\tname: %s\n", i, postfix[i].type,
                                           postfix[i].value, postfix[i].name);
                                }

<<<<<<< Updated upstream
                            // print all elements of postfix
                            for (int i = 0; i < num_tokens; i++) {
                                printf("Postfix %d\t\ttype: %u\t\tvalue: %lld\t\tname: %s\n", i, postfix[i].type,
                                       postfix[i].value, postfix[i].name);
                            }

                            // if there is error in converting to postfix
                            if (error) {
                                printf("Error!\n");
                                continue;
                            } else {

                                long long int result = evaluatePostfix(postfix, num_tokens, variables, num_variables,
                                                                       &error, file, &variableCount, operations, &opCount);

                                printf("Operation count in main: %d\n", opCount);
                                // print elements of operations
                                for (int i = 0; i < opCount; i++) {
                                    printf("Noteq Operations %d\t\t%s\n", i, operations[i]);
                                }
                                // if there is an error in evaluating the postfix
=======
                                // if there is error in converting to postfix
>>>>>>> Stashed changes
                                if (error) {
                                    printf("Error!\n");
                                    continue;
                                } else {
<<<<<<< HEAD
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
        lineCount++;
        bok++;
    }


    // for all variables in variables array
    for (int i = 0; i < num_variables; i++) {
        fprintf(file, "\t%%%s = alloca i32\n", variables[i].name);
    }
=======
>>>>>>> 926f9fe1fcd57210df871720a345cd9a9bc49b8c

                                    long long int result = evaluatePostfix(postfix, num_tokens, variables, num_variables,
                                                                           &error, file, &variableCount);

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
            lineCount++;
            bok++;
        }
        // for all variables in variables array
        for (int i = 0; i < num_variables; i++) {
            fprintf(file, "\t%%%s = alloca i32\n", variables[i].name);
        }

        fprintf(file, "\tret i32 0\n");
        fprintf(file, "%c", '}'); // print closing bracket
        fclose(file);
        return 0;
    }
