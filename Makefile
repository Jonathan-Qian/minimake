all: minimake

minimake: main.o parser.o graph.o target.o threadpool.o
	gcc -g -o minimake main.o parser.o graph.o target.o threadpool.o

main.o: main.c defs.h
	gcc -g -c main.c

parser.o: parser.c defs.h
	gcc -g -c parser.c

graph.o: graph.c defs.h
	gcc -g -c graph.c

target.o: target.c defs.h
	gcc -g -c target.c

threadpool.o: threadpool.c defs.h
	gcc -g -c threadpool.c

clean:
	rm -f *.o minimake