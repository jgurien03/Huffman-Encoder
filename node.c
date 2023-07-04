#include "node.h"
#include "node_helper.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

// this function creates a node
Node *node_create(uint8_t symbol, uint64_t frequency) {
  // dynamically allocates memory for a node
  Node *nod = (Node *)malloc(sizeof(Node));
  // check if node exists, if it does, create right and left nodes
  // and set the node's frequency and symbol
  if (nod != NULL) {
    nod->left = NULL;
    nod->right = NULL;
    nod->symbol = symbol;
    nod->frequency = frequency;
    // else, free the node
  } else {
    free(nod);
    nod = NULL;
  }
  // return the created node
  return nod;
}

// deletes node
void node_delete(Node **N) {
  // checks if it exists, and if it does, frees it
  if (*N != NULL) {
    free(*N);
    *N = NULL;
  }
}

// function to join left and right nodes into a parent node
Node *node_join(Node *left, Node *right) {
  // checks if the left and right nodes exist, and if they do,
  // create parent node
  Node *parent =
      left != NULL && right != NULL
          ? node_create((uint8_t)'$', left->frequency + right->frequency)
          : NULL;
  // if the parent node is invalid, return null
  if (parent == NULL) {
    return NULL;
    // else, set the parent's left and right nodes to the two passed-in
    // ones
  } else {
    parent->left = left;
    parent->right = right;
  }
  // return parent node
  return parent;
}

// returns true if the left node's frequency is less than the right's, and
// false otherwise
bool node_cmp(Node *left, Node *right) {
  if (left->frequency < right->frequency) {
    return true;
  }
  return false;
}

// prints a node, its children, and prints null statements if any of them
// don't exist
void node_print(Node *n) {
  if (n != NULL) {
    printf("Parent node created correctly!\n");
    printf("frequency: %lu\n", n->frequency);
    printf("symbol: %u\n", n->symbol);
    if (n->left != NULL) {
      printf("Left node created correctly!\n");
      printf("frequency: %lu\n", n->left->frequency);
      printf("symbol: %u\n", n->symbol);
    } else {
      printf("Left node is null!\n");
    }
    if (n->right != NULL) {
      printf("Right node created correctly!\n");
      printf("frequency: %lu\n", n->right->frequency);
      printf("symbol: %u\n", n->symbol);
    } else {
      printf("Right node is null!\n");
    }
  } else {
    printf("Nodes failed to print\n");
  }
}
