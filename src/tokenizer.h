#define MAX_LENGTH 256


int isOperatorName(char *ch) {
    return (strcmp(ch, "xor") == 0 || strcmp(ch, "ls") == 0 || strcmp(ch, "rs") == 0 || strcmp(ch, "lr") == 0 ||
            strcmp(ch, "rr") == 0 || strcmp(ch, "not") == 0);
}

int isOperatorSymbol(char *ch) {
    return strchr("^<>#$", *ch) != NULL;
}

int isSymbol(char *ch) {
    return strchr("+-*&|(),=", *ch) != NULL;
}

int returnIndex(Token *variables, int num_tokens, char *name) {
    for (int i = 0; i < num_tokens; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

Token *tokenizer(char *input, int *num_tokens, Token *variables, int *num_variables) {
    // keep track of the number of local tokens
    int num_local_tokens = 0;
    // get the length of the input string
    int input_length = strlen(input);

    // allocate memory for the array of tokens
    Token *tokens = malloc(sizeof(Token) * input_length);

    // iterate through the input string, one character at a time
    int i = 0;
    while (i < input_length) {
        if (input[i] == '%') {
            tokens[i].name = "Comment_line";

            return tokens;
        }
        // if the current character is a digit, parse it as a number
        if (isdigit(input[i])) {
            // find the end of the number
            int j = i;
            while (isdigit(input[j])) {
                j++;
            }

            // extract the number string from the input
            char *num_str = malloc(sizeof(char) * (j - i + 1));
            strncpy(num_str, &input[i], j - i);
            num_str[j - i] = '\0';

            // convert the number string to an integer
            LLI num = atoll(num_str);

            // add the number as a new token to the array
            tokens[num_local_tokens].value = num;
            tokens[num_local_tokens].name = "num_str";
            tokens[num_local_tokens].type = TOKEN_TYPE_NUMBER;
            (num_local_tokens)++;
            (*num_tokens)++;

            // move the index to the end of the number
            i = j;

            // free the temporary number string memory
            free(num_str);

            // if the current character is a letter, parse it as a string
        } else if (isalpha(input[i])) {
            // find the end of the string
            int j = i;
            while (isalpha(input[j])) {
                j++;
            }
            // extract the string from the input
            char *name = malloc(sizeof(char) * (j - i + 1));
            strncpy(name, &input[i], j - i);
            name[j - i] = '\0';
            if (isOperatorName(name)) {   // if it's an operator, add the string as a new token to the array
                tokens[num_local_tokens].name = name;
                if (strcmp(name, "xor") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_XOR;
                    tokens[num_local_tokens].name = "^";
                } else if (strcmp(name, "ls") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_LS;
                    tokens[num_local_tokens].name = "<";
                } else if (strcmp(name, "rs") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_RS;
                    tokens[num_local_tokens].name = ">";
                } else if (strcmp(name, "lr") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_LR;
                    tokens[num_local_tokens].name = "$";
                } else if (strcmp(name, "rr") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_RR;
                    tokens[num_local_tokens].name = "#";
                } else if (strcmp(name, "not") == 0) {
                    tokens[num_local_tokens].type = TOKEN_TYPE_NOT;
                    tokens[num_local_tokens].name = "!";
                }
            } else {   // if it's a variable, add the string as a new token to the array
                tokens[num_local_tokens].value = 0;
                tokens[num_local_tokens].name = name;
                tokens[num_local_tokens].type = TOKEN_TYPE_IDENTIFIER;

                // If variable is not on the array, add it to the array of variables
                if (returnIndex(variables, *num_variables, name) == -1) {
                    variables[*num_variables].value = 0;
                    variables[*num_variables].name = name;
                    variables[*num_variables].type = TOKEN_TYPE_IDENTIFIER;
                    (*num_variables)++;
                }
            }
            (num_local_tokens)++;
            (*num_tokens)++;

            // move the index to the end of the string
            i = j;

            // if the current character is an operator, parse it as an operator
        } else if (isSymbol(&input[i])) {
            char *name = malloc(sizeof(char) * 2);
            strncpy(name, &input[i], 1);
            if (input[i] == '+') {
                tokens[num_local_tokens].type = TOKEN_TYPE_PLUS;
            } else if (input[i] == '-') {
                tokens[num_local_tokens].type = TOKEN_TYPE_MINUS;
            } else if (input[i] == '*') {
                tokens[num_local_tokens].type = TOKEN_TYPE_ASTERISK;
            } else if (input[i] == '&') {
                tokens[num_local_tokens].type = TOKEN_TYPE_AMPERSAND;
            } else if (input[i] == '|') {
                tokens[num_local_tokens].type = TOKEN_TYPE_PIPE;
            } else if (input[i] == '(') {
                tokens[num_local_tokens].type = TOKEN_TYPE_OPENPARENTHESIS;
            } else if (input[i] == ')') {
                tokens[num_local_tokens].type = TOKEN_TYPE_CLOSEPARENTHESIS;
            } else if (input[i] == ',') {
                tokens[num_local_tokens].type = TOKEN_TYPE_COMMA;
            } else if (input[i] == '=') {
                tokens[num_local_tokens].type = TOKEN_TYPE_EQUALS;
            }
            tokens[num_local_tokens].name = name;
            tokens[num_local_tokens].name[1] = '\0';
            tokens[num_local_tokens].value = 0;
            (num_local_tokens)++;
            (*num_tokens)++;
            i++;
        } else if (isspace(input[i])) {
            i++;
        } else {
            //printf("Error: Invalid character%c\n", input[i]);
            return NULL;
        }
    }
    if (tokens == NULL) {
        return NULL;
    }
    // return the array of tokens
    return tokens;
}