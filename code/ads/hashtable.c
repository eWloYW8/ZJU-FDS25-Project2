#include "./hashtable.h"
#include "../simplificator/ExpressionObj.h"


HashTable* create_hash_table(unsigned long long size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->buckets = (HashTableEntry**)calloc(size, sizeof(HashTableEntry*));
    table->size = size;
    return table;
}

void free_hash_table(HashTable* table) {
    for (unsigned long long i = 0; i < table->size; ++i) {
        HashTableEntry* entry = table->buckets[i];
        while (entry) {
            HashTableEntry* next = entry->next;
            free_expression_obj(entry->obj);
            free(entry);
            entry = next;
        }
    }
    free(table->buckets);
    free(table);
}

void insert(HashTable* table, struct ExpressionObj* obj) {
    long long hash = expression_obj_get_hash(obj);
    unsigned long long index = hash % table->size;
    HashTableEntry* new_entry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    new_entry->hash = hash;
    new_entry->obj = deep_copy_expression_obj(obj);
    new_entry->next = table->buckets[index];
    table->buckets[index] = new_entry;
}

struct ExpressionObj* find(HashTable* table, struct ExpressionObj* obj) {
    long long hash = expression_obj_get_hash(obj);
    unsigned long long index = hash % table->size;
    HashTableEntry* entry = table->buckets[index];
    while (entry) {
        if (entry->hash == hash && expression_obj_is_equal(entry->obj, obj)) {
            return entry->obj;
        }
        entry = entry->next;
    }
    return NULL;
}

void remove(HashTable* table, struct ExpressionObj* obj) {
    long long hash = expression_obj_get_hash(obj);
    unsigned long long index = hash % table->size;
    HashTableEntry* entry = table->buckets[index];
    HashTableEntry* prev = NULL;
    while (entry) {
        if (entry->hash == hash && expression_obj_is_equal(entry->obj, obj)) {
            if (prev) {
                prev->next = entry->next;
            } else {
                table->buckets[index] = entry->next;
            }
            free_expression_obj(entry->obj);
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}