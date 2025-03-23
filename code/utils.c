void remove_blankspace(char *str){
    int count = 0;
    for (int i = 0; str[i]; i++){
        if (str[i] != ' '){
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

long long gcd(long long a, long long b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

long long lcm(long long a, long long b) {
    return (a / gcd(a, b)) * b;
}