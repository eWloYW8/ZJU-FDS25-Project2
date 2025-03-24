void remove_blankspace(char *str){
    int count = 0;
    for (int i = 0; str[i]; i++){
        if (str[i] != ' '){
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

long long fastpow(long long base, long long exp){
    long long result = 1;
    while (exp > 0){
        if (exp % 2 == 1){
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

long long gcd(long long a, long long b){
    while (b != 0){
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}