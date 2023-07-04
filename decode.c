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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "i:o:vh"

// initializes main function

int main(int argc, char **argv) {
  // initializing default variables
  int in = STDIN_FILENO;
  int out = STDOUT_FILENO;
  int opt;
  bool verbose = false;
  bool lala = false;
  // parsers command options
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      fprintf(stderr, "SYNOPSIS\n");
      fprintf(stderr, "  A Huffman decoder.\n");
      fprintf(stderr,
              "  Decompresses a file using the Huffman coding algorithm.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "USAGE\n");
      fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "OPTIONS\n");
      fprintf(stderr, "  -h             Program usage and help.\n");
      fprintf(stderr, "  -v             Print compression statistics.\n");
      fprintf(stderr, "  -i infile      Input file to decompress.\n");
      fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
      return 0;
    case 'i':
      in = open(optarg, O_RDONLY);
      if (in == -1) {
        fprintf(stderr, "File open error!\n");
        return 1;
      }
      lala = true;
      break;
    case 'o':
      out = open(optarg, O_CREAT | O_WRONLY);
      if (out == -1) {
        fprintf(stderr, "File open error!\n");
        return 1;
      }
      lala = true;
      break;
    case 'v':
      verbose = true;
      break;
    default:
      fprintf(stderr, "SYNOPSIS\n");
      fprintf(stderr, "  A Huffman decoder.\n");
      fprintf(stderr,
              "  Decompresses a file using the Huffman coding algorithm.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "USAGE\n");
      fprintf(stderr, "  ./decode [-h] [-i infile] [-o outfile]\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "OPTIONS\n");
      fprintf(stderr, "  -h             Program usage and help.\n");
      fprintf(stderr, "  -v             Print compression statistics.\n");
      fprintf(stderr, "  -i infile      Input file to decompress.\n");
      fprintf(stderr, "  -o outfile     Output of decompressed data.\n");
      return 1;
    }
  }
  // creates array that can hold the size of header (16)
  uint8_t hey[sizeof(Header)];
  // read in all bytes into the array
  read_bytes(in, hey, sizeof(Header));
  // create a second header
  Header head1;
  // use pointer manipulation to set the array to a header type
  head1 = *(Header *)hey;
  // check if the inherited header doesn't match, and if it doesn't,
  // print a helpful error message
  if (head1.magic != MAGIC) {
    fprintf(stderr, "Invalid magic number.\n");
    return 1;
  }
  // retrieves permissions
  fchmod(out, head1.permissions);
  // sets tree size
  uint64_t size = head1.tree_size;
  // sets file size
  uint64_t file_in_size = head1.file_size;
  // sets a tree array to hold size (tree size)
  uint8_t tree[size];
  // doles out array
  for (uint32_t i = 0; i < size; i += 1) {
    tree[i] = 0;
  }
  // checks if the infile is equal to stdin, and if it is, then
  // read bytes 1 by 1, utilizing pointer manipulation to get all
  // proper values without any segmentation faults
  if (in == STDIN_FILENO) {
    uint64_t i = 0;
    int j = 0;
    while (i < size) {
      j = read_bytes(in, tree + i, 1);
      if (j == -1) {
        break;
      }
      i += 1;
    }
    // if the infile isn't stdin, read all bytes in normally with tree
    // size amount
  } else {
    read_bytes(in, tree, size);
  }
  // creates a node value to hold the rebuild (root) node
  Node *here;
  here = rebuild_tree(size, tree);
  // sets temporary node value to root node
  Node *temp = here;
  // creates bit value to hold value read in from read_bit
  uint8_t bit = 0;
  // loop while the bytes written is less than the file size
  // read in a bit each time
  // check if the bit is 0, and if it is, then set the temporary node
  // to the left node
  // check if the new node's left and right values are null (leaf),
  // and write the bytes 1 by 1
  // then, set the node back to the previous node
  while (bytes_written < file_in_size) {
    read_bit(in, &bit);
    if (bit == 0) {
      temp = temp->left;
      if (temp->left == NULL && temp->right == NULL) {
        write_bytes(out, &temp->symbol, 1);
        temp = here;
      }
      // if the bit isn't 0, then set the temporary node to the
      // right side
      // check if the node is a leaf, and if it is, then write bytes
      // 1 by 1
      // sets the temp node back to the rebuild (root) node
    } else {
      temp = temp->right;
      if (temp->left == NULL && temp->right == NULL) {
        write_bytes(out, &temp->symbol, 1);
        temp = here;
      }
    }
  }
  // initializes a stat struct to obtain stats
  struct stat s1;
  delete_tree(&here);
  // prints stats
  if (verbose == true) {
    // if infile or outfile is going through the stdin or stdout
    // pipe, obtain stats and set the bytes_read to the inherited
    // file size
    if (lala == true) {
      fstat(in, &s1);
      bytes_read = s1.st_size;
    }
    // print all statistics
    fprintf(stderr, "Compressed file size: %lu bytes\n", bytes_read);
    fprintf(stderr, "Decompressed file size: %ld bytes\n", bytes_written);
    fprintf(stderr, "Space saving: %.2f%%\n",
            (100 * (1 - ((double)bytes_read / bytes_written))));
  }
  // close in and out files, and return 0
  close(in);
  close(out);
  return 0;
}
