OPT = -Wall -Wextra
VPATH = src

all: minimake

minimake: main.o parser.o graph.o target.o threadpool.o buildcontext.o
	gcc $(OPT) -o $@ $^

main.o: main.c buildcontext.h parser.h graph.h defs.h
	gcc $(OPT) -c $< -o $@

parser.o: parser.c parser.h defs.h
	gcc $(OPT) -c $< -o $@

graph.o: graph.c graph.h defs.h
	gcc $(OPT) -c $< -o $@

target.o: target.c target.h defs.h
	gcc $(OPT) -c $< -o $@

threadpool.o: threadpool.c threadpool.h defs.h
	gcc $(OPT) -c $< -o $@

buildcontext.o: buildcontext.c buildcontext.h defs.h
	gcc $(OPT) -c $< -o $@

clean:
	rm -f *.o minimake