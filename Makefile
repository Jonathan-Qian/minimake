all: minimake

minimake: main.o parser.o graph.o target.o
	gcc -g -o minimake main.o parser.o graph.o target.o

main.o: main.c defs.h
	gcc -g -c main.c

parser.o: parser.c defs.h
	gcc -g -c parser.c

graph.o: graph.c defs.h
	gcc -g -c graph.c

target.o: target.c defs.h
	gcc -g -c target.c

clean:
	rm -f *.o minimake