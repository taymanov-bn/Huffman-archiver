// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#ifndef TREE_H
#define TREE_H


typedef struct Node { // структура узла 
  unsigned char symbol; // символ (байт)
  size_t frequency; // частота символа 
  struct Node* left; // левый ребенок 
  struct Node* right; // правый ребенок 
} Node;

Node* create_node(unsigned char symbol, size_t freq, Node* left, Node* right); 
void free_tree(Node* tree);

#endif
