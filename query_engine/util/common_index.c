/* ========================================================================== */
/* File: common_index.c
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Indexer
 *
 * Author: Irene Feng
 * Date: Sunday August 9, 2015
 *
 * Common functions dealing with Indexer: for WordNodes and DocNodes.
 */
/* ========================================================================== */

#include <stdio.h>
#include <stdlib.h>

// #include "common_index.h"
// #include "hashtable.h"
#include "../util/libtseutil.h"

WordNode * init_list()
{
	WordNode * doc_list = malloc(sizeof(WordNode));
	if(!doc_list){
		fprintf(stderr, "malloc for a new doc list for a new word failed");
		return NULL;
	}
	doc_list->head = NULL;

	return doc_list;
}

int num_docs(WordNode *list)
{
	int num = 0;
	if (!list){
		return -1;
	}

	//size 0
	if(list->head == NULL){
		return 0;
	} else {
		DocNode * n = list->head;
		while(n != NULL){
			num++;
			n = n->next;
		}
		return num;
	}
}

//return 0 for failure
int add_doc(int ID, int freq, WordNode* list)
{
	if(!list){
		return 0;
	}
	//create list node to be added
	DocNode *n = (DocNode *) calloc(1, sizeof(DocNode));
	if (!n) {
		fprintf(stderr, "malloc failed for adding doc failed.");
		return 0;

	}
    n->docID = ID;
    n->freq = freq;

    if (list->head == NULL){
    	//first document
    	n->next = NULL;
    	list->head = n;
    } else {
    	//add DocNode to the HEAD of the list for easy access when viewing the doc.
    	n->next = list->head;
    	list->head = n;
    }
    return 1;

}

//if a given word has been seen in the current document already.
//returns -1 for error, 0 if not in the current document, 1 if in the current document
int in_doc(int ID, WordNode *list){
	
	if(!list){
		return -1;
	}

	if(list->head->docID == ID){
		return 1;
	} else {
		return 0;
	}
}

int free_list(WordNode* list){
	
	if(!list){
		return -1;
	}

	if(list->head == NULL){
		free(list);
		return 1; 
	} else {

		DocNode * n = list->head;
		DocNode * tmp;
		while(n != NULL){
			tmp = n->next;
			free(n);
			n = tmp;
		}

		free(list);

		return 1;
	}

}

DocNode *get_index(int ID, WordNode *list)
{
	if(!list){
		return NULL;
	}

	if(list->head == NULL){
		return NULL; 
	} else {
		
		DocNode * n = list->head;
		while(n != NULL){
			if (n->docID == ID){
				return n;
			}
			n = n->next;
		}
	}

	// if passing through all of them, and find nothing
	return NULL;

}

// merge sort code, borrowed from
// http://c.happycodings.com/sorting-searching/code10.html
// returns the head of the list. 
DocNode *mergesort(DocNode *head)
{
	DocNode *head_one;
	DocNode *head_two;

	if((head == NULL) || (head->next == NULL)) 
	return head;

	head_one = head;
	head_two = head->next;
	while((head_two != NULL) && (head_two->next != NULL)) {
		head = head->next;
		head_two = head->next->next;
	}
	head_two = head->next;
	head->next = NULL;

	return merge(mergesort(head_one), mergesort(head_two));
}

/* merge the lists.. */ 
// http://c.happycodings.com/sorting-searching/code10.html
DocNode *merge(DocNode *head_one, DocNode *head_two)
{
	DocNode *head_three;

	if(head_one == NULL) 
		return head_two;

	if(head_two == NULL) 
		return head_one;

	if(head_one->freq > head_two->freq) {
		// make head_one in front of the current list 
		head_three = head_one;
		head_three->next = merge(head_one->next, head_two);
	} else {
		// make head_two in the front of the current list
		head_three = head_two;
		head_three->next = merge(head_one, head_two->next);
	}

	return head_three;
}


