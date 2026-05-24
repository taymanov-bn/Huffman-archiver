#ifndef HEAP_H
#define HEAP_H

#include "tree/tree.h"


typedef struct {
  Node** arr;
  int size;
} Heap;

Heap* create_heap();
void add_heap(Heap* arr, Node* node);
Node* get_min_heap(Heap* arr);
void free_heap(Heap* arr);







#endif

