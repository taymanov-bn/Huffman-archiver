#include <stdlib.h>
#include "tree/tree.h"
#include "heap/heap.h"

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
