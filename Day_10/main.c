#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1000

int opener(char c) {
    return (c == '(' || c == '[' ||  c == '{' ||  c == '<');
}

int closer(char c) {
    return (c == ')' || c == ']' || c == '}' ||  c == '>');
}

int validate(char* line, int sz) {
    char stack[MAX_LINE];

    // Iterate through all 'i' characters of 'line'
    for (int i = 0, j = 0; i < sz; i++) {
        char head = line[i];

        // If the 'head' is an opener, 
        // push it onto the stack and 
        // continue iterating
        if (opener(head))
            stack[j++] = head;

        // Else if it's a closer, we need
        // to make sure the last thing on
        // the stack is a matching opener
        else if (closer(head)) {
            char tail = stack[j - 1];

            // Check if the the 'head' matches
            // 'tail', returning the appropriate 
            // score for the error if it doesn't
            if (head == ')' && tail != '(')
                return 3;

            else if (head == ']' && tail != '[')
                return 57;

            else if (head == '}' && tail != '{')
                return 1197;

            else if (head == '>' && tail != '<')
                return 25137;

            // The 'tail' and 'head' matched, 
            // decrement the stack index and 
            // continue iterating
            else
                stack[j--] = '\0';
        }
    }
    return 0;
}

unsigned long complete(char* line, int sz) {

    // Allocate for the stack and tally
    char stack[MAX_LINE];
    int i = 0, j = 0;
    unsigned long tally = 0;

    // Iterate through all 'i' characters of 'line'
    for (; i < sz; i++) {
        char head = line[i];
        if (opener(head))
            stack[j++] = head;

        else if (closer(head)) {
            char tail = stack[j - 1];
            stack[j--] = '\0';
        }
    }

    // Iterate through the stack
    for (int k = (j-1); k >= 0; k--) {
        if (stack[k] == '(')
            tally = tally*5 + 1;

        else if (stack[k] == '[')
            tally = tally*5 + 2;

        else if (stack[k] == '{')
            tally = tally*5 + 3;

        else if (stack[k] == '<')
            tally = tally*5 + 4;
    }
    return tally;
}

int invalid(char* line, int sz) {

    // Else we need to allocate for the stack
    char stack[MAX_LINE];
    int i = 0, j = 0;

    // Iterate through all 'i' characters of 'line'
    for (/**/; i < sz; i++) {
        char head = line[i];

        // If the 'head' is an opener, 
        // push it onto the stack and 
        // continue iterating
        if (opener(head))
            stack[j++] = head;

        // Else if it's a closer, we need
        // to make sure the last thing on
        // the stack is a matching opener
        else if (closer(head)) {
            char tail = stack[j - 1];

            // Check if the the 'head' matches
            // 'tail', returning the appropriate 
            // score for the error if it doesn't
            if (head == ')' && tail != '(')
                return 1;

            else if (head == ']' && tail != '[')
                return 1;

            else if (head == '}' && tail != '{')
                return 1;

            else if (head == '>' && tail != '<')
                return 1;

            // The 'tail' and 'head' matched, 
            // decrement the stack index and 
            // continue iterating
            else
                stack[j--] = '\0';
        }
    }
    return 0;
}

void sort(unsigned long* tally, int sz) {
    unsigned long tmp;

    for (int i = 0, idx = 0, j = 0; i < (sz - 1); i++) {
        idx = i;
        for (j = (i + 1); j < sz; j++)
            if (tally[j] < tally[idx])
                idx = j;

        tmp = tally[i];
        tally[i] = tally[idx], tally[idx] = tmp;
    }
}

unsigned long middle(unsigned long* tally, int sz) {

    // Sort the results using a
    // basic Selection Sort
    sort(tally, sz);

    // Return the middle result
    return tally[sz / 2];
}

unsigned long syntax(char* fname) {
    unsigned long* tally = (unsigned long*)malloc(MAX_LINE);
    int sz = 0, index = 0;
    char line[MAX_LINE];

    // Access the file
    FILE* file = fopen(fname, "r");
    if (file == NULL)
        return -1;

    // Read the file
    for (char c = ' '; /**/; /**/) {
        c = fgetc(file);

        // End of line, check the validity of the line
        if (c == '\n' || c == '\r' || c == EOF) {

            // If  the line wasn't invalid, 
            // find the completion score
            if (!invalid(line, sz))
                tally[index++] = complete(line, sz);

            // Exit if this was the last line 
            // in the file to parse
            if (c == EOF)
                break;

            // Else, clear the line and reset 
            // the size index
            memset(line, 0, sz);
            sz = 0;
        }

        // Valid chunk data, append and continue iteration
        else if (closer(c) || opener(c))
            line[sz++] = c, line[sz + 1] = '\0';

        // Invalid character at index 'sz' of current line;
        // Ignore and continue iteration
        else
            continue;
    }

    // Close the file and return the middle of the sorted results
    fclose(file);
    return middle(tally, index);
}

int main(int argc, char* argv[]) {
    printf("Final Score: %lu\n", syntax("./test_05.in"));
    return 0;
}
