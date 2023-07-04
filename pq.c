#include "pq.h"
#include "node.h"
#include "node_helper.h"
#include "string.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// creates struct for priority queue
struct PriorityQueue {
  uint32_t capacity;
  uint64_t size;
  Node **arr;
};

// function to create priority queue
PriorityQueue *pq_create(uint32_t capacity) {
  // dynamically allocates memory for pq
  PriorityQueue *pq = malloc(sizeof(PriorityQueue));
  // if it exists, set its capacity to the passed-in capacity, and creates
  // a node
  // sets its size to 0 as well
  if (pq != NULL) {
    pq->capacity = capacity;
    pq->size = 0;
    Node **arr = (Node **)calloc(capacity, sizeof(Node *));
    // if the node exists, set the arr's node value to this node
    if (arr != NULL) {
      pq->arr = arr;
      // else, free it
    } else {
      free(arr);
      arr = NULL;
    }
    // free the priority queue in the event that it doesn't exist
  } else {
    free(pq);
    pq = NULL;
  }
  // return final pq
  return pq;
}

// function to delete priority queue
void pq_delete(PriorityQueue **q) {
  // checks if the queue exists, and if it does, frees it and its arr
  if (*q != NULL) {
    free((*q)->arr);
    free(*q);
    *q = NULL;
  }
}

// returns if the pq is empty by checking if its size is equal to 0
bool pq_empty(PriorityQueue *q) { return q->size == 0; }

// returns the pq's size by return pq pointed size
uint32_t pq_size(PriorityQueue *q) { return q->size; }

// returns true if the pq is full (size equals capacity), and false otherwise
bool pq_full(PriorityQueue *q) { return pq_size(q) == q->capacity; }

// enqueues a node into the queue
bool enqueue(PriorityQueue *q, Node *n) {
  // boolean to check if the pq is full or if the pq is null
  bool check;
  check = pq_full(q) || q == NULL ? true : false;
  // if it is true, return false
  if (check == true) {
    return false;
  }
  // else, pass the array's stored size index's amount into the node
  // call up heap to build (percolate up) the heap
  // increment size by 1
  if (check == false) {
    q->arr[q->size] = n;
    up_heap(q->arr, q->size);
    q->size += 1;
  }
  // return true if successfully completed
  return true;
}

// dequeues a node from the queue
bool dequeue(PriorityQueue *q, Node **n) {
  // boolean to check if the pq is empty or if the pq is null
  bool check;
  check = pq_empty(q) || q == NULL ? true : false;
  // if it is, return false
  if (check == true) {
    return false;
  }
  // else, set the node's value to the first (front) value of the node
  // swaps the node's values with the last index value, and percolate
  // down after to reformat the array
  if (check == false) {
    *n = q->arr[0];
    q->arr[0] = q->arr[q->size - 1];
    q->size -= 1;
    down_heap(q->arr, q->size);
  }
  // return true if completed
  return true;
}

// prints the pq by looping through the capacity and printing each node's
// index
void pq_print(PriorityQueue *q) {
  for (uint32_t i = 0; i < q->capacity; i += 1) {
    node_print(q->arr[i]);
  }
}
