#include "huffman.h"
#include "byte.h"
#include "code.h"
#include "defines.h"
#include "io.h"
#include "pq.h"
#include "stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// initializes static variables
static uint32_t count = 0;
Code c;

// function to build a tree
Node *build_tree(uint64_t hist[static ALPHABET]) {
  // dynamically creates a priority queue
  PriorityQueue *pq = pq_create(ALPHABET);
  // if the pq exists, then set 5 nodes
  if (pq != NULL) {
    Node *final;
    Node *finish;
    Node *left;
    Node *right;
    Node *here;
    // loop through ALPHABET (256), and check if the histogram
    // values aren't 0 (unique values)
    for (uint32_t i = 0; i < ALPHABET; i += 1) {
      if (hist[i] != 0) {
        // if they are, then create a node given the
        // index and the value stored in the histogram
        here = node_create(i, hist[i]);
        // enqueue the node
        enqueue(pq, here);
      }
    }
    // next, continue while the size of the pq is greater than 1
    while (pq_size(pq) > 1) {
      // dequeue left and right nodes
      dequeue(pq, &left);
      dequeue(pq, &right);
      // create a parent node that joins the left and right
      // nodes
      final = node_join(left, right);
      // enqueues this node to replace previous left and right
      // ones
      enqueue(pq, final);
    }
    // finally, dequeues left over node, deletes the pq, and
    // returns this node, representing the root node
    dequeue(pq, &finish);
    pq_delete(&pq);
    return finish;
    // if the pq is null, free it and set it to null
  } else {
    free(pq);
    pq = NULL;
  }
  // if all else fails, then return null
  return NULL;
}

// function to build code table
void build_codes(Node *root, Code table[static ALPHABET]) {
  // check if the root exists, and if it does, then check if the static
  // variable count is 0 (it will always be 0 on first run)
  if (root != NULL) {
    // if it is 0, then initialize the code structure using helper
    // function
    if (count == 0) {
      code(&c);
    }
    // then, increment the count so the code structure can always
    // remain the same
    count += 1;
    // check if the left and right roots are null, and if they are,
    // then set the table with the root's symbol as the index to
    // the code
    if (root->left == NULL && root->right == NULL) {
      table[root->symbol] = c;
      // if they aren't then push a 0 bit and recurse on the left side
    } else {
      uint8_t pop;
      code_push_bit(&c, 0);
      build_codes(root->left, table);
      // next pop this bit
      code_pop_bit(&c, &pop);

      // then, push a bit and recurse down the right side
      code_push_bit(&c, 1);
      build_codes(root->right, table);
      // pop this bit too
      code_pop_bit(&c, &pop);
    }
    // if the root is invalid, set the count back to 0 to restart if needed
  } else {
    count = 0;
  }
}

// function to dump a tree using post-order traversal
void dump_tree(int outfile, Node *root) {
  // if the root exists, continue
  if (root != NULL) {
    // travels down the left side of tree using recursion first
    dump_tree(outfile, root->left);
    // next, travel down the right side recursively
    dump_tree(outfile, root->right);
    // if the left and right root value's are null (leaf value):
    // create a buffer array of size two and set its two indices
    // to the character 'L' (leaf) and its symbol
    // write these characters to the outfile
    if (root->left == NULL && root->right == NULL) {
      uint8_t buf[2] = {'L', root->symbol};
      write_bytes(outfile, buf, 2);
      // else, the node must be an interior node, so create a size 1
      // buffer and set its one value to 'I' (interior node)
      // then, write this value to the outfile
    } else {
      uint8_t buf[1] = {'I'};
      write_bytes(outfile, buf, 1);
    }
  }
}

// function to rebuild the tree
Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
  // first, create a stack object
  Stack *stack = stack_create(nbytes);
  // check if the stack is valid, and if it is, continue
  if (stack != NULL) {
    // loop from the bottom to nbytes (top of stack)
    for (uint32_t i = 0; i < nbytes; i += 1) {
      // check if the dumped index is 'L,' meaning left
      // if it is, then push a created node with its symbol
      // being the tree dump's i + 1 (representing symbol
      // index)
      // also, set the pushed node's frequency to 0
      if (tree_dump[i] == 'L') {
        stack_push(stack, node_create(tree_dump[i + 1], 0));
        // increment i by 1
        i += 1;
        // else, create three nodes
      } else {
        Node *temp1;
        Node *temp2;
        Node *temp3;
        // pop the first and second nodes
        stack_pop(stack, &temp1);
        stack_pop(stack, &temp2);
        // then, use the parent node to join them
        temp3 = node_join(temp2, temp1);
        // push this node onto the stack
        stack_push(stack, temp3);
      }
    }
    // then, create final node, representing the root
    Node *final;
    // pop this node off of the stack, delete the stack, and return
    // the popped node
    stack_pop(stack, &final);
    stack_delete(&stack);
    return final;
    // if the stack is null, then free the stack and set it to null
  } else {
    free(stack);
    stack = NULL;
  }
  // if all else fails, return null
  return NULL;
}

// function to delete tree
void delete_tree(Node **root) {
  // check if the tree root exists
  if ((*root) != NULL) {
    // if it does, conduct post-order traversal to delete all
    // aspects of the tree
    delete_tree(&(*root)->left);
    delete_tree(&(*root)->right);
    // deletes root node and set it to null
    node_delete(root);
    root = NULL;
  }
}
