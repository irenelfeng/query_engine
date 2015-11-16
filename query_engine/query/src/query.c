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
// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables
// #define QUERY_MAX 10
// #define BUF_MAX 1000000
// #define MAX(x, y) (((x) > (y)) ? (x) : (y)) // all these constants are defined in query.h header file 
 /* ========================================================================== */

int main(int argc, char * argv[])
{
	if (argc == 3) {

		//check if 1st arg is file
		if (!IsFile(argv[1])) {
			fprintf(stderr, "ERROR: Sorry, %s is not a file.", argv[1]);
			return 1;
		}
		//check if 2nd arg is directory
		if (!IsDir(argv[2])) {
			fprintf(stderr, "ERROR: Sorry, %s is not a directory.", argv[2]);
			return 1;
		}

		// make indexer
		HashTable * index = ReadFile(argv[1]);
		if (index == NULL){
			return 1;
		}

		char buffer[QUERY_MAX];

		printf("QUERY>: ");
		while( fgets(buffer, QUERY_MAX , stdin) != NULL ) /* break with ^D or ^Z */
		{ 
			WordNode * search_results = processQuery(buffer, index);

			if (!search_results) {
				// an error that must exit. 
				return 1;
			}
			if (search_results->head == NULL) {
				printf("No results found.");
			
			} else {
				// order by frequency, mergesort returns the head (top-ranking url) of search_results
				DocNode * doc = mergesort(search_results->head);
				//set the search_results head to the doc, for freeing purposes. 
				search_results->head = doc; 
				// go through every doc node
				while ( doc!= NULL){
					char * docName = (char *)calloc(5,sizeof(char)); //assuming
					int ret = snprintf(docName, 5, "%d", doc->docID);
					if (ret == -1){
						fprintf(stderr, "WOW you have tons of files. I cannot handle this. Sorry \n");
						free(docName);
						continue;
					}

					char * url = get_url(docName, argv[2]);
					if (!url){
						fprintf(stderr, "Getting the url of doc %s failed. \n", docName);
						continue;
					}

					printf("Document ID: %d URL: %s", doc->docID, url);
					free(url);
					free(docName);

					doc = doc->next;
				}

			}

			//next iteration
			printf("\nQUERY>: ");
			free_list(search_results);
		}

		printf("done");

		free_table(index);
		return 0;

	} else {

		fprintf(stderr,"ERROR: Sorry, you entered in an incorrect number of input arguments. You must have 2 (index filename to read from, directory of html files)");
		return 1;
	}
}