CC = gcc
COMOBJ = main.o huffman.o charcounter.o

all: huffman

huffman: $(COMOBJ)
	$(CC) -o huffman $(COMOBJ)

clean:
	rm *.o

main.o: main.c huffman.h
huffman.o: huffman.c huffman.h charcounter.h
charcounter.o: charcounter.c charcounter.h