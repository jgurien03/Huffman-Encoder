#include "byte.h"
#include "code.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "node_helper.h"
#include "pq.h"
#include "stack.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "i:o:vh"

// initializes main function
int main(int argc, char **argv) {
  // initializing default variables
  int in = STDIN_FILENO;
  int out = STDOUT_FILENO;
  // creates temporary file in the event that we are encoding with stdin
  FILE *here = tmpfile();
  // makes a temp fileno to write and read from it using sys calls
  int temp = fileno(here);
  bool verbose = false;
  int opt;
  // parses command options
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    // prints helper message
    case 'h':
      fprintf(stderr, "SYNOPSIS\n");
      fprintf(stderr, "  A Huffman encoder.\n");
      fprintf(stderr,
              "  Compresses a file using the Huffman coding algorithm.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "USAGE\n");
      fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "OPTIONS\n");
      fprintf(stderr, "  -h             Program usage and help.\n");
      fprintf(stderr, "  -v             Print compression statistics.\n");
      fprintf(stderr, "  -i infile      Input file to compress.\n");
      fprintf(stderr, "  -o outfile     Output of compressed data.\n");
      return 0;
    // sets infile
    case 'i':
      in = open(optarg, O_RDONLY);
      if (in == -1) {
        fprintf(stderr, "File open error!\n");
        return 1;
      }
      break;
    // sets outfile
    case 'o':
      out = open(optarg, O_CREAT | O_WRONLY);
      break;
    // shows stats if enabled
    case 'v':
      verbose = true;
      break;
    // print a helper message if all invalid option
    // entered
    default:
      fprintf(stderr, "SYNOPSIS\n");
      fprintf(stderr, "  A Huffman encoder.\n");
      fprintf(stderr,
              "  Compresses a file using the Huffman coding algorithm.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "USAGE\n");
      fprintf(stderr, "  ./encode [-h] [-i infile] [-o outfile]\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "OPTIONS\n");
      fprintf(stderr, "  -h             Program usage and help.\n");
      fprintf(stderr, "  -v             Print compression statistics.\n");
      fprintf(stderr, "  -i infile      Input file to compress.\n");
      fprintf(stderr, "  -o outfile     Output of compressed data.\n");
      return 1;
    }
  }
  // initializes a histogram that can hold ALPHABET indexes
  uint64_t hist[ALPHABET];
  // clears histogram
  for (uint32_t i = 0; i < 256; i += 1) {
    hist[i] = 0;
  }
  // creates a buffer of size 1
  uint8_t buffer[1];
  uint64_t temp_bytes = 0;
  // read bytes into the buffer from the infile one byte at a time
  while (read_bytes(in, buffer, 1) != -1) {
    // increment temporary bytes amount that is used for later
    temp_bytes += 1;
    // sets the values in the histrogram
    hist[buffer[0]] += 1;
    // checks if the infile is stdin
    if (in == STDIN_FILENO) {
      // if it is, write bytes into temp file one at a time
      int here = write_bytes(temp, buffer, 1);
      // break if no more bytes can be read
      if (here == -1) {
        break;
      }
    }
    // sets buffer index back to 0 to read in more bytes next time
    buffer[0] = 0;
  }
  // creates two booleans to check if file is stdin
  bool file_check;
  bool hi = false;
  file_check = in == STDIN_FILENO ? true : false;
  // if file is stdin, set boolean hi to true and the infile to the temp
  // file
  if (file_check == true) {
    hi = true;
    in = temp;
  }
  // initialize two more booleans to check if the first and second
  // index values of the histogram are 0
  bool check;
  check = hist[1] == 0 ? true : false;
  bool check1;
  check1 = hist[0] == 0 ? true : false;
  // if they are, then set these indices' values to 1
  if (check == true) {
    hist[1] = 1;
  }
  if (check1 == true) {
    hist[0] = 1;
  }
  // initialize a code table that can hold ALPHABET indices
  Code table[256];
  // loop throught the table and initialize each index
  for (uint32_t i = 0; i < 256; i += 1) {
    table[i] = code_init();
  }
  // initializes a unique count that will increase each time a value
  // in the histogram isn't equal to 0
  uint32_t unique_count = 0;
  for (uint32_t i = 0; i < 256; i += 1) {
    if (hist[i] > 0) {
      unique_count += 1;
    }
  }
  // build the tree and code table
  Node *tree = build_tree(hist);
  build_codes(tree, table);
  // initializing two stat structure to be used for stats
  struct stat stat1;
  struct stat stat2;
  fstat(in, &stat1);
  // sets infile and outfile permissions
  fchmod(in, stat1.st_mode);
  fchmod(out, stat1.st_mode);
  // creates a header and sets its magic number, mode, tree size, and
  // file size to variables retrieved from the header structure, the
  // fstat call, and the unique count variable
  Header head;
  head.magic = MAGIC;
  head.permissions = stat1.st_mode;
  head.tree_size = (3 * unique_count) - 1;
  head.file_size = stat1.st_size;
  // creates a bit value that represents the header
  uint8_t *bit = (uint8_t *)&head;
  // write bytes to outfile from the header
  write_bytes(out, bit, sizeof(Header));
  // dumps the tree to write all leaves, interiors, and symbols to file
  dump_tree(out, tree);
  // subtracts external bytes read from temp bytes to get correct stats
  bytes_read -= temp_bytes;
  // calls lseek to rewind file back to the start
  lseek(in, 0, SEEK_SET);
  // reads in bytes again 1 by 1 until the infile cannot be read from
  // anymore
  while (true) {
    int j = read_bytes(in, buffer, 1);
    if (j == -1) {
      break;
      // writes codes to the outfile from the table
    } else {
      write_code(out, &table[buffer[0]]);
    }
  }
  // flushes out all remaining codes
  flush_codes(out);
  // calls fstat on outfile
  fstat(out, &stat2);
  // if the file is stdin, subtract bytes read bytes written from bytes
  // written to get proper stats
  if (hi == true) {
    bytes_written -= bytes_read;
  }
  // if verbose is true, print all stat values
  if (verbose == true) {
    fprintf(stderr, "Uncompressed file size: %lu bytes\n", bytes_read);
    fprintf(stderr, "Compressed file size: %ld bytes\n", bytes_written);
    fprintf(stderr, "Space saving: %.2f%%\n",
            (100 * (1 - ((double)bytes_written / bytes_read))));
  }
  // close all files and delete tree
  close(in);
  close(out);
  fclose(here);
  delete_tree(&tree);
  // finally, return 0
  return 0;
}
