// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "heap.h"
#include "huffman_tree.h"
#include "error.h"

#define FREQUENCY_SIZE 256

int encode(char* file_input, char* file_output) { 
  if (!file_input || !file_output) {  
    print_error("Files are not provided");
    return -1;
  }  

  FILE* inp_file = fopen(file_input, "rb"); 
  if (!inp_file) return -1; 

  FILE* out_file = fopen(file_output, "wb"); 
  if (!out_file) {
    fclose(inp_file); 
    return -1;
  }

  size_t* frequency = frequency_counter(file_input); 
  if (!frequency) {
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Heap* heap = heap_build_freq(frequency);
  if (!heap) {
    free(frequency);
    fclose(inp_file);
    fwrite(frequency, sizeof(size_t), FREQUENCY_SIZE, out_file);
    fclose(out_file);
    return 0;
  }

  Node* tree = build_huffman_tree(heap); 
  if (!tree) {
    free_heap(heap);
    free(frequency);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  char** codes = symbols_code(tree); 
  if (!codes) {
    free(frequency);
    free_tree(tree);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  if (!fwrite(frequency, sizeof(size_t), FREQUENCY_SIZE, out_file)) { 
    print_error("Failed to write frequency table");
    free(frequency);
    free_tree(tree);
    free_codes(codes);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }
  
  uint8_t buffer_byte = 0; 
  int bit_count = 0; 
  size_t symbol_code_size = 0; 
  unsigned char symbol; 

  while (fread(&symbol, sizeof(char), 1, inp_file) == 1) {
    char* code = codes[symbol]; 
    symbol_code_size = strlen(code); 
    for (size_t i = 0; i < symbol_code_size; i++) { 
      if (bit_count == 8) { 
        if (!fwrite(&buffer_byte, sizeof(char), 1, out_file)) { 
          free(frequency);
          free_tree(tree);
          free_codes(codes);
          fclose(inp_file);
          fclose(out_file);
          print_error("Failed to write compressed data");
          return -1;
        } 

        bit_count = 0;
        buffer_byte = 0;
      }
      buffer_byte = buffer_byte | ((code[i] - '0') << (7 - bit_count)); 
      bit_count++; 
    }
  }

  if (bit_count > 0) { 
    if(!fwrite(&buffer_byte, sizeof(char), 1, out_file)) { 
      free(frequency);
      free_tree(tree);
      free_codes(codes);
      fclose(inp_file);
      fclose(out_file);
      print_error("Failed to write last byte");
      return -1;
    }
  }
 
  free(frequency);
  free_tree(tree);
  free_codes(codes);
  fclose(inp_file);
  fclose(out_file);

  return 0;
}
