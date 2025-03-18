void remove_blankspace(char *str){
    int count = 0;
    for (int i = 0; str[i]; i++){
        if (str[i] != ' '){
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

long long longlong_pow(long long base, int exponent){
    long long result = 1;
    while (exponent > 0){
        if (exponent % 2 == 1){
            result *= base;
        }
        exponent >>= 1;
        base *= base;
    }
}

long long gcd(long long a, long long b){
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    if (a == 0) return b;
    return gcd(b % a, a);
}