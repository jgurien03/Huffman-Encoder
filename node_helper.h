#pragma once

uint32_t l_child(uint32_t n);

uint32_t r_child(uint32_t n);

uint32_t parent(uint32_t n);

void swap(Node *x, Node *y);

void up_heap(Node **arr, uint32_t n);

void down_heap(Node **arr, uint32_t heap_size);
