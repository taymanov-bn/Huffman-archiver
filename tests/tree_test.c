#include <stdio.h>
#include <assert.h>
#include "tree/tree.h"

void test_create_node(unsigned char symbol, size_t freq, Node* left, Node* right) {
  Node* node = create_node(symbol, freq, left, right);
  int test_result = (node != NULL && node->symbol == symbol &&
                     node->frequency == freq && node->left == left &&
                     node->right == right);

  free_tree(node);
  assert(test_result);
  printf("test_create_node PASSED\n");
}

void test_free_tree(Node* tree) {
  free_tree(tree);
  printf("test_free_tree PASSED\n");
}

int main() {

  // проверка на дерево без детей
  test_create_node('a', 3, NULL, NULL);

  // проверка на дерево с детьми
  Node* left_node = create_node('b', 5, NULL, NULL);
  Node* right_node = create_node('c', 7, NULL, NULL);
  test_create_node('a', 3, left_node, right_node);
  
  // проверка на очистку памяти  
  Node* left_node_free = create_node('b', 5, NULL, NULL);
  Node* right_node_free = create_node('c', 7, NULL, NULL);
  Node* parent_free = create_node('a', 3, left_node_free, right_node_free);
  test_free_tree(parent_free);

  return 0;
  
}
