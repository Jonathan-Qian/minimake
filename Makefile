OPT = -g -Wall -Wextra

all: minimake

minimake: main.o parser.o graph.o target.o threadpool.o buildcontext.o
	gcc $(OPT) -o minimake main.o parser.o graph.o target.o threadpool.o buildcontext.o

main.o: main.c defs.h
	gcc $(OPT) -c main.c

parser.o: parser.c defs.h
	gcc $(OPT) -c parser.c

graph.o: graph.c defs.h
	gcc $(OPT) -c graph.c

target.o: target.c defs.h
	gcc $(OPT) -c target.c

threadpool.o: threadpool.c defs.h
	gcc $(OPT) -c threadpool.c

buildcontext.o: buildcontext.c defs.h
	gcc $(OPT) -c buildcontext.c

clean:
	rm -f *.o minimake