#ifndef TREE_H
#define TREE_H


typedef struct Node {
  unsigned char symbol;
  size_t frequency;
  struct Node* left;
  struct Node* right;

} Node;

Node* create_node(unsigned char symbol, size_t freq, Node* left, Node* right);
void free_tree(Node* tree);

#endif
