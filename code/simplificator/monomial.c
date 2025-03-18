#include "./monomial.h"
#include "./expressionobj.h"
#include "./polynomial.h"
#include "../ast.h"
#include "../ads/hashtable.h"

#include <stdlib.h>

Monomial* create_monomial() {
    Monomial* monomial = (Monomial*)malloc(sizeof(Monomial));
    monomial->coefficient = 1;
    monomial->coefficient_denominator = 1;
    monomial->expressionobjects = create_hash_table(MONOMIAL_SIZE);
    return monomial;
}

void free_monomial(Monomial* monomial) {
    free_hash_table(monomial->expressionobjects);
    free(monomial);
}

Monomial* deep_copy_monomial(Monomial* monomial) {
    Monomial* new_monomial = create_monomial();
    new_monomial->coefficient = monomial->coefficient;
    new_monomial->coefficient_denominator = monomial->coefficient_denominator;
    for (unsigned long long i = 0; i < monomial->expressionobjects->size; ++i) {
        HashTableEntry* entry = monomial->expressionobjects->buckets[i];
        while (entry) {
            insert(new_monomial->expressionobjects, entry->obj);
            entry = entry->next;
        }
    }
    return new_monomial;
}





