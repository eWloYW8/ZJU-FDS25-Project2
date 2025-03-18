#include "./monomial.h"
#include "./expressionobj.h"
#include "./polynomial.h"
#include "../ast.h"
#include "../ads/hashtable.h"

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

int monomial_is_equal(Monomial* monomial1, Monomial* monomial2) {
    if (monomial1->coefficient != monomial2->coefficient || monomial1->coefficient_denominator != monomial2->coefficient_denominator) {
        return 0;
    }
    if (monomial1->expressionobjects->size != monomial2->expressionobjects->size) {
        return 0;
    }
    for (unsigned long long i = 0; i < monomial1->expressionobjects->size; ++i) {
        HashTableEntry* entry1 = monomial1->expressionobjects->buckets[i];
        HashTableEntry* entry2 = monomial2->expressionobjects->buckets[i];
        while (entry1 && entry2) {
            if (entry1->hash != entry2->hash || !expression_obj_is_equal(entry1->obj, entry2->obj)) {
                return 0;
            }
            entry1 = entry1->next;
            entry2 = entry2->next;
        }
        if (entry1 || entry2) {
            return 0;
        }
    }
    return 1;
}



