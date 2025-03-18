#pragma once

struct ExpressionObj;

typedef struct HashNode {
    struct ExpressionObj* key;
    long long value;
} HashNode;

typedef struct HashTableEntry {
    unsigned long long hash;
    struct HashNode* obj;
    struct HashTableEntry* next;
} HashTableEntry;

typedef struct {
    HashTableEntry** buckets;
    unsigned long long size;
    unsigned long long count;
} HashTable;

HashTable* create_hash_table(unsigned long long size);

void free_hash_table(HashTable* table);

HashNode* insert_hash_table(HashTable* table, struct ExpressionObj* obj);

HashNode* find_hash_table(HashTable* table, struct ExpressionObj* obj);

void remove_hash_table(HashTable* table, struct ExpressionObj* obj);

int is_empty_hash_table(HashTable* table);

int is_only_one_hash_table(HashTable* table);

int count_hash_table(HashTable* table);

HashNode* getone_in_hash_table(HashTable* table);