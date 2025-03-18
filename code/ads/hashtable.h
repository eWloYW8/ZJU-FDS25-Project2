#pragma once

enum HashNodeType {
    HASH_NODE_MONOMIAL,
    HASH_NODE_POLYNOMIAL,
};

struct HashNode {
    enum HashNodeType type;
    union {
        struct PolynomialNode* polynomialnode;
        struct MonomialNode* monomialnode;
    } data;
};

typedef struct HashTableEntry {
    long long hash;
    struct HashNode* obj;
    struct HashTableEntry* next;
} HashTableEntry;

typedef struct {
    enum HashNodeType type;
    HashTableEntry** buckets;
    unsigned long long size;
} HashTable;

HashTable* create_hash_table(unsigned long long size, enum HashNodeType type);

void free_hash_table(HashTable* table);

void insert(HashTable* table, struct HashNode* obj);

struct HashNode* find(HashTable* table, struct HashNode* obj);

void remove(HashTable* table, struct HashNode* obj);