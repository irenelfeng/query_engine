/* ========================================================================== */
/* File: common_index.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Indexer
 *
 * This file contains common functions for the indexer component of the TSE. 
 *
 */
/* ========================================================================== */

#ifndef COMMON_INDEX_H
#define COMMON_INDEX_H

// ---------------- Structures/Types

typedef struct DocNode {
	int docID;
	int freq;
	struct DocNode *next;              // pointer to next node
} DocNode;

typedef struct WordNode {
	DocNode *head; 
} WordNode;

//WordNode created dynamically
WordNode * init_list();

//gets the number of docs of a certain word
int num_docs(WordNode * list);

//adds a doc to an existing WordNode list
int add_doc(int ID, int freq, WordNode* list);

//checks if a word has been seen before in the document. 
//returns 1 if the word has been seen before, returns 0 if not. this means the a new docNode needs to be created for this word. 
int in_doc(int ID, WordNode *list);

// frees list
int free_list(WordNode* list);

// returns DocNode that has this ID in the list. returns NULL if not present in list. 
DocNode* get_index(int ID, WordNode * list);

// merge sort code, borrowed from
// http://c.happycodings.com/sorting-searching/code10.html
// returns the head of the list. 
DocNode *mergesort(DocNode *head);

DocNode *merge(DocNode *head_one, DocNode *head_two);

#endif // COMMON_INDEX_H
