# makes library for ltseutil.h 

CC=gcc
CFLAGS1=-Wall
CFLAGS=-O2 -Wall -pedantic -std=c11

UTILLIB= libtseutil.a
SOURCES= hashtable.c web.c file.c common_index.c
OBJECTS= $(SOURCES:.c=.o)

$(UTILLIB): $(OBJECTS) $(OBJFILE)
	ar -cvq $(UTILLIB) $(OBJECTS) $(OBJFILE)

$(OBJECTS): $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)
