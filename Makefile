all: minimake

minimake: main.o parser.o graph.o target.o
	gcc -o minimake main.o parser.o graph.o target.o

main.o: main.c defs.h
	gcc -c main.c

parser.o: parser.c defs.h
	gcc -c parser.c

graph.o: graph.c defs.h
	gcc -c graph.c

target.o: target.c defs.h
	gcc -c target.c

clean:
	rm -f *.o minimake