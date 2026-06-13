// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#ifndef TREE_H
#define TREE_H

typedef struct Node {
    // символ (байт)
    unsigned char symbol;
    // частота символа
    size_t frequency;
    // левый ребенок
    struct Node* left;
    // правый ребенок
    struct Node* right;
} Node;

// создает новый узел дерева
Node* create_node(unsigned char symbol, size_t frequency, Node *left, Node *right);

// рекурсивно освобождает память дерева
void free_tree(Node *tree);

#endif
