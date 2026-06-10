// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan


#ifndef HEAP_H
#define HEAP_H

#include "tree/tree.h"


typedef struct { // структура кучи 
  Node** arr; // массив узлов 
  int size; // размер кучи (массива) 
} Heap;

Heap* create_heap();
void add_heap(Heap* arr, Node* node);
Node* get_min_heap(Heap* arr);
void free_heap(Heap* arr);

#endif

