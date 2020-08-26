#target: dependency1 dependency2 ...
#<tab> command
OBJECTS=hashstructure.o liststructure.o treestructure.o 
all:worker main

hashstructure.o: hashstructure.c
	gcc -c -o hashstructure.o hashstructure.c

liststructure.o: liststructure.c
	gcc -c -o liststructure.o liststructure.c

treestructure.o: treestructure.c
	gcc -c -o treestructure.o treestructure.c

worker.o: worker.c
	gcc -c -o worker.o worker.c

main.o:main.c
	gcc -c -o main.o main.c

worker: worker.o $(OBJECTS)
	gcc -o worker worker.o $(OBJECTS)

main: main.o $(OBJECTS)
	gcc -o main main.o $(OBJECTS)

.PHONY: clean

clean:
	rm -f main.o
	rm -f worker.o
	rm -f worker
	rm -f main
	rm -f hashstructure.o
	rm -f liststructure.o
	rm -f treestructure.o
