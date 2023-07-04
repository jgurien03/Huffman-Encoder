How to Build and Run Program:

To run this program, use the 'Make' function with the makefile to compile the
project as necessary. If you would like to only make the derived files to 
encode, simply press 'make encode.' You can do the same thing for decode as
well by pressing 'make decode.' Also, you can use 'make scan-build' or 'make
format' for any additional compiling needs. Once compiling this program, you
can then parse certain options using the getopt() parser. The command options
are listed as follows:

	-h: This will print a helper message showing options.

	-i: This will specify an infile to get a file from (default stdin).
		
	-o: This will specify and outfile to send a file to (default stdout).

	-v: This will enable stats to print stats.

Scan-Build False Positives:

My program has one false positive - that being that when I try to decode a 
file, sometimes it will show that I am dereferencing a null pointer. This is
supposed to happen, since I am checking if a node's left child is null, and, 
this triggers scan-build because it sees that I am trying to check a value
that is null. This is supposed to happen though, so it is a false positive.

Files Included:

encode.c - A main that allows for my program to encode a file.

decode.c - A main that allows for my program to decode a file.

defines.h - The necessary interface to define certain macros such as ALPHABET.

header.h - This supplies the interface for the header structure.

node.h - This contains the interface for the node functions.

node.c - This contains the implementation for the node functions.

pq.h -This contains the necessary interface for the priority queue functions.

pq.c - This contains the implementation of the priority queue.

code.h - This contains the interface for the code functions.

code.c - This contains the implementations of the code functions.

io.h - This contains the interface for the io functions.

io.c - This contains the implementations for the io functions.

stack.h - This contains the interface for the stack functions.

stack.c - This contains the implementations for the stack function.

huffman.h - This contains the interface for the huffman functions.

huffman.c - This contains the implementations for the huffman functions.

Makefile - A file used to compile my program.

README.md - This file, a file containing all command options and file info.

DESIGN.pdf - My design document that contains my outline for this assignment.

node\_helper.c - A file containing the heap functions I used in lab 4.

node\_helper.h - A file containing the interface for the heap file.

byte.c - A file containing a function to initialize a code.

byte.h - A file containing the interface for the code initialization function.
