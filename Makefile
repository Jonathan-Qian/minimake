minimake: parser.o
	gcc -o minimake parser.o

parser.o: parser.c defs.h
	gcc -c parser.c

clean:
	rm -f *.o minimake