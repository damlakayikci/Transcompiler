
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

char* read_last_line(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: failed to open file \"%s\".\n", filename);
        return NULL;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int linecount = 0;

    // Count the number of lines in the file
    while ((read = getline(&line, &len, fp)) != -1) {
        linecount++;
    }

    // Seek to the beginning of the last line
    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < linecount-1; i++) {
        getline(&line, &len, fp);
    }

    // Read the last line
    getline(&line, &len, fp);

    fclose(fp);
    return line;
}

int main() {
    int num_variables = 0;
    Token *variables = malloc(sizeof(Token) * MAX_VARIABLES);
    char input[256];
    int bok = 0;
    int variableCount = 0;
    char *operations[256];
    int opCount = 0;
    int numLinesInFile = 0;



    FILE *intermediate;
    char nameFile[] = "intermediate.ll";

    intermediate = fopen(nameFile, "w");
    if (intermediate == NULL) {
        printf("Error creating file.\n");
        return 1;
    }



// TODO bu ne sikimse o olucak
    while (bok < 7) {
        int num_tokens = 0; //  keep track of the number of tokens
        int index = 0;     //  keep track of the index of the tokens
        int output_count = 0;
        int error = 0; // boolean for errors
        int lineCount = 0;
        int equalFlag = 0;

// TODO bu kalkicak sanirim
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

            // if function returns null, there is an error
            if (tokens == NULL) {
                printf("Error on line %d!\n", lineCount);
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
                        printf("Error on line %d!\n", lineCount);
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

                            // if there is error in converting to postfix
                            if (error) {
                                printf("Error on line %d!\n", lineCount);
                                continue;
                            } else {

                                long long int result = evaluatePostfix(postfix, num_tokens - 2, variables,
                                                                       num_variables, &error, intermediate, &variableCount,
                                                                       operations, &opCount);


                                // if there is an error in evaluating the postfix
                                if (error) {
                                    printf("Error on line %d!\n", lineCount);
                                    continue;
                                } else {
                                    if (num_tokens== 3) {
                                        fprintf(intermediate, "\tstore i32 %lld, i32* %%%s\n", tokens[2].value, tokens[0].name);
                                    } else{
                                        int var_index = returnIndex(variables, num_variables, variable.name);
                                        variables[var_index].value = result;
                                        // TODO a=3 formatindaysa diye bi if eklenicek ustte ne var bilmiyorum
                                        fprintf(intermediate, "\tstore i32 %%%d, i32* %%%s\n", variableCount, tokens[0].name);
                                    }

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

                            // if there is error in converting to postfix
                            if (error) {
                                printf("Error on line %d!\n", lineCount);
                                continue;
                            } else {

                                long long int result = evaluatePostfix(postfix, num_tokens, variables, num_variables,
                                                                       &error, intermediate, &variableCount, operations,
                                                                       &opCount);

                                // if there is an error in evaluating the postfix
                                if (error) {
                                    printf("Error on line %d!\n", lineCount);
                                    continue;
                                } else { // TODO buraya bak
                                    printf("%lld\n", result);
                                    // call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %8 )
                                    fprintf(intermediate ,"\tcall i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %%%d)\n", variableCount);
                                    variableCount++; // TODO sorgulama cagatay boyle olmasi gerekiyomus
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

    // TODO       DON'T FORGET  ERROR CHECK
    // close the intermediate file
    fclose(intermediate);

    FILE *file;
    char filename[] = "file.ll";
    file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error creating file.\n");
        return 1;
    }


    char beginning[] = "; ModuleID = 'advcalc2ir'\ndeclare i32 @printf(i8*, ...)\n\n@print.str = constant [4 x i8] c\"%d\\0A\\00\"\n\ndefine i32 @main() {\n";
    fprintf(file, "%s", beginning);
    // for all variables in variables array
    for (int i = 0; i < num_variables; i++) {
        fprintf(file, "\t%%%s = alloca i32\n", variables[i].name);
    }

    // copy contents of intermediate file to file.ll
    FILE *from;
    char  c;
    // Open one file for reading
    from = fopen(nameFile, "r");
    if (from == NULL){
        printf("Cannot open file %s \n", filename);
        exit(0);
    }

    // Read contents from file
    c = fgetc(from);
    while (c != EOF){
        fputc(c, file); // write to file.ll
        c = fgetc(from);
    }

    fclose(from);

    fprintf(file, "\tret i32 0\n"); // write return value to file.ll
    fprintf(file, "%c", '}'); // write closing bracket to file.ll
    fclose(file); // close file.ll
    return 0;
}
