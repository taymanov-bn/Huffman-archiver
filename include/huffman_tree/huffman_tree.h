// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "tree/tree.h"
#include "heap/heap.h"

int* frequency_counter(const char* filename);
Heap* heap_build_freq(int* frequency);
Node* build_huffman_tree(Heap* heap);
char* dfs_codes (Node* root, char* buffer, char** code, int* index);
void free_codes(char** codes);
char** symbols_code(Node* root);

#endif
