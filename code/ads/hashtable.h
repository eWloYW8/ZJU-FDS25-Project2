#pragma once

struct ExpressionObj;

struct HashNode {
    struct ExpressionObj* key;
    long long value;
};

typedef struct HashTableEntry {
    unsigned long long hash;
    struct HashNode* obj;
    struct HashTableEntry* next;
} HashTableEntry;

typedef struct {
    HashTableEntry** buckets;
    unsigned long long size;
} HashTable;

HashTable* create_hash_table(unsigned long long size);

extern unsigned long long expression_obj_get_hash(struct ExpressionObj* obj);

void free_hash_table(HashTable* table);

void insert(HashTable* table, struct HashNode* obj);

struct HashNode* find(HashTable* table, struct HashNode* obj);

void remove(HashTable* table, struct HashNode* obj);