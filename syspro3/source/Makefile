#target: dependency1 dependency2 ...
#<tab> command
OBJECTS=parse.o read_args.o hashstructure.o liststructure.o treestructure.o 
all:whoClient whoServer master worker

hashstructure.o: hashstructure.c
	gcc -c -o hashstructure.o hashstructure.c

liststructure.o: liststructure.c
	gcc -c -o liststructure.o liststructure.c

treestructure.o: treestructure.c
	gcc -c -o treestructure.o treestructure.c

parse.o: parse.c
	gcc -c -o parse.o parse.c

read_args.o: read_args.c
	gcc -c -o read_args.o read_args.c

whoClient.o: whoClient.c
	gcc -c -o whoClient.o whoClient.c

whoClient: whoClient.o $(OBJECTS)
	gcc -o whoClient whoClient.o $(OBJECTS)  -lpthread

whoServer.o: whoServer.c
	gcc -c -o whoServer.o whoServer.c

whoServer: whoServer.o $(OBJECTS)
	gcc -o whoServer whoServer.o $(OBJECTS)  -lpthread

master.o: master.c
	gcc -c -o master.o master.c

master: master.o 
	gcc -o master master.o

worker.o: worker.c
	gcc -c -o worker.o worker.c

worker: worker.o $(OBJECTS)
	gcc -o worker worker.o $(OBJECTS)


.PHONY: clean

clean:
	rm -f whoServer.o
	rm -f whoClient.o
	rm -f read_args.o
	rm -f parse.o
	rm -f master.o
	rm -f worker.o
	rm -f worker
	rm -f whoClient
	rm -f whoServer
	rm -f master
	rm -f hashstructure.o
	rm -f liststructure.o
	rm -f treestructure.o
