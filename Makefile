OPT = -Wall -Wextra

all: minimake

minimake: main.o parser.o graph.o target.o threadpool.o buildcontext.o
	gcc $(OPT) -o minimake main.o parser.o graph.o target.o threadpool.o buildcontext.o

main.o: main.c buildcontext.h parser.h graph.h defs.h
	gcc $(OPT) -c main.c

parser.o: parser.c parser.h defs.h
	gcc $(OPT) -c parser.c

graph.o: graph.c graph.h defs.h
	gcc $(OPT) -c graph.c

target.o: target.c target.h defs.h
	gcc $(OPT) -c target.c

threadpool.o: threadpool.c threadpool.h defs.h
	gcc $(OPT) -c threadpool.c

buildcontext.o: buildcontext.c buildcontext.h defs.h
	gcc $(OPT) -c buildcontext.c

clean:
	rm -f *.o minimake