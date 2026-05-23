#include <stdlib.h>
#include <stdint.h>
#include "huffman_tree/huffman_tree.h"
#include "heap/heap.h"
#include "tree/tree.h"

#define FREQUENCY_SIZE 256

int decode (char* file_input, char* file_output) {
  if (!file_input || !file_output) return -1;

  FILE* inp_file = fopen(file_input, "rb");
  if (!inp_file) return -1;

  FILE* out_file = fopen(file_output, "wb");
  if (!out_file) {
    fclose(inp_file);
    return -1;
  }

  int* freq_arr = calloc(256, sizeof(int));
  if (!freq_arr) {
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  if (!fread(freq_arr, sizeof(int), FREQUENCY_SIZE, inp_file)) {
    free(freq_arr);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Heap* heap = heap_build_freq(freq_arr);
  if (!heap) {
    free(freq_arr);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Node* tree = build_huffman_tree(heap);
  if (!tree) {
    free(freq_arr);
    free_heap(heap);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }






}
