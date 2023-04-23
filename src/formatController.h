
#define MAX_LENGTH 256

int isFunctionOperator(char *ch) {
    return (strcmp(ch, "^") == 0 || strcmp(ch, "<") == 0 || strcmp(ch, ">") == 0 || strcmp(ch, "$") == 0 ||
            strcmp(ch, "#") == 0);
}

int shouldntRepeat(char *ch) {
    return strchr("+-*&|,=", *ch) != NULL;
}


Token *formatController(Token *input, int inputSize, int recursive, int *index, int *output_count) {
    Token *output = malloc(sizeof(Token) * inputSize);
    int recIndex;
    int nonRecIndex;
    // if there is an equal sign it should be in the second position and there should be an identifier before it
    if ((input[1].type == TOKEN_TYPE_EQUALS) && ((input[0].type != TOKEN_TYPE_IDENTIFIER) || (inputSize <= 2))) {
        return NULL;
    }
    if (!recursive) { // Initial check for errors
        for (int d = 0; d < inputSize - 1; d++) {       //for each token
            // for repeated tokens return NULL
            if ((input[d].type == TOKEN_TYPE_IDENTIFIER ||input[d].type == TOKEN_TYPE_NUMBER) &&
                (input[d + 1].type == TOKEN_TYPE_IDENTIFIER || input[d + 1].type == TOKEN_TYPE_NUMBER ||
                 strcmp(input[d + 1].name, "(") == 0)) {
                return NULL;
            }
            if (shouldntRepeat(input[d].name) && shouldntRepeat(input[d + 1].name)) {
                return NULL;
            }
            // empty parenthesis
            if (strcmp(input[d].name, "(") == 0 && strcmp(input[d + 1].name, ")") == 0) {
                return NULL;
            }
            if (input[d].type == TOKEN_TYPE_EQUALS && d != 1) { // if equal sign is not in the second index
                return NULL;
            }
        }
    }
    int parenthesisCount = 0;       //count the parenthesis
    while ((*index) < inputSize) {       //for each token
        if (isFunctionOperator(input[(*index)].name)) {     //if it is a function operator
            int innerParenthesisCount = 0;
            // if the end of the input is reached return NULL
            if ((*index) + 1 == inputSize) {
                return NULL;
            }
            if (strcmp(input[(*index) + 1].name, "(") == 0) {   //if next is an open parenthesis
                output[(*index) + 1].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                output[(*index) + 1].name = "(";
                (*output_count)++;    //increase the output count
                parenthesisCount++;
                innerParenthesisCount++;
            } else {   //if next is not an open parenthesis return NULL
                return NULL;
            }

            // determine the index of the operator
            if (recursive) {
                recIndex = *index;
            } else {
                nonRecIndex = *index;
            }

            int j = 2; // the index of the next token after the open parenthesis
            int commaCount = 0;
            while (parenthesisCount != 0) {   //while the parenthesis are not closed
                if ((*index) + j == inputSize) {    //if the end of the input is reached parenthesis are not closed !
                    return NULL;
                }

                    // increase the parenthesis count if an open parenthesis is found
                else if (strcmp(input[(*index) + j].name, "(") == 0) {       //if it is an open parenthesis
                    output[(*index) + j].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
                    output[(*index) + j].name = "(";
                    (*output_count)++;    //increase the output count
                    parenthesisCount++;
                    innerParenthesisCount++;
                    j++;
                }
                    // decrease the parenthesis count if a close parenthesis is found
                else if (strcmp(input[(*index) + j].name, ")") == 0) {      //if it is a close parenthesis
                    output[(*index) + j].type = TOKEN_TYPE_CLOSEPARENTHESIS;    //add it to the output
                    output[(*index) + j].name = ")";
                    (*output_count)++;   //increase the output count
                    parenthesisCount--;
                    innerParenthesisCount--;
                    j++;
                }
                    // if another function operator is found, we go into recursion
                else if (isFunctionOperator(input[(*index) + j].name)) {     //if it is a function operator
                    (*index) += j; //increase the index to for recursion function parameter
                    Token *inner = formatController(input, inputSize, 1, index, output_count);

                    // if there is an error in the inner function return NULL
                    if (inner == NULL) {
                        return NULL;
                    }

                    // re-adjust the index
                    (*index) -= j;

                    // add the new inner function elements to the output
                    for (int k = 0; k < inputSize; k++) {
                        if (inner[k].name !=
                            NULL) { // if the token is empty it's out of inner function scope, pass them
                            output[k] = inner[k];
                        }
                    }

                    //if we have a comma replace it with the operator
                } else if (strcmp(input[(*index) + j].name, ",") == 0) {
                    if (innerParenthesisCount != 1) { // if the comma is not in the first level of parenthesis
                        return NULL;
                    }
                    if (recursive) {
                        output[(*index) + j].type = input[recIndex].type;
                        output[(*index) + j].name = input[recIndex].name;
                        output[(*index) + j].value = input[recIndex].value;

                    } else {
                        output[(*index) + j].type = input[nonRecIndex].type;
                        output[(*index) + j].name = input[nonRecIndex].name;
                        output[(*index) + j].value = input[nonRecIndex].value;

                    }
                    commaCount++;  //increase the comma count
                    (*output_count)++;   //increase the output count
                    //change the stage
                    j++;
                }

                    // else copy whatever we have to output array
                else {
                    output[(*index) + j].type = input[(*index) + j].type;
                    output[(*index) + j].name = input[(*index) + j].name;
                    output[(*index) + j].value = input[(*index) + j].value;
                    (*output_count)++;   //increase the output count
                    j++;
                }
            }

            //if there is not 1 comma inside expression
            if (commaCount != 1) {
                return NULL;
            }
            (*index) += j;    //skip the tokens that are already processed

            //if it is a recursive call, return the output
            if (recursive == 1) {
                return output;
            }

            // if we have comma outside of operator, return NULL
        } else if (strcmp(input[(*index)].name, ",") == 0) {
            return NULL;
        } else if (strcmp(input[(*index)].name, "(") == 0) {   //if it is an open parenthesis
            output[(*index)].type = TOKEN_TYPE_OPENPARENTHESIS;   //add it to the output
            output[(*index)].name = "(";
            output[(*index)].value = input[(*index)].value;
            (*output_count)++;    //increase the output count
            (*index)++;
            parenthesisCount++;
        } else if (strcmp(input[(*index)].name, ")") == 0) {      //if it is a close parenthesis
            output[(*index)].type = TOKEN_TYPE_CLOSEPARENTHESIS;    //add it to the output
            output[(*index)].name = ")";
            output[(*index)].value = input[(*index)].value;
            (*output_count)++;   //increase the output count
            (*index)++;
            parenthesisCount--;
        }
            // else copy whatever we have to output array
        else {
            output[(*index)].type = input[(*index)].type;
            output[(*index)].name = input[(*index)].name;
            output[(*index)].value = input[(*index)].value;
            (*output_count)++;  //increase the output count
            (*index)++;
        }
    }
    if (parenthesisCount != 0) {
        return NULL;
    }
    return output;
}