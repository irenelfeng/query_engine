Query Engine
=============

##Overview

The query program will output a list of results based on a query string, returning the document(s) that match a given query string by the user. The documents are in order of highest number of matches. 

##How to Use

In the query/ folder, run the QEBATS.sh script to create the executable and run unit tests.
Afterwards, run the executable in terminal by this command: 
```
query test/large_index test/large_sample/
```
This creates a continuous loop of queries until the user enters in an escape character for the input. 

## New webpages 

This query therefore runs on the sample index (test/large_index) and the sample webpages (test/large_sample).
A related program I have created can be used to make new index files and new directories based on a specified webpage. You can find this at https://github.com/irenelfeng/webpages (will be open soon)

## Special Considerations 

Runs on bash version 4+
