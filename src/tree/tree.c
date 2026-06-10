// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#include <stdio.h>
#include <stdlib.h>
#include "tree/tree.h"
#include "error/error.h"


Node* create_node(unsigned char symbol, size_t freq, Node* left, Node* right) {
  Node* node = malloc(sizeof(struct Node)); // выделение памяти для узла
  if (!node) {
    // проверка на выделение памяти, если не выделилось - возврощаю ошибку
    print_error("Failed to allocate memory for node");
    return NULL;
  }
  
  // после выделения памяти и проверки, присваиваем полученные на вход данные этому узлу 
  node->symbol = symbol; 
  node->frequency = freq;
  node->left = left;
  node->right = right;
  return node;

}

void free_tree(Node* tree) { // освобождение памяти дерева
  if (!tree) {
    return;
  }
  // дальше проверка на детей, если они есть, то через рекурсию освобождаем память и для детей 
  if (tree->left != NULL || tree->right != NULL) {
    if (tree->left != NULL) {
      free_tree(tree->left);
    }
    if (tree->right != NULL) {
      free_tree(tree->right);
    }
  }

  free(tree); // в конце освобождаем корень дерева 

}
