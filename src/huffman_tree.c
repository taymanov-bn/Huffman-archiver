// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "heap.h"
#include <string.h>
#include "error.h"

#define PARENT_SYMBOL 0

size_t* frequency_counter(const char* filename) {
  unsigned char byte;
  FILE* file = fopen(filename, "rb");
  if (!file) {
    print_error("Failed to open file for frequency counting");
    return NULL;
  }

  size_t* frequency_array = calloc(256, sizeof(size_t));
  if (!frequency_array) {
    print_error("Failed to allocate memory for frequency array");
    fclose(file);
    return NULL;
  }

  while (fread(&byte, sizeof(byte), 1, file) == 1) {
    frequency_array[byte]++;
  }

  fclose(file);
  return frequency_array;
}

Heap* heap_build_freq(size_t* frequency) {
  if (!frequency) {
    print_error("Invalid frequency array");
    return NULL;
  }

  Heap* heap = create_heap();
  if (!heap) {
    return NULL;
  }

  int node_counter = 0;

  for (int i = 0; i < 256; i++) {
    if (frequency[i] > 0) {
      Node* node = create_node(i, frequency[i], NULL, NULL);
      if (!node) {
        free_heap(heap);
        return NULL;
      }
      add_heap(heap, node);
      node_counter++;
    }
  }

  if (node_counter == 0) {
    free_heap(heap);
    return NULL;
  }

  return heap;
}


Node* build_huffman_tree(Heap* heap) {
  if (!heap) {
    print_error("Invalid heap");
    return NULL;
  }

  while (heap->size > 1) {
    Node* left_node = get_min_heap(heap);
    if (!left_node) {
      free_heap(heap);
      print_error("Failed to get min node from heap");
      return NULL;
    }

    Node* right_node = get_min_heap(heap); 
    if (!right_node) {
      free_tree(left_node);
      free_heap(heap);
      print_error("Failed to get min node from heap");
      return NULL;
    }
    
    Node* parent = create_node(PARENT_SYMBOL, left_node->frequency + right_node->frequency,
                               left_node, right_node);
    if (!parent) {
      free_tree(left_node);
      free_tree(right_node);
      free_heap(heap);
      print_error("Failed to create parent node");
      return NULL;
    }

    add_heap(heap, parent);

  }

  if (heap->size == 1) {
    Node* huffman_tree = heap->arr[0];
    free_heap(heap);
    return huffman_tree;
  }
  
  print_error("Failed to build huffman tree");
  return NULL;
}

void free_codes(char** codes) {
  if (!codes) {
    return;
  }

  for (int i = 0; i < 256; i++) {
    if (codes[i]) {
      free(codes[i]);
    }
  }
  free(codes);
}

char* dfs_codes(Node* root, char* buffer, char** code, int* index) {
  if (!root) return NULL;

  if (root->left == NULL && root->right == NULL) {
    if (*index == 0) {
      buffer[0] = '0';
      *index = 1;
    }
    if (!(code[root->symbol] = malloc(*index + 1))) {
      print_error("Failed to allocate memory for symbol code");
      return NULL;
    }
    buffer[*index] = '\0';
    memcpy(code[root->symbol], buffer, *index + 1);
    return code[root->symbol];
  }

  buffer[*index] = '0'; 
  (*index)++; 
  dfs_codes(root->left, buffer, code, index); 
  (*index)--; 
  buffer[*index] = '1'; 
  (*index)++;
  dfs_codes(root->right, buffer, code, index);
  (*index)--; 
  return NULL;
}

char** symbols_code(Node* root) {
  char** codes = calloc(256, sizeof(char*)); 

  if (!codes) {
    print_error("Failed to allocate memory for codes table");
    return NULL;
  }

  char buffer[256] = {};
  int index = 0;
  dfs_codes(root, buffer, codes, &index); 
  return codes;
}
