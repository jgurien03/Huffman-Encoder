#include "node.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t l_child(uint32_t n) {
  // sets left child
  return 2 * n + 1;
}

uint32_t r_child(uint32_t n) {
  // sets right child
  return 2 * n + 2;
}

uint32_t parent(uint32_t n) {
  // sets parent
  return (n - 1) / 2;
}

void swap(Node *x, Node *y) {
  Node t = *x;
  *x = *y;
  *y = t;
}

void up_heap(Node **arr, uint32_t n) {
  // swaps arr of current and parent of curent if arr is less than parent
  while ((n > 0) && (node_cmp(arr[n], arr[parent(n)]) == true)) {
    swap(arr[n], arr[parent(n)]);
    n = parent(n);
  }
}

void down_heap(Node **arr, uint32_t heap_size) {
  uint32_t n = 0;
  uint32_t smaller;
  // makes sure that left child is under size of elements
  while (l_child(n) < heap_size) {
    // if no right child left child is smaller
    if (r_child(n) == heap_size) {
      smaller = l_child(n);
    } else {
      // if left child is smaller than right child, smaller is
      // left child
      if (node_cmp(arr[l_child(n)], arr[r_child(n)])) {
        smaller = l_child(n);
        // right child is smaller
      } else {
        smaller = r_child(n);
      }
    }
    // if current index is less than smaller index, break
    if (node_cmp(arr[n], arr[smaller])) {
      break;
    }
    // swaps smaller and current arr, and sets current array
    // to smaller
    swap(arr[smaller], arr[n]);
    n = smaller;
  }
}
