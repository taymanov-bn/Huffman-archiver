#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "tree/tree.h"
#include "heap/heap.h"

int* frequence_counter(const char* filename);
Heap* heap_build_freq(int* freq);
Node* build_huffman_tree(Heap* heap);
