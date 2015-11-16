/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * This file contains the definitions and functions for a hashtable of urls.
 * Checks for unique url in the hashtable, adds the hashtable, and frees the memory in the hashtable.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants
#define MAX_HASH_SLOT 10000                  // number of "buckets"

// ---------------- Structures/Types
 #include "common_index.h"

typedef struct HashTableNode {
    char *key;                               // key 
    void *value;
    struct HashTableNode *next;              // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
} HashTable;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */

unsigned long JenkinsHash(const char *str, unsigned long mod);

HashTable *init_hash();

//returns 1 if key is in unique, 0 if not unique (key already in the hashtable)
int unique_hash(char *key, HashTable *ht);

void * get_value(char *key, HashTable *ht);

// adds a hash table node to the hash table
//returns 0 if error, 1 if success
int add_hash(char *key, void * value, HashTable *ht);

int free_table(HashTable *ht);


#endif // HASHTABLE_H
