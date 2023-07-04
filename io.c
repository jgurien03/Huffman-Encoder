#include "io.h"
#include "byte.h"
#include "code.h"
#include "defines.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// initialize static variables

static int read1 = 0;
static int write1 = 0;
static int bytes = -1;
static int bytes1 = -1;
static uint64_t pos = 0;
static uint64_t pos1 = 0;
static uint8_t buf1[BLOCK];
static uint8_t buf2[BLOCK];
uint64_t bytes_read;
uint64_t bytes_written;

// function to read in bytes
int read_bytes(int infile, uint8_t *buf, int nbytes) {
  // check if the number of bytes is less than or equal to zero, and
  // if it is, return -1
  if (nbytes <= 0) {
    return -1;
    // else, set static variable to the sys call read function
  } else {
    bytes = read(infile, buf, nbytes);
    // check if the bytes read is 0, and if it is, return -1
    // return this here because no more bytes can be read
    if (bytes == 0) {
      return -1;
    }
    // else, increase the read1 static variable by the bytes read
    read1 += bytes;
    // increase the extern variable bytes read by this value too
    bytes_read += bytes;
    // recursively call read_bytes with a subtracted number of
    // bytes to read
    // this subtracted number accounts for the number of bytes that
    // was just read in
    read_bytes(infile, buf, nbytes - bytes);
  }
  // once everything finishes, return the bytes that are read
  return read1;
}

// function to return the amount of bytes that are read
int write_bytes(int infile, uint8_t *buf, int nbytes) {
  // checks if the number of bytes is less than or equal to 0
  // if it is, return -1
  if (nbytes <= 0) {
    return -1;
    // else, set static variable to write sys call
  } else {
    bytes1 = write(infile, buf, nbytes);
    // if this number is 0 (no more bytes to write), then return -1
    // and exit
    if (bytes1 == 0) {
      return -1;
    }
    // else, increase total variable by the bytes written
    write1 += bytes1;
    // recursively call function with a subtracted number of bytes
    // to account for the bytes just read
    write_bytes(infile, buf, nbytes - bytes1);
  }
  // once done, set the bytes written extern to the amount of bytes
  // written
  // then, return number of bytes written (write1)
  bytes_written = write1;
  return write1;
}

// function to read a bit
bool read_bit(int infile, uint8_t *bit) {
  // creates boolean variable to check if we are at a valid block index
  // i do this by calling the bitshift AND operator
  bool block_read;
  block_read = (pos & ((BLOCK * 8) - 1)) == 0 ? true : false;
  // if this is true, and loop through the block and dole out the block
  // then, read new values into the block
  if (block_read == true) {
    uint32_t i = 0;
    while (i < BLOCK) {
      buf1[i] = 0;
      i += 1;
    }
    int j = read_bytes(infile, buf1, BLOCK);
    // if the read_bytes function returns -1, then there are no
    // more bytes that can be read, so I return false
    if (j == -1) {
      return false;
    }
    // if this isn't true, then I reset the position to 0 so I
    // can start over in the block
    pos = 0;
  }
  // these variables grab the position and location of the bit inside
  // the byte
  uint64_t position = pos / 8;
  uint64_t location = pos % 8;
  // uses bitshifting to set the bit's value to the retrieved bit value
  *bit = ((buf1[position] >> (location)) & 1UL);
  // increments the position by 1 (bit's position) to account for bit
  // just getting set
  pos += 1;
  // return true if all things pass correctly
  return true;
}

// function to write from a code structure to an outfile
void write_code(int outfile, Code *c) {
  // first, check if the code exists
  if (c != NULL) {
    // if it does, then loop through the code size and get the
    // position and location of the bit in the byte
    for (uint32_t i = 0; i < code_size(c); i += 1) {
      uint32_t position = pos1 / 8;
      uint32_t location = pos1 % 8;
      // check if the current bit is 1 or 0
      if (code_get_bit(c, i) == 1) {
        // if it is 1, then set the bit
        buf2[position] |= (1UL << location);
        // else, clear the bit
      } else {
        buf2[position] &= ~(1UL << location);
      }
      // increment the position by 1 to account for a new
      // bit getting set
      pos1 += 1;
      bool block_full;
      // initialize a boolean to check if we are at a valid
      // block index
      block_full = (pos1 & (BLOCK * 8) - 1) == 0 ? true : false;
      // if we are, then write these values to an outfile
      // then, dole out the block to and restart it by setting
      // the position back to 0
      if (block_full == true) {
        uint32_t i = 0;
        write_bytes(outfile, buf2, BLOCK);
        while (i < BLOCK) {
          buf2[i] = 0;
          i += 1;
        }
        pos1 = 0;
      }
    }
  }
}

// function to flush out all remaining codes (bytes)
void flush_codes(int outfile) {
  // boolean to return true if the position is in a valid byte position
  // or not
  // if it is (meaning it can be divisible by 8), then flush out all
  // remaining bytes as normal
  // if it is odd, then flush out all remaining bytes plus 1, to account
  // for lost byte amounts
  bool block_full;
  block_full = (pos1 & ((8) - 1)) == 0 ? true : false;
  if (block_full == true) {
    write_bytes(outfile, buf2, (pos1 / 8));
  } else {
    write_bytes(outfile, buf2, (pos1 / 8) + 1);
  }
}
