// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "huffman_tree.h"
#include "heap.h"
#include "tree.h"
#include "error.h"

#define FREQUENCY_SIZE 256 

int decode(char* file_input, char* file_output) {
  if (!file_input || !file_output) { 
    print_error("Files are not provided");
    return -1;
  }

  FILE* inp_file = fopen(file_input, "rb"); 
  if (!inp_file) {
    print_error("Invalid input file path");
    return -1;
  }

  FILE* out_file = fopen(file_output, "wb"); 
  if (!out_file) {
    fclose(inp_file); 
    print_error("Invalid output file path");
    return -1;
  }

  size_t* frequency_array = calloc(256, sizeof(size_t));  
  if (!frequency_array) {
    print_error("Failed to allocate memory for frequency array during decoding");
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  if (!fread(frequency_array, sizeof(size_t), FREQUENCY_SIZE, inp_file)) {
    free(frequency_array);
    fclose(inp_file);
    fclose(out_file);
    print_error("Failed to read frequency table to freq array");
    return -1;
  }

  Heap* heap = heap_build_freq(frequency_array); 
  if (!heap) {
    free(frequency_array);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Node* tree = build_huffman_tree(heap); 
  if (!tree) {
    free(frequency_array);
    free_heap(heap);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  unsigned char byte = 0;
  uint8_t buffer_byte = 0;
  size_t frequency_counter = 0; 

  for (int i = 0; i < 256; i++) {
    if (frequency_array[i] > 0) frequency_counter += frequency_array[i]; 
  }

  Node* current_node = tree; 

  if (tree->left == NULL && tree->right == NULL) {
  while (frequency_counter > 0) {
    if (!fwrite(&tree->symbol, sizeof(char), 1, out_file)) {
      free(frequency_array);
      free_tree(tree);
      fclose(inp_file);
      fclose(out_file);
      print_error("Failed to write decoded data");
      return -1;
    }
    frequency_counter--;
  }
    free(frequency_array);
    free_tree(tree);
    fclose(inp_file);
    fclose(out_file);
    return 0;
  }
  
  while (fread(&byte, sizeof(char), 1, inp_file) == 1) { 
    if (frequency_counter == 0) break; 

    for (int i = 0; i < 8; i++) {
      buffer_byte = (byte >> (7 - i)) & 1; 

      if (buffer_byte == 0) { 
        if (current_node->left != NULL) { 
          current_node = current_node->left;
          if (current_node->left == NULL && current_node->right == NULL) { 
            if (!fwrite(&current_node->symbol, sizeof(char), 1, out_file)) {
              free(frequency_array);
              free_tree(tree);
              fclose(inp_file);
              fclose(out_file);
              print_error("Failed to write decoded data");
              return -1;
            }
            frequency_counter--; 
            if (frequency_counter == 0) break; 
            current_node = tree; 
          }
        }
      }

      if (buffer_byte == 1) { 
        if (current_node->right != NULL) {
          current_node = current_node->right;
          if (current_node->left == NULL && current_node->right == NULL){
            if (!fwrite(&current_node->symbol, sizeof(char), 1, out_file)) {
              free(frequency_array);
              free_tree(tree);
              fclose(inp_file);
              fclose(out_file);
              print_error("Failed to write decoded data");
              return -1;
            }
            frequency_counter--;
            if (frequency_counter == 0) break;
            current_node = tree;
          }
        }
      }
    }
    if (frequency_counter == 0) break;
  }
  free(frequency_array);
  free_tree(tree);
  fclose(inp_file);
  fclose(out_file);
  return 0;
}
