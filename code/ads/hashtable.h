#pragma once

#include <stdlib.h>
#include <string.h>

struct ExpressionObj;

typedef struct HashTableEntry {
    long long hash;
    struct ExpressionObj* obj;
    struct HashTableEntry* next;
} HashTableEntry;

typedef struct {
    HashTableEntry** buckets;
    unsigned long long size;
} HashTable;

HashTable* create_hash_table(unsigned long long size);

void free_hash_table(HashTable* table);

void insert(HashTable* table, struct ExpressionObj* obj);

struct ExpressionObj* find(HashTable* table, struct ExpressionObj* obj);

void remove(HashTable* table, struct ExpressionObj* obj);