#include "stack.h"
#include "code.h"
#include "defines.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// stack structure
struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
};

// function to create a stack
Stack *stack_create(uint32_t capacity) {
  // dynamically allocate memory for the stack
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  // if the stack exists, set its top to 0, and its capacity to the
  // passed-in capacity
  // then, create a complementary node array
  // if the node array exists, set the stack structure's node array
  // to the newly created one
  // else, free the arr
  if (stack != NULL) {
    stack->top = 0;
    stack->capacity = capacity;
    Node **arr = (Node **)calloc(capacity, sizeof(Node *));
    if (arr != NULL) {
      stack->items = arr;
    } else {
      free(arr);
      arr = NULL;
    }
    // if the stack doesn't exist, free it and set the pointer to null
  } else {
    free(stack);
    stack = NULL;
  }
  // return the created stack
  return stack;
}

// function to delete the stack
void stack_delete(Stack **s) {
  // if the stack exists, check if the arr exists too
  // if the arr exists, free it
  // then, free the stack and set its pointer to null
  if (*s != NULL) {
    if ((*s)->items != NULL) {
      free((*s)->items);
    }
    free(*s);
    *s = NULL;
  }
}

// returns true if the stack's top is equal to 0
bool stack_empty(Stack *s) { return s->top == 0; }

// returns true if the stack's top is equal to its capacity
bool stack_full(Stack *s) { return s->top == s->capacity; }

// returns the stack's size by returning the top variable (highest point in
// stack)
uint32_t stack_size(Stack *s) { return s->top; }

// pushes a bit onto the stack
bool stack_push(Stack *s, Node *n) {
  // if the stack exists, check if it is full
  // if it is full, return false
  // else, set the stack's node array at the top index to the passed-in
  // node
  // then, increment the top by 1 to account for new amount and return
  // true
  if (s != NULL) {
    if (stack_full(s)) {
      return false;
    } else {
      s->items[s->top] = n;
      s->top += 1;
      return true;
    }
    // else, free the stack
  } else {
    free(s);
    s = NULL;
  }
  // if it is null, return false
  return false;
}

// function to pop value from the stack
bool stack_pop(Stack *s, Node **n) {
  // if the stack exists, check if it is empty
  // if it isn't empty set the node pointer's value to the top of stack
  // decrease the top value
  // return true
  if (s != NULL) {
    if (stack_empty(s)) {
      return false;
    } else {
      *n = s->items[s->top - 1];
      s->top -= 1;
      return true;
    }
    // else, free the stack and set it to null
  } else {
    free(s);
    s = NULL;
  }
  // return false if all else fails
  return false;
}

// function to print stack
void stack_print(Stack *s) {
  // if stack exists, loop to the top and print each node array's index
  if (s != NULL) {
    for (uint32_t i = 0; i < s->top; i += 1) {
      node_print(s->items[i]);
    }
  }
}
