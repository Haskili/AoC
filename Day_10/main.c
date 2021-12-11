#include <stdio.h>
#include <string.h>

#define MAX_LINE 5000

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

            // Check if 'tail' is another closer, 
            // which would automatically fail 
            // since we're expecting an opener
            if (closer(tail))
                return -1;

            // Check if the the 'head' matches
            // 'tail', returning the appropriate 
            // score for the error if it doesn't
            else if (head == ')' && tail != '(')
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

int syntax(char* fname) {
    int tally = 0, idx = 0;
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
            int status = validate(line, idx);
            if (status >= 0)
                tally += status;

            // Exit if this was the last line in the file
            if (c == EOF)
                break;

            // Clear the line and reset the index
            memset(line, 0, idx);
            idx = 0;
        }

        // Valid chunk data, append and continue iteration
        else if (closer(c) || opener(c))
            line[idx++] = c;

        // Invalid character at index 'idx' of current line;
        // Ignore and continue iteration
        else
            continue;
    }

    // Close the file and end the program
    fclose(file);
    return tally;
}

int main(int argc, char* argv[]) {
    printf("Final Score: %i\n", syntax("./test_02.in"));
    return 0;
}