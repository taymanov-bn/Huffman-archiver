// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "tree.h"
#include "heap.h"

// подсчитывает частоты символов в файле
size_t* frequency_counter(const char* filename);

// строит кучу из таблицы частот
Heap* heap_build_freq(size_t* frequency);

// строит дерево хаффмана из кучи
Node* build_huffman_tree(Heap* heap);

// освобождает массив кодов
void free_codes(char** codes);

// возвращает таблицу кодов для каждого символа
char** symbols_code(Node* root);

#endif
