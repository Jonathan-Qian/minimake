minimake: main.o parser.o
	gcc -o minimake main.o parser.o

main.o: main.c defs.h
	gcc -c main.c

parser.o: parser.c defs.h
	gcc -c parser.c

clean:
	rm -f *.o minimake