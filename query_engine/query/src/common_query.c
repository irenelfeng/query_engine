/* ========================================================================== */
/* File: query.c 
 *
 * Author: Irene Feng
 * Date: 8/11/15
 *
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Query
 *
 * This is the main source file to run the query engine on indexed files. 
 * 
 * Please see the README for more specifications. 
 *
 */
/* ========================================================================== */
#include <stdio.h>                           // printf
#include <stdlib.h>
#include <string.h> 						//strlen

#include "../../util/libtseutil.h"
#include "query.h"

// #define QUERY_MAX 1000
// #define BUF_MAX 1000000
// #define MAX(x, y) (((x) > (y)) ? (x) : (y))

WordNode * processQuery(char * query, HashTable *index){
	//TODO: eat leftover from the buffer

	if (NULL == strchr(query, '\n')){
		printf("Whoa there. You entered too many characters. Query must exit.");
		return NULL;
    }
	//if there is more than 1000 characters. didn't handle yet. 
	printf("\n");

	// get first word in query
	char * pch = strtok(query," \n");
	if(pch == NULL){
		fprintf(stdout, "No input specified. \n");
		printf("QUERY>: ");
		return init_list();
	}
	
	if ((strcmp("OR", pch) == 0) || (strcmp("AND",pch) == 0)) {
			fprintf(stderr, "Invalid input. AND/OR cannot start your search query. please use a non-operator word to search \n");
			printf("QUERY>: ");
			return NULL; 
	}

	//initialize search results 
	int or = -1; //set to negative one on first run
	WordNode * search_results = NULL;
	WordNode * tmp_list = NULL;

	// go through rest of the query string
	while (pch != NULL)
	{

		//check if OR or AND
		if (strcmp("OR", pch) == 0) {
			// printf("or here");
			or = 1;
			pch = strtok (NULL, " \n");
			continue;
		} else if (strcmp("AND", pch) == 0) {
			// printf("and here");

			or = 0;
			pch = strtok (NULL, " \n");
			continue;
		}

		// switch to lowercase
		NormalizeWord(pch);

		if(or == 1){
			// fprintf(stdout,"Doing OR");
			if (search_results) {
				// if there has been an OR before, now unionize
				search_results = unionize(search_results, tmp_list);

			} else {
				// only the first or
				// need to hold on to previous list
				search_results = tmp_list;
			}

			// set tmp_list to the new list word
			tmp_list = make_copy((WordNode *) get_value(pch ,index));


		} else if (or == 0){
			// printf("Doing AND"); 
			// get intersect 
			tmp_list = intersection(tmp_list, make_copy(get_value(pch ,index))); 

		} else {
			// first run (or = -1)
			tmp_list = make_copy((WordNode *) get_value(pch ,index));
			// if(tmp_list){
			// 	fprintf(stderr, "GOT SOMETHING!!\n");
			// 	if (tmp_list->head)
			// 	fprintf(stderr, "here is something %d", tmp_list->head->docID);
			// }

		}

		or = 0; // "marks AND for next"
		pch = strtok (NULL, " \n");

	} //end search loop

	// in the end, unionize the two lists! 
	search_results = unionize(search_results, tmp_list);

	return search_results;
}

WordNode * unionize(WordNode * tmp_list, WordNode * list_2)
{
	WordNode * unioned = init_list();

	if(!tmp_list && !list_2){
		return unioned; // return empty list
	}

	if(!tmp_list){
		// must make a copy, in case this list_2 is retrieved from the hash table. 
		DocNode * doc = list_2->head;
		while (doc != NULL) {
			// just add doc 
			add_doc(doc->docID , doc->freq , unioned);
			// fprintf(stdout, "%d %d ", doc->docID, doc->freq);
			doc = doc->next; 
		}
		free_list(list_2);
		return unioned;
	}

	if(!list_2){
		// must make a copy
		DocNode * doc = tmp_list->head;
		while (doc != NULL) {
			// just add doc 
			add_doc(doc->docID , doc->freq , unioned);
			// fprintf(stdout, "%d %d ", doc->docID, doc->freq);
			doc = doc->next; 
		}
		free_list(tmp_list);
		return unioned;
	}

	// keep track of shared documents
	int max_seen_size = num_docs(list_2);
	int* have_seen = (int *) calloc(max_seen_size, sizeof(int));
	int i=0;

	//go through every node in tmp_list
	DocNode * doc = tmp_list->head;

	while (doc != NULL) {
		// for each doc node, see if that docNode is present in list_2. 
		DocNode * shared = get_index(doc->docID, list_2);
		
		if(shared){
			//if shared, keep track of this docID. 
			have_seen[i++] = doc->docID;
			// take max frequency 
			int newfreq = MAX((shared->freq),(doc->freq)); 
			//make a new docNode and add this docNode to the unioned list
			add_doc(doc->docID ,newfreq , unioned);
		} else {
			// even if not shared, add to the list. 
			add_doc(doc->docID , doc->freq , unioned);
		}
		// fprintf(stdout, "%d %d ", doc->docID, doc->freq);
		doc = doc->next; 
	}
	
	// now go through every node in list_2
	doc = list_2->head;

	while (doc != NULL) {

		//check if docID has already been added from tmp_list
		int index = find_index(have_seen, max_seen_size, doc->docID);
		
		if (index == -1) {
			// only adds if doc has not been added before
			add_doc(doc->docID , doc->freq , unioned);
		}

		doc = doc->next;
	}

	free_list(tmp_list);
	free_list(list_2);

	return unioned;

}

WordNode * intersection(WordNode * tmp_list, WordNode * list_2)
{
	// make new list
	WordNode * intersect = init_list();
	// no results found for either list - return empty list
	if(!tmp_list){
		return intersect;
	}
	if(!list_2){
		return intersect;
	}
	
	//go through every node in tmp_list
	DocNode * doc = tmp_list->head;

	//go through each of the doc nodes 
	while (doc != NULL) {
		// for each doc node, see if that docNode is present in list_2. 
		DocNode * shared = get_index(doc->docID, list_2);
		if(shared){
			// add the frequencies together 
			int newfreq = (shared->freq) + (doc->freq); 
			//make a new docNode and add this docNode to the intersect list
			add_doc(doc->docID ,newfreq , intersect);
		}
		// fprintf(stdout, "%d %d ", doc->docID, doc->freq);
		doc = doc->next; 
	}

	free_list(tmp_list);
	free_list(list_2);

	return intersect;

}

//make list copy - should not free a list in the hash table.
WordNode * make_copy(WordNode * list)
{
	if (!list){
		return NULL;
	}

	WordNode * copy = init_list();
	
	DocNode * doc = list->head;
	while (doc != NULL) {
		// just add doc 
		add_doc(doc->docID , doc->freq , copy);
		// fprintf(stdout, "%d %d ", doc->docID, doc->freq);
		doc = doc->next; 
	}

	return copy;

}

HashTable *ReadFile(char *file)
{
	FILE * fp;

	if ((fp=fopen(file, "r")) == NULL) {
		fprintf(stderr, "ERROR: Failure to open index file %s", file);
		return NULL;
	}

	HashTable *ht = init_hash();

	char line[BUF_MAX];


    /* Read the entire file line by line. */
   	while (fgets(line, BUF_MAX, fp) != NULL) {


   		//initialize word, numdocs, and wordnode
		char *word; 
		int num_docs = 0;

    	word = strtok(line, " ");

	    num_docs = atoi(strtok(NULL, " "));
	    
	    if( num_docs == 0){
	    	fprintf(stderr, "ERROR: index file incorrect format.");
	    	free_table(ht);
	    	return NULL;
	    }

	    WordNode * list = init_list();

    	//have to load the docs in backwards
    	int backwards[num_docs*2];

    	for(int i=0; i<num_docs; i++){
    		
    		int doc_id = atoi(strtok(NULL, " "));
    		int freq = atoi(strtok(NULL, " "));
    		// get position of docid to be loaded in backwards 
    		int here = (num_docs * 2 - 2) - (2*i);
    		int next = (num_docs * 2 - 1) - (2*i);
    		//load from the back
    		backwards[here] = doc_id;
    		backwards[next] = freq;

    	}

    	//add in the docs using backwards array
    	for(int i = 0; i < num_docs; i++) {

    		add_doc(backwards[2*i], backwards[(2*i)+1], list);

    	}
    	//add the completed WordNode to the hash table
    	add_hash(word, list, ht);
    	
	}

	fclose(fp);

	return ht;
}

int find_index(int a[], int size, int value)
{
   int i;
   for (i=0; i<size; i++)
   {
	 if (a[i] == value)
	 {
	    return(i);  
	 }
   }
   return(-1);  // if not found
}

char * get_url(char * docName, char * dir)
{
	char * file = (char *) calloc(strlen(dir)+7,sizeof(char));

	sprintf(file, "%s/%s", dir, docName);

	FILE *fp;

	if ((fp=fopen(file, "r")) == NULL) {
    	fprintf(stderr, "Opening file %s failed", file);
    	free(file);
    	return NULL;
	}

	char line[BUF_MAX];
	char * url;
	if (fgets(line, BUF_MAX, fp) != NULL) {
		//get first line, which is the url and return it
		url = malloc(strlen(line)+1*sizeof(char));
		strcpy(url, line);

	} else {
		fprintf(stderr, "Error reading line in file %s", file);
		free(file);
		fclose(fp);
		return NULL;
	}

	free(file);
	fclose(fp);

	return url;
	
}