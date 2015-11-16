# makefile for query engine
CC = gcc
CFLAGS = -Wall -pedantic -std=c11

# my project details
UTILFLAG = -ltseutil
EXEC = query
# OBJS = hashtable.o web.o common_index.o file.o
SRCDIR = src/
SRCS = $(SRCDIR)common_query.c
# OBJDIR = obj/

UTILDIR = ../util/
UTILC = $(UTILDIR)hashtable.c $(UTILDIR)common_index.c $(UTILDIR)file.c $(UTILDIR)web.c
UTILH=$(UTILC:.c=.h)
MAIN = $(SRCDIR)query.c
UTILLIB=$(UTILDIR)libtseutil.a

# with library: 
query: $(MAIN) $(UTILLIB)
	$(CC) $(CFLAGS) -o $@ $(MAIN) $(SRCS) -L$(UTILDIR) $(UTILFLAG)

#deprecated: used without library. 
# query: $(SRCS)
# 	$(CC) $(CFLAGS) -o $(EXEC) $(SRCS) 

$(UTILLIB): $(UTILC) $(UTILH)
	cd $(UTILDIR); make;

debug: $(MAIN) $(UTILLIB)
	$(CC) $(CFLAGS) -g -ggdb -c $(MAIN)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC) $(MAIN) -L$(UTILDIR) $(UTILFLAG)

query_test: $(MAIN) test/query_test.c $(UTILLIB)
	$(CC) $(CFLAGS) -g -ggdb -o test/query_test test/query_test.c $(SRCS) -L$(UTILDIR) $(UTILFLAG)

clean:
	rm -f *.o
	rm $(EXEC)