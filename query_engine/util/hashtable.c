/* ========================================================================== */
/* File: hashtable.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Crawler
 *
 * Author: Irene Feng
 * Date: Saturday July 25 2015
 *
 * Hashtable functions
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen
#include <stdio.h>
#include <stdlib.h>

// ---------------- Local includes  e.g., "file.h"
#include "hashtable.h"                       // hasht->ble functionality
#include "common_index.h"

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

HashTable *init_hash(){

    HashTable *ht = calloc(1,sizeof(HashTable));
    return ht;
}

unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    size_t len = strlen(str);
    unsigned long hash, i;

    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}

//returns 1 if key is in unique, 0 if not unique (key already in the hashtable)
int unique_hash(char *key, HashTable *ht)
{
    //first check if key exists
    if(!key){
        return 0;
    }

    unsigned long hash_num = JenkinsHash(key, MAX_HASH_SLOT);
    HashTableNode *n = ht->table[hash_num];
    //if the pointer does not point to anything
    if (n == NULL) {
        //key is unique because no key in the hash!

        return 1; 
    }
    //else, the pointer points to a list of hashTableNodes
    else {
        //search for the key in the linked list 
        while(n != NULL){
            if(strcmp(n->key, key) == 0){
                //key is not unique

                return 0;
            }
            n = n->next;
        }
        //if none ofthe hash table nodes match the key, key is unique.
        return 1;

    }

}

void * get_value(char *key, HashTable *ht)
{

    unsigned long hash_num = JenkinsHash(key, MAX_HASH_SLOT);
    
    if(unique_hash(key, ht)){
        // printf("key %s not found", key);
        return NULL;
    } else {
        HashTableNode *n = ht->table[hash_num];
        while(n != NULL){
            if(strcmp(n->key, key) == 0){

                //return the value at the key
                return n->value;
            }
            n = n->next;
        }
    }

    return NULL; //should not be reached

}

// adds a hash table node to the hash table
//returns 0 if error, 1 if success
int add_hash(char *key, void * value, HashTable *ht)
{
    //check empty key
    unsigned long hash_num = JenkinsHash(key, MAX_HASH_SLOT);
    // printf("hash slot of %s is %lu\n", key, hash_num);

    //create node to be added
    HashTableNode *n = (HashTableNode *) calloc(1, sizeof(HashTableNode));
    //allocate space for key
    n->key = (char *) calloc(strlen(key)+1, sizeof(char));
    strcpy(n->key, key);

    //set value to the value
    n->value = value;
    n->next = NULL;
    
    // if empty 
    if((ht->table[hash_num]) == NULL){
        //just add HashTableNode
        ht->table[hash_num] = n;
        return 1;
    } else {
        //search until the end of the list
        HashTableNode *end = ht->table[hash_num];
        // printf("a collision!\n");

        while(end->next != NULL){
            end = end->next;
        }
        //make end's next the new node
        end->next = n;

        return 1;

    }

    //should not reach
    return 0;


}

//frees every hash table node in the hash table
int free_table(HashTable *ht)
{
    //march through hash table and free any hash table nodes.
    for(int i=0; i < MAX_HASH_SLOT; i++){
        if((ht->table[i]) == NULL){
            continue;
        } else {

            //free all hash tables in the list.
            HashTableNode *end = ht->table[i];
            HashTableNode *tmp;

            while(end != NULL){
                tmp = end->next;
                free(end->key);
                end->key = NULL;
                free_list(end->value); //TODO: this is not general. so it will not work for Lab4.
                free(end);
                end = tmp;
            }
        }

    }

    free(ht);
    return 1;
}