#include "./hashtable.h"
#include "../simplificator/expressionobj.h"
#include "../simplificator/monomial.h"
#include "../simplificator/polynomial.h"

#include <stdlib.h>
#include <string.h>

HashTable* create_hash_table(unsigned long long size, enum HashNodeType type) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->buckets = (HashTableEntry**)malloc(sizeof(HashTableEntry*) * size);
    table->size = size;
    table->type = type;
    for (unsigned long long i = 0; i < size; ++i) {
        table->buckets[i] = NULL;
    }
    return table;
}

void free_hash_table(HashTable* table) {
    for (unsigned long long i = 0; i < table->size; ++i) {
        HashTableEntry* entry = table->buckets[i];
        while (entry) {
            HashTableEntry* next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(table->buckets);
    free(table);
}

void insert(HashTable* table, struct HashNode* obj) {
    unsigned long long hash = 0;
    if (table->type == HASH_NODE_MONOMIAL) {
        hash = expression_obj_get_hash(obj->data.monomialnode->coefficient);
    } else if (table->type == HASH_NODE_POLYNOMIAL) {
        hash = expression_obj_get_hash(obj->data.polynomialnode->data);
    }
    unsigned long long index = hash % table->size;
    HashTableEntry* entry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    entry->hash = hash;
    entry->obj = obj;
    entry->next = table->buckets[index];
    table->buckets[index] = entry;
}