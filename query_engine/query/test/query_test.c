// Filename: Test cases for query.h/.c
// Description: A unit test for query engine 
// 
// Author: Irene Feng, test spec design from Professor Xia. 
//
// Test Harness Spec:
// ------------------
//
//  It uses these files but they are not unit tested in this test harness:
//
//  It tests the following functions:
//  WordNode * intersect(WordNode *tmp_list, WordNode *list_2);
//  WordNode * unionize(WordNode *tmp_list, WordNode *list_2);
//  DocNode * mergesort(DocNode * head);
//  int find_index(int a[], int size, int value);
//  WordNode * processQuery(char * query, HashTable *index);
// 
//  If any of the tests fail it prints status 
//  If all tests pass it prints status.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../util/hashtable.h"
#include "../../util/web.h"
#include "../../util/common_index.h"
#include "../../util/file.h"
#include "../src/query.h"


// each test should start by setting the result count to zero

#define START_TEST_CASE  int rs=0

// check a condition and if false print the test condition failed
// e.g., SHOULD_BE(dict->start == NULL)

#define SHOULD_BE(x) if (!(x))  {rs=rs+1; \
    printf("Line %d Fails\n", __LINE__); \
  }


// return the result count at the end of a test

#define END_TEST_CASE return rs

//
// general macro for running a best
// e.g., RUN_TEST(TestDAdd1, "DAdd Test case 1");
// translates to:
// if (!TestDAdd1()) {
//     printf("Test %s passed\n","DAdd Test case 1");
// } else { 
//     printf("Test %s failed\n", "DAdd Test case 1");
//     cnt = cnt +1;
// }
//


#define RUN_TEST(x, y) if (!x()) {              \
    printf("Test %s passed\n", y);              \
} else {                                        \
    printf("Test %s failed\n", y);              \
    cnt = cnt + 1;                              \
}


// test data set, loaded from index_test.dat
HashTable* i;
// test links
WordNode * tmp;
WordNode * final;

//test int array
int a[5] = { 1, 2, 3, 4, 5} ;

// simple test, intersecting one list into final_link
int TestINTERSECT1() {
  START_TEST_CASE;
  tmp = init_list();
  add_doc(1, 2, tmp);
  final = NULL;
  final = intersection(tmp, final); // an empty list
  SHOULD_BE(final->head == NULL);
  free_list(tmp);
  free_list(final);
  END_TEST_CASE;
}

// intersecting two lists 
int TestINTERSECT2() {
  START_TEST_CASE;
  tmp = init_list();
  add_doc(1, 2, tmp);
  add_doc(2, 3, tmp);
  final = init_list();
  add_doc(2, 3, final);
  add_doc(3, 4, final);
  final = intersection(tmp, final);
  SHOULD_BE(final->head->docID == 2);
  SHOULD_BE(final->head->freq == 6);
  SHOULD_BE(final->head->next == NULL);
  free_list(final);
  END_TEST_CASE;
}

// unioning two lists 
int TestUNIONIZE() {
  // these two lists are the same as the ones in testINTERSECT2
  START_TEST_CASE;
  tmp = init_list();
  add_doc(1, 2, tmp);
  add_doc(2, 3, tmp);
  final = init_list();
  add_doc(2, 3, final);
  add_doc(3, 4, final);
  final = unionize(tmp, final);
  SHOULD_BE(final->head->docID == 3);
  SHOULD_BE(final->head->freq == 4);
  SHOULD_BE(final->head->next->docID == 1);
  SHOULD_BE(final->head->next->freq == 2);
  SHOULD_BE(final->head->next->next->docID == 2);
  SHOULD_BE(final->head->next->next->freq == 3);
  SHOULD_BE(final->head->next->next->next == NULL);
  free_list(final);

  END_TEST_CASE;
}

// test mergesort
int TestSORT(){
  START_TEST_CASE;
  final = init_list();
  add_doc(1, 2, final);
  add_doc(7, 5, final);
  add_doc(2, 3, final);
  add_doc(3, 4, final);
  DocNode * doc = mergesort(final->head);
  final->head = doc;
  SHOULD_BE(doc->docID == 7);
  SHOULD_BE(doc->freq == 5);
  SHOULD_BE(doc->next->docID == 3);
  SHOULD_BE(doc->next->freq == 4);
  SHOULD_BE(doc->next->next->docID == 2);
  SHOULD_BE(doc->next->next->freq == 3);
  SHOULD_BE(doc->next->next->next->docID == 1);
  SHOULD_BE(doc->next->next->next->freq == 2);
  SHOULD_BE(doc->next->next->next->next == NULL);
  free_list(final);
  END_TEST_CASE;

}
// if value in the index
int TestINDEX1(){
  START_TEST_CASE;
  // testing if 2 is in index
  int index = find_index(a, 5, 2);
  SHOULD_BE(index == 1);
  END_TEST_CASE;
}

int TestINDEX2(){
  START_TEST_CASE;
  // testing if 124 is in index
  int index = find_index(a, 5, 124);
  SHOULD_BE(index == -1);
  END_TEST_CASE; 
}

//test getting specified url
int TestURL1(){
    START_TEST_CASE;
    char * url = get_url("2", "dir/");
    SHOULD_BE(strcmp(url, "http://old-www.cs.dartmouth.edu/~cs50/tse/wiki/Computer_science.html\n") == 0);
    free(url);
    END_TEST_CASE;
}

// test getting a bad url : that is, the file is non existent for some reason.
int TestURL2(){
    START_TEST_CASE;
    char * url = get_url("3", "dir/");
    SHOULD_BE(url == NULL);
    END_TEST_CASE;
}
// test processing a single-word query
int TestProcessQuery1() {
  START_TEST_CASE;
  char * query = (char *)calloc(13,sizeof(char));
  strcpy(query, "certified\n");
  WordNode* qresult = processQuery(query, i);
  SHOULD_BE(qresult->head->docID == 4);
  SHOULD_BE(qresult->head->freq == 5);
  SHOULD_BE(qresult->head->next == NULL);
  free(query);
  END_TEST_CASE;
}

// test processing a query with 2 words that don't have common documents
int TestProcessQuery2() {
  START_TEST_CASE;
  char * query = (char *)calloc(23,sizeof(char));
  strcpy(query, "certified AND stadium\n");
  WordNode* qresult = processQuery(query, i);
  SHOULD_BE(qresult->head == NULL);
  free(query);
  END_TEST_CASE;
}

// test processing a query with 2 words that do have common documents
int TestProcessQuery3() {
  START_TEST_CASE;
  char * query = (char *)calloc(40, sizeof(char));
  strcpy(query, "architectures AND certified\n");
  WordNode* qresult = processQuery(query, i);
  SHOULD_BE(qresult->head->docID == 4);
  SHOULD_BE(qresult->head->freq == 7);
  SHOULD_BE(qresult->head->next == NULL);
  free(query);
  END_TEST_CASE;
}

//test processing empty query
int TestProcessQuery4() {
  START_TEST_CASE;
  char * query = (char *)calloc(2,sizeof(char));
  strcpy(query, "\n");
  WordNode* qresult = processQuery(query, i);
  SHOULD_BE(qresult->head == NULL);
  free(query);
  END_TEST_CASE;
}




int main(int argc, char** argv) {
  int cnt = 0;
  i = ReadFile("./test_index");
  
  RUN_TEST(TestINTERSECT1, "AND case 1");
  RUN_TEST(TestINTERSECT2, "AND case 2");
  RUN_TEST(TestUNIONIZE, "OR case 1");
  RUN_TEST(TestSORT, "MERGESORT case");
  RUN_TEST(TestINDEX1, "Find Index case 1");
  RUN_TEST(TestINDEX2, "Find Index case 2");
  RUN_TEST(TestURL1, "getURL case 1");
  RUN_TEST(TestURL2, "getURL case 2");
  RUN_TEST(TestProcessQuery1, "processQuery case 1");
  RUN_TEST(TestProcessQuery2, "processQuery case 2");
  RUN_TEST(TestProcessQuery3, "processQuery case 3");
  RUN_TEST(TestProcessQuery4, "processQuery case 4");

  free_table(i);

  if (!cnt) {
    printf("All passed!\n"); return 0;
  } else {
    printf("Some fails!\n"); return 1;
  }

}

