CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -gdwarf-4 -g -Ofast

.PHONY: clean spotless encode all

all: encode decode

encode: node.o node_helper.o pq.o encode.o code.o stack.o io.o byte.o huffman.o
	$(CC) -o $@ $^

decode: node.o node_helper.o pq.o code.o stack.o io.o byte.o huffman.o decode.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

spotless:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[c] 

scan-build: clean
	scan-build --use-cc=$(CC) make

