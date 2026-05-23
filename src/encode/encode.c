#include <stdlib.h>
#include <stdint.h>
#include "tree/tree.h"
#include "heap/heap.h"
#include "huffman_tree/huffman_tree.h"

#define FREQUENCY_SIZE 256

int encode(char* file_input, char* file_output) {
  if (!file_input || !file_output) return -1;

  FILE* inp_file = fopen(file_input, "rb");
  if (!inp_file) return -1;

  FILE* out_file = fopen(file_output, "wb");
  if (!out_file) {
    fclose(inp_file);
    return -1;
  }

  int* freq = frequency_counter(file_input);
  if (!freq) {
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Heap* heap = heap_build_freq(freq);
  if (!heap) {
    free(freq);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Node* tree = build_huffman_tree(heap);
  if (!tree) {
    free(freq);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  char** codes = symbols_code(tree);
  if (!codes) {
    free(freq);
    free_tree(tree);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  if(!fwrite(freq, sizeof(int), FREQUENCY_SIZE, out_file)) return -1;
  
  uint8_t buffer_byte = 0;
  int bit_count = 0;
  size_t symbol_code_size = 0;
  char symbol;

  while (fread(&symbol, sizeof(char), 1, inp_file) == 1) {
    
    char* code = codes[symbol];
    symbol_code_size = strlen(code);
    
    for (int i = 0; i < symbol_code_size; i++) {
        if (!fwrite(buffer_byte, sizeof(char), 1, out_file)) {
          free(freq);
          free_tree(tree);
          fclose(inp_file);
          fclose(out_file);
        };
        bit_count = 0;
        buffer_byte = 0;
      }
     
      buffer_byte = buffer_byte | ((code[i) - '0') << (7 - bit_count);
      bit_count++;
    }

  }



  




  return 0;
}
