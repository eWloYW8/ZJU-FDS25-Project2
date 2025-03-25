#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to remove all spaces from a given string
void remove_blankspace(char *str){
    int count = 0;
    for (int i = 0; str[i]; i++){
        if (str[i] != ' '){
            str[count++] = str[i]; // Copy non-space characters to the front
        }
    }
    str[count] = '\0'; // Null-terminate the modified string
}

// Function to compute base raised to the power exp using fast exponentiation
long long fastpow(long long base, long long exp){
    long long result = 1;
    while (exp > 0){
        if (exp % 2 == 1){
            result *= base; // Multiply result by base if exp is odd
        }
        base *= base; // Square the base
        exp /= 2; // Divide exp by 2
    }
    return result;
}

// Function to compute the greatest common divisor (GCD) of two numbers
long long gcd(long long a, long long b){
    while (b != 0){
        long long t = b;
        b = a % b; // Update b to be the remainder of a divided by b
        a = t; // Update a to be the old value of b
    }
    return a; // Return the GCD
}

// Modify the expression string: if the character '-' is found in the first position or after a '(',
// it is replaced with '0-' to ensure correct parsing of negative numbers.
char *expadd0(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '-' && (i == 0 || str[i - 1] == '(')) {
            // If '-' is found at the beginning or after '(', replace it with '0-'
            char *new_str = (char *)malloc(strlen(str) + 3); // Allocate memory for new string
            strncpy(new_str, str, i); // Copy the part before '-'
            new_str[i] = '0'; // Add '0' before '-'
            new_str[i + 1] = '-'; // Add '-'
            strcpy(new_str + i + 2, str + i + 1); // Copy the rest of the string
            new_str[strlen(str) + 2] = '\0'; // Null-terminate the new string
            free(str); // Free the old string
            str = new_str; // Update str to point to the new string
        }
        i++;
    }
    return str; // Return the modified string
}

// When encountering these cases, add a '*' to the expression string to ensure correct parsing:
// 1. A number followed by an opening parenthesis, e.g., "5(".
// 2. A closing parenthesis followed by a variable, e.g., ")x".
// 3. A number followed by a variable, e.g., "5x".
char *expadd_multiply(char *str) {
    int len = strlen(str);
    char *new_str = (char *)malloc(2 * len + 1); // Allocate memory for new string
    int j = 0;
    for (int i = 0; i < len; i++) {
        new_str[j++] = str[i]; // Copy the current character
        if (i < len - 1) {
            if ((isdigit(str[i]) && str[i + 1] == '(') || // Case 1
                (str[i] == ')' && isalpha(str[i + 1])) || // Case 2
                (isdigit(str[i]) && isalpha(str[i + 1]))) { // Case 3
                new_str[j++] = '*'; // Add '*' after the current character
            }
        }
    }
    new_str[j] = '\0'; // Null-terminate the new string
    free(str); // Free the old string
    return new_str; // Return the modified string
}