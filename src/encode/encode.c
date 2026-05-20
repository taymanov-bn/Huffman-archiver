#include <stdlib.h>
#include "tree/tree.h"
#include "heap/heap.h"
#include "huffman_tree/huffman_tree.h"

FILE* encode(char* file_input, char* file_output) {
  if (!file_input || !file_output) return NULL;

  FILE* inp_file = fopen(file_input, "rb");
  if (!inp_file) return NULL;

  FILE* out_file = fopen(file_output, "wb");
  if (!out_file) {
    fclose(inp_file);
    return NULL;
  }

  int* freq = frequency_counter(file_input);
  if (!freq) {
    fclose(inp_file);
    fclose(out_file);
    return NULL;
  }

  Heap* heap = heap_build_freq(freq);
  if (!heap) {
    free(freq);
    fclose(inp_file);
    fclose(out_file);
    return NULL;
  }

  Node* tree = build_huffman_tree(heap);
  if (!tree) {
    free(freq);
    free_heap(heap);
    fclose(inp_file);
    fclose(out_file);
    return NULL;
  }

  char** codes = symbols_code(tree);
  if (!codes) {
    free(freq);
    free_heap(heap);
    free_tree(tree);
    fclose(inp_file);
    fclose(out_file);
    return NULL;
  }

  while ()



  





}
