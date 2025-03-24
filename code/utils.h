// This file contains the utility functions used in the code.
#pragma once

// remove_blankspace removes all the blank spaces from a string.
void remove_blankspace(char *str);

// fastpow calculates the power of a base raised to an exponent using fast exponentiation.
long long fastpow(long long base, long long exp);

// gcd calculates the greatest common divisor of two numbers using the Euclidean algorithm.
long long gcd(long long a, long long b);

// Modify the expression string: if the character '-' is found in the first position or after a '(',
// it is replaced with '0-' to ensure correct parsing of negative numbers.
char *expadd0(char *str);