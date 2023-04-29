#include <stdio.h>
#include "tokenizer.h"

#define MAX_LENGTH 256
#define INT_BITS 64
#define LLI long long int

typedef struct {
    Token items[MAX_LENGTH];
    int top;
} TokenStack;

// regular stack methods
void push(TokenStack *stack, Token item) {
    stack->top++;
    stack->items[stack->top].name = item.name;
    stack->items[stack->top].type = item.type;
}

Token pop(TokenStack *stack, Token *postfix) {
    Token item = stack->items[stack->top];
    postfix->name = item.name;
    postfix->type = item.type;
    stack->top--;
    return item;
}

Token peek(TokenStack *stack) {
    return stack->items[stack->top];
}

// Function to return precedence of operators
int precedence(char *operator) {
    if (strcmp(operator, "|") == 0)
        return 1;
    else if (strcmp(operator, "&") == 0)
        return 2;
    else if (strcmp(operator, "+") == 0 || strcmp(operator, "-") == 0)
        return 3;
    else if (strcmp(operator, "*") == 0)
        return 4;
    else if (strcmp(operator, "^") == 0 || strcmp(operator, "<") == 0 || strcmp(operator, ">") == 0 ||
             strcmp(operator, "$") == 0 || strcmp(operator, "#") == 0)
        return 0;
    else if (strcmp(operator, "!") == 0)
        return 6;
    else
        return -1;
}

// Function to check if the scanned character
// is an operator
int isOperator(char *ch) {
    return (strcmp(ch, "+") == 0 || strcmp(ch, "-") == 0 || strcmp(ch, "*") == 0 ||
            strcmp(ch, "|") == 0 || strcmp(ch, "<") == 0 || strcmp(ch, ">") == 0 || strcmp(ch, "^") == 0 ||
            strcmp(ch, "$") == 0 || strcmp(ch, "#") == 0 || strcmp(ch, "!") == 0) || strcmp(ch, "&") == 0;

}


// function that converts infix expression to postfix expression
Token *infixToPostfix(Token *infix, int infixSize, int *error) {
    int i, j;
    Token *postfix = malloc(sizeof(Token) * (infixSize + 1));
    TokenStack stack;
    // initialize top to -1 to indicate empty stack
    stack.top = -1;

    for (i = 0, j = 0; i < infixSize; i++) {
        // If the scanned character is operand
        // add it to the postfix expression
        if (infix[i].name != NULL) { // ignore null tokens
            if (infix[i].type == TOKEN_TYPE_IDENTIFIER) {
                postfix[j].name = infix[i].name;
                postfix[j++].type = infix[i].type;
            } else if (infix[i].type == TOKEN_TYPE_NUMBER) {
                postfix[j].name = infix[i].name;
                postfix[j].value = infix[i].value;
                postfix[j++].type = infix[i].type;
            }

                // if the scanned character is '(' push it in the stack
            else if (strcmp(infix[i].name, "(") == 0) {
                push(&stack, infix[i]);
            }

                // if the scanned character is ')'
                // pop the stack and add it to the
                // output string until empty or '(' found
            else if (strcmp(infix[i].name, ")") == 0) {
                while (stack.top > -1 && strcmp(peek(&stack).name, "(") != 0) {
                    pop(&stack, &postfix[j++]);
                }
                if (stack.top > -1 && strcmp(peek(&stack).name, "(") != 0) {
                    *error = 1;
                    return 0;
                } else
                    stack.top--;
            }

                // If the scanned character is an operator
                // push it in the stack
            else if (isOperator(infix[i].name)) {
                while (stack.top > -1 && precedence(peek(&stack).name) >= precedence(infix[i].name)) {
                    pop(&stack, &postfix[j++]);
                }
                push(&stack, infix[i]);
            }
        }
    }

    // Pop all remaining elements from the stack
    while (stack.top > -1) {
        if (strcmp(peek(&stack).name, "(") == 0) {
            *error = 1;
            return 0;
        }
        pop(&stack, &postfix[j++]);
    }
    return postfix;
}

int isEmpty(TokenStack *stack) {
    return stack->top == -1;
}

// Regular stack methods for evaluatePostfix function
Token popPostfix(TokenStack *stack) {
    if (!isEmpty(stack))
        return stack->items[stack->top--];
    const Token result;
    return result;
}

void pushPostfix(TokenStack *stack, Token element) {
    stack->top++;
    stack->items[stack->top].name = element.name;
    stack->items[stack->top].type = element.type;
    stack->items[stack->top].value = element.value;
}

LLI leftRotate(LLI n, LLI d) {
    /* In n<<d, last d bits are 0. To put first 3 bits of n at
      last, do bitwise or of n<<d with n >>(INT_BITS - d) */
    return (n << d) | (n >> (INT_BITS - d));
}

/*Function to right rotate n by d bits*/
LLI rightRotate(LLI n, LLI d) {
    /* In n>>d, first d bits are 0. To put last 3 bits of at
      first, do bitwise or of n>>d with n <<(INT_BITS - d) */
    return (n >> d) | (n << (INT_BITS - d));
}
void modifyName(Token *token) {
    printf("Pre %s\n", token->name);
    if (token->type == TOKEN_TYPE_IDENTIFIER) {
        char *dest = "i32 %";
        char *new_name = malloc(strlen(dest) + strlen(token->name) + 1); // allocate memory for new string
        strcpy(new_name, dest); // copy "i32 " to new string
        strcat(new_name, token->name); // concatenate token.name to new string
        free(token->name); // free the old name
        token->name = new_name; // update token name to new string

    } else {
        char *new_name = (char *) malloc(20 * sizeof(char)); // allocate enough memory to hold the formatted string
        sprintf(new_name, "%lld", token->value); // write the formatted string to the allocated memory
        //free(token->name); // free the old name
        token->name = new_name; // update token.name to point to the new string
    }
    printf("Post %s\n", token->name);
}


// The main function that returns value
// of a given postfix expression
LLI evaluatePostfix(Token *postfix, int postfixSize, Token *variables, int num_variables, int *error, FILE *file,
                    int *variableCount) {
    TokenStack stack;
    stack.top = -1;
    Token token1, token2;
    LLI val1 = 0;
    LLI val2 = 0;
    Token newToken;
    newToken.type = TOKEN_TYPE_IDENTIFIER;
    newToken.name = malloc(250 * sizeof(char));
    newToken.value = 0;

    int i = 0;
    // Scan all characters one by one
    for (i = 0; i < postfixSize; ++i) {
        if (postfix[i].name != NULL) {
            if (isOperator(postfix[i].name)) {
                // check not operator first
//                if (strcmp(postfix[i].name, "!") == 0) {
//                    if (peek(&stack).type == TOKEN_TYPE_IDENTIFIER) {
//                        if (returnIndex(variables, num_variables, peek(&stack).name) == -1) {
//                            val1 = popPostfix(&stack).value;
//                        } else { // if variable is found in the variable list get the value from the variable list
//                            val1 = variables[returnIndex(variables, num_variables, popPostfix(&stack).name)].value;
//                        }
//                        pushPostfix(&stack, ~val1);
//                    } else if (peek(&stack).type == TOKEN_TYPE_NUMBER) {
//                        val1 = popPostfix(&stack).value;
//                        pushPostfix(&stack, ~val1);
//                    } else {
//                        *error = 1;
//                        return 0;
//                    }
//                } else
                if (peek(&stack).type == TOKEN_TYPE_IDENTIFIER || peek(&stack).type == TOKEN_TYPE_NUMBER) {
                    // if it is a number
                    if (returnIndex(variables, num_variables, peek(&stack).name) == -1) {
                        token1 = popPostfix(&stack);
                        val1 = token1.value;
                    } else {
                        //val1 = variables[returnIndex(variables, num_variables, popPostfix(&stack).name)].value;
                        token1 = variables[returnIndex(variables, num_variables, popPostfix(&stack).name)];
                        val1 = token1.value;
//                        printf("\t%%%d = load i32, i32* %%%s\n", *variableCount, variable.name);
                        fprintf(file, "\t%%%d = load i32, i32* %%%s\n", (*variableCount)++, token1.name);
                    }
                    if (peek(&stack).type == TOKEN_TYPE_IDENTIFIER || peek(&stack).type == TOKEN_TYPE_NUMBER) {
                        if (returnIndex(variables, num_variables, peek(&stack).name) == -1) {
                            token2 = popPostfix(&stack);
                            val2 = token2.value;
                        } else {
                            // val2 = variables[returnIndex(variables, num_variables, popPostfix(&stack).name)].value;
                            token2 = variables[returnIndex(variables, num_variables, popPostfix(&stack).name)];
                            val2 = token2.value;
//                            printf("\t%%%d = load i32, i32* %%%s\n", *variableCount, variable.name);
                            fprintf(file, "\t%%%d = load i32, i32* %%%s\n", (*variableCount)++, token2.name);
                        }

                        printf("PRE  token1.name  %s,token2.name  %s\n", token1.name, token2.name);

                        modifyName(&token1);
                        modifyName(&token2);

                        printf("POST  token1.name  %s,token2.name  %s\n", token1.name, token2.name);

                        // evaluate the expression
                        switch (postfix[i].name[0]) {
                            case '+':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                fprintf(file, "\t%s = add i32 %s, %s\n", newToken.name, token1.name, token2.name);
                                //printf("\t%s = add i32 %s, %s\n", newToken.name, name1, name2);
                                newToken.value = val2 + val1;
                                pushPostfix(&stack, newToken);
                                break;
                            case '-':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = val2 - val1;
                                pushPostfix(&stack, newToken);
                                break;
                            case '*':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = val2 * val1;
                                pushPostfix(&stack, newToken);
                                break;
                            case '^':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = val2 ^ val1;
                                pushPostfix(&stack, newToken);
                                break;
                            case '$':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = leftRotate(val2, val1);
                                pushPostfix(&stack, newToken);
                                break;
                            case '#':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = rightRotate(val2, val1);
                                pushPostfix(&stack, newToken);
                                break;
                            case '<':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = val2 << val1;
                                pushPostfix(&stack, newToken);
                                break;
                            case '>':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = val2 >> val1;
                                pushPostfix(&stack, newToken);
                                break;
                            case '&':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = val2 & val1;
                                pushPostfix(&stack, newToken);
                                break;
                            case '|':
                                sprintf(newToken.name, "%%%d", (*variableCount)++);
                                newToken.value = val2 | val1;
                                pushPostfix(&stack, newToken);
                                break;
                        }
                    } else { // else something went wrong
                        *error = 1;
                        return 0;
                    }
                } else { // else something went wrong
                    *error = 1;
                    return 0;
                }
            } else if (postfix[i].type == TOKEN_TYPE_NUMBER) {
                pushPostfix(&stack, postfix[i]);
            } else if (postfix[i].type == TOKEN_TYPE_IDENTIFIER) {
                if (returnIndex(variables, num_variables, postfix[i].name) == -1) {
                    pushPostfix(&stack, postfix[i]);
                } else {
                    pushPostfix(&stack, variables[returnIndex(variables, num_variables, postfix[i].name)]);
                }
            } else { // else something went wrong
                *error = 1;
                return 0;
            }
        }
    }
    return popPostfix(&stack).value;
}
