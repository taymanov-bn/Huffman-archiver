#include <stdio.h>
#include <stdlib.h>
#include "tree.h"


Node* create_node(unsigned char symbol, size_t freq, Node* left, Node* right) {
  Node* node = malloc(sizeof(struct Node));
  if (!node)
    return NULL;
  
  node->symbol = symbol;
  node->frequency = freq;
  node->left = left;
  node->right = right;
  return node;

}

void free_tree(Node* tree) {
  if (!tree) {
    return;
  }
  
  if (tree->left != NULL || tree->right != NULL) {
    if (tree->left != NULL) {
      free_tree(tree->left);
    }
    if (tree->right != NULL) {
      free_tree(tree->right);
    }
  }

  free(tree);

}
