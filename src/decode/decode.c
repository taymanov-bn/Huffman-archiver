#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "huffman_tree/huffman_tree.h"
#include "heap/heap.h"
#include "tree/tree.h"
#include "error/error.h"

#define FREQUENCY_SIZE 256

int decode (char* file_input, char* file_output) {
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

  int* freq_arr = calloc(256, sizeof(int));
  if (!freq_arr) {
    print_error("Failed to allocate memory for frequency array during decoding");
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  if (!fread(freq_arr, sizeof(int), FREQUENCY_SIZE, inp_file)) {
    free(freq_arr);
    fclose(inp_file);
    fclose(out_file);
    print_error("Failed to read frequency table to freq array");
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

  unsigned char byte = 0;
  uint8_t buffer_byte = 0;
  int freq_counter = 0;

  for (int i = 0; i < 256; i++) {
    if (freq_arr[i] > 0) freq_counter += freq_arr[i];
  }

  Node* current_node = tree;


  while (fread(&byte, sizeof(char), 1, inp_file) == 1) {
    if (freq_counter == 0) break;

    for (int i = 0; i < 8; i++) {

      buffer_byte = (byte >> (7 - i)) & 1;

      if (buffer_byte == 0) {
        if (current_node->left != NULL) {
          current_node = current_node->left;
          if (current_node->left == NULL && current_node->right == NULL){
            if (!fwrite(&current_node->symbol, sizeof(char), 1, out_file)) {
              free(freq_arr);
              free_tree(tree);
              fclose(inp_file);
              fclose(out_file);
              print_error("Failed to write decoded data");
              return -1;
            }
            freq_counter--;
            if (freq_counter == 0) break;
            current_node = tree;
          }
        }
      }

      if (buffer_byte == 1) {
        if (current_node->right != NULL) {
          current_node = current_node->right;
          if (current_node->left == NULL && current_node->right == NULL){
            if (!fwrite(&current_node->symbol, sizeof(char), 1, out_file)) {
              free(freq_arr);
              free_tree(tree);
              fclose(inp_file);
              fclose(out_file);
              print_error("Failed to write decoded data");
              return -1;
            }
            freq_counter--;
            if (freq_counter == 0) break;
            current_node = tree;
          }
        }
      }
    }
    if (freq_counter == 0) break;
  }
  
  free(freq_arr);
  free_tree(tree);
  fclose(inp_file);
  fclose(out_file);
  return 0;

}
