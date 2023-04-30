#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "tokenizer.h"

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
    FILE *intermediate;
    char* line = read_last_line("intermediate.ll");
    printf("%s", line);
    char* split = strtok(line, "=");
    printf("%s", split);
    free(line);
    return 0;
}