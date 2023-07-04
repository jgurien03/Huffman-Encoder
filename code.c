#include "code.h"
#include "defines.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// function to initialize a code structure
Code code_init(void) {
  // initializes structure
  Code c;
  // sets its top value to 0
  c.top = 0;
  // loops through the max code size and doles out each index of the arr
  for (uint32_t i = 0; i < MAX_CODE_SIZE; i += 1) {
    c.bits[i] = 0;
  }
  // returns code structure
  return c;
}

// returns the top value of the code
uint32_t code_size(Code *c) { return c->top; }

// returns true if the top of the code equals 0
bool code_empty(Code *c) { return c->top == 0; }

// returns true if the top equals ALPHABET (256)
bool code_full(Code *c) { return c->top == 256; }

// sets a bit
bool code_set_bit(Code *c, uint32_t i) {
  // boolean to check if the index is out of range with the code
  bool check;
  check = i > c->top ? true : false;
  // if it is, return false
  if (check == true) {
    return false;
    // else, find position and location and bitshift to set bit in a byte
    // return true after
  } else {
    uint32_t position = i / 8;
    uint32_t location = i % 8;
    c->bits[position] |= (1 << location);
    return true;
  }
}

// function to clear a bit's value
bool code_clr_bit(Code *c, uint32_t i) {
  // value to check if index is out of range with the code
  bool check;
  check = i > c->top ? true : false;
  // return false if it is out of range
  if (check == true) {
    return false;
    // else, get position and location in the byte and clear the bit by
    // using bitshifting
    // return true after
  } else {
    uint32_t position = i / 8;
    uint32_t location = i % 8;
    c->bits[position] &= ~(1 << location);
    return true;
  }
  // if all else fails, return false
  return false;
}

// gets the value of a bit
bool code_get_bit(Code *c, uint32_t i) {
  // checks if the structure exists
  if (c) {
    // if it does, gets the position and location and uses
    // bitshifting to get the current value of the bit
    // returns true if the value is greater than 0 and the index is
    // in range
    // else, return false
    uint32_t position = i / 8;
    uint32_t location = i % 8;
    return (i < c->top) && ((c->bits[position] >> (location)) & 1UL) > 0
               ? true
               : false;
  } else {
    // if all else fails, return false
    return false;
  }
}

// function that pushes a bit onto the code structure
bool code_push_bit(Code *c, uint8_t bit) {
  // check boolean that sees if the code is already full
  // if it is, return false
  bool check;
  check = code_full(c) ? true : false;
  if (check == true) {
    return false;
    // else, check the bit's value
  } else {
    // if the bit is 1, set the bit, else, clear it
    bit == 1 ? code_set_bit(c, c->top) : code_clr_bit(c, c->top);
    // increment the top by 1 since a new bit was pushed
    c->top += 1;
    // return true after
    return true;
  }
  // if all else fails, return false
  return false;
}

// function that pops a bit from the stack
bool code_pop_bit(Code *c, uint8_t *bit) {
  // check boolean to return false if the code is empty
  bool check;
  check = code_empty(c) ? true : false;
  // if it is, return false
  if (check == true) {
    return false;
    // else, set the bit's value by using get bit
    // then clear the top bit off the stack
    // decrease the top by 1 since a bit was popped
  } else {
    *bit = code_get_bit(c, c->top - 1) ? 1 : 0;
    code_clr_bit(c, c->top - 1);
    c->top -= 1;
    // return true if all else goes well
    return true;
  }
  // return false if fails
  return false;
}

// function to print code structure by looping through the size and printing
// the bit's value by calling code_get_bit
void code_print(Code *c) {
  for (uint32_t i = 0; i < code_size(c); i += 1) {
    printf("%u\n", code_get_bit(c, i));
  }
}
