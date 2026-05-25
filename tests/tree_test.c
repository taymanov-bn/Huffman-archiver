#include <stdio.h>
#include <assert.h>
#include "tree/treee.h"

void test_create_node_null(unsigned char symbol, size_t freq, Node* left, Node* right) {
  Node* node = create_node(symbol, freq, left, right);
  assert(node != NULL);
  assert(node->symbol == symbol);
  assert(node->frequency == freq);
  assert(node->left == NULL);
  assert(node->right == NULL);
  printf("test_create_node PASSED\n");
}

void test_create_node_with_children(unsigned char symbol, size_t freq, Node* left, Node* right) {
  Node* node = create_node(symbol, freq, left, right);
  assert(node != NULL);
  assert(node->symbol == symbol);
  assert(node->frequency == freq);
  assert(node->left == left);
  assert(node->right == right);
  printf("test_create_node_with_children PASSED\n");

}
