#include "./hashtable.h"

#include <stdlib.h>

extern unsigned long long expression_obj_get_hash(struct ExpressionObj* obj);
extern int expression_obj_is_equal(struct ExpressionObj* obj1, struct ExpressionObj* obj2);

HashTable* create_hash_table(unsigned long long size) {
    HashTable *table = malloc(sizeof(HashTable));
    table->buckets = malloc(size * sizeof(HashTableEntry*));
    for (unsigned long long i = 0; i < size; i++) {
        table->buckets[i] = NULL;
    }
    table->size = size;
    table->count = 0;
    return table;
}

void free_hash_table(HashTable* table) {
    for (unsigned long long i = 0; i < table->size; i++) {
        HashTableEntry* entry = table->buckets[i];
        while (entry != NULL) {
            HashTableEntry* temp = entry;
            entry = entry->next;
            free(temp->obj);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

HashNode* insert_hash_table(HashTable* table, struct ExpressionObj* obj) {
    unsigned long long hash = expression_obj_get_hash(obj) % table->size;
    HashTableEntry* entry = malloc(sizeof(HashTableEntry));
    entry->hash = hash;
    entry->obj = malloc(sizeof(struct HashNode));
    entry->obj->key = obj;
    entry->obj->value = 1;
    entry->next = table->buckets[hash];
    table->buckets[hash] = entry;
    table->count++;
    return entry->obj;
}

HashNode* find_hash_table(HashTable* table, struct ExpressionObj* obj) {
    unsigned long long hash = expression_obj_get_hash(obj) % table->size;
    HashTableEntry* entry = table->buckets[hash];
    while (entry != NULL) {
        if (expression_obj_is_equal(entry->obj->key, obj)) {
            return entry->obj;
        }
        entry = entry->next;
    }
    return NULL;
}

void remove_hash_table(HashTable* table, struct ExpressionObj* obj) {
    unsigned long long hash = expression_obj_get_hash(obj) % table->size;
    HashTableEntry* entry = table->buckets[hash];
    HashTableEntry* prev = NULL;
    while (entry != NULL) {
        if (expression_obj_is_equal(entry->obj->key, obj)) {
            if (prev == NULL) {
                table->buckets[hash] = entry->next;
            } else {
                prev->next = entry->next;
            }
            table->count--;
            free(entry->obj);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

unsigned long long count_hash_table(HashTable* table) {
    return table->count;
}

HashNode* getone_in_hash_table(HashTable* table) {
    for (unsigned long long i = 0; i < table->size; i++) {
        HashTableEntry* entry = table->buckets[i];
        if (entry != NULL) {
            return entry->obj;
        }
    }
    return NULL;
}