// use this header file to apply testing

#ifndef QUERY_H
#define QUERY_H


#define QUERY_MAX 1000
#define BUF_MAX 1000000
#define MAX(x, y) (((x) > (y)) ? (x) : (y))


HashTable *ReadFile(char *file);

// processes a query, returning the list of documents relevant to the query.
WordNode * processQuery(char * query, HashTable *index);

//union two lists, return the resulting list
WordNode * unionize(WordNode * tmp_list, WordNode * list_2);
// intersect two lists, return the resulting list
WordNode * intersection(WordNode * tmp_list, WordNode * list_2);

// returns index of an element value in an array a[]. Returns -1 if the value does not exist in list.
int find_index(int a[], int size, int value);

// make a copy of a list to a new place in memory 
WordNode * make_copy(WordNode * list);

char * get_url(char * docName, char * dir);

#endif // QUERY_H