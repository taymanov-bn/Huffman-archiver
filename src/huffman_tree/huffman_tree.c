#include <stdlib.h>
#include "tree/tree.h"
#include "heap/heap.h"

#define PARENT_SYMBOL 0

int* frequence_counter(const char* filename) {
  unsigned char byte;

  FILE* file = fopen(filename, "rb");
  if (!file) {
    return NULL;
  }

  int* freq_arr = calloc(256, sizeof(int));

  if (!freq_arr) {
    return NULL;
  }

  while (fread(&byte, sizeof(char), 1, file) == 1) {
    freq_arr[byte]++;
  }

  fclose(file);
  return freq_arr;


}

Heap* heap_build_freq(int* freq) {
  if (!freq) {
    return NULL;
  }

  Heap* heap = create_heap();
  int node_counter = 0;

  if (!heap) {
    return NULL;
  }
  
  for (int i = 0; i < 256; i++) {
    if (freq[i] > 0) {
      Node* node = create_node(i, freq[i], NULL, NULL);
      if (!node) {
        free_heap(heap);
        return NULL;
      }
      if (!add_heap(heap, node)) {
        free_heap(heap);
        return NULL;
      }
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
    return NULL;
  }

  while (heap->size > 1) {
    Node* left_node = get_min_heap(heap);
    if (!left_node) {
      free_heap(heap);
      return NULL;
    }

    Node* right_node = get_min_heap(heap);
    if (!right_node) {
      free_node(left_node);
      free_heap(heap);
      return NULL;
    }
    
    Node* parent = create_node(PARENT_SYMBOL, left_node->frequency + right_node->frequency,
                               left_node, right_node);
    if (!parent) {
      free_tree(left_node);
      free_tree(right_node);
      free_heap(heap);
      return NULL;
    }
    if (!add_heap(heap, parent)) {
      free_tree(left_node);
      free_tree(right_node);
      free_tree(parent);
      free_heap(heap);
      return NULL;
    }

  }

  if (heap->size == 1) {
    Node* huffman_tree = heap->arr[0];
    free_heap(heap);
    return huffman_tree;
  }

  return NULL;


}
