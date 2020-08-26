# Define the symbols we might want to change:
CC=gcc
OBJECTS=parse.o main.o hashtable.o list.o bstree.o 
CFLAGS=-g
# comment
# (note: the <tab> in the command line is necessary for make to work)
#target: dependency1 dependency2 ...
#<tab> command


DiseaseMonitor: $(OBJECTS)
	$(CC) $(CFLAGS) -o DiseaseMonitor $(OBJECTS) -lm 

main.o: main.c
	$(CC) -c -o main.o main.c

parse.o: parse.c
	$(CC) -c -o parse.o parse.c

hashtable.o: hashtable.c
	$(CC) -c -o hashtable.o hashtable.c

list.o: list.c
	$(CC) -c -o list.o list.c

bstree.o: bstree.c
	$(CC) -c -o bstree.o bstree.c




.PHONY: clean

clean:
	rm -f DiseaseMonitor DiseaseMonitor.o
	rm -f main.o
	rm -f parse.o
	rm -f hashtable.o
	rm -f list.o
	rm -f bstree.o

	

	
