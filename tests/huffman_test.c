#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "heap.h"
#include "huffman_tree.h"
#include "error.h"

void test_frequency_counter(char* filename, size_t* frequency) {
  size_t* freq = frequency_counter(filename);
  int result = 1; 
  for (int i = 0; i < 256; i++) {
    result = result && (freq[i] == frequency[i]);
  } 

  free(freq);
  assert(result);
  printf("test_frequency_counter PASSED\n");
}

void test_heap_build_freq(size_t* frequency) {
  Heap* heap_freq = heap_build_freq(frequency);

  if (!heap_freq) return;

  int result = 1; 
  size_t freq_counter = 0;

  for (int i = 0; i < 256; i++) {
    if (frequency[i] > 0) freq_counter++; 
  }

  for (int i = 0; i < heap_freq->size; i++) {
    result = result && (heap_freq->arr[0]->frequency <= heap_freq->arr[i]->frequency); 
  }

  result = result && (heap_freq->size == freq_counter); 

  for (int i = 0; i < heap_freq->size; i++) {
    free_tree(heap_freq->arr[i]); 
  }

  free_heap(heap_freq);
  assert(result);
  printf("test_heap_build_freq PASSED\n");
}


void test_build_huffman_tree(Heap* heap, size_t* freq) {
  Node* huffman_tree = build_huffman_tree(heap);
  if (!huffman_tree) return;
  size_t sum = 0;
  int result = 1; 
  
  for (int i = 0; i < 256; i++) {
    sum += freq[i];
  } 

  result = result && (huffman_tree->frequency == sum); 
  
  free_tree(huffman_tree);

  assert(result);
  printf("test_build_huffman_tree PASSED \n");
}

void test_symbols_code(Node* root) {
  char** codes = symbols_code(root);
  if (!codes) return;

  int result = 1;
  
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 256; j++) {
      if (i == j) continue;
      if (codes[i] && codes[j]) { 
        result = result && (strncmp(codes[i], codes[j], strlen(codes[i])) != 0);
      }
    }
  }
  for (int i = 0; i < 256; i++) {
    free(codes[i]);
  }

  free(codes);
  free_tree(root);

  assert(result);
  printf("test_symbols_code PASSED\n");
}



int main() {
  FILE* freq_file = fopen("testfile.txt", "w");
  if (!freq_file) {
    print_error("Failed to open freq test file");
    return 1;
  }
  fprintf(freq_file, "aababccc");
  fclose(freq_file);

  size_t freq_counter[256] = { ['a'] = 3, ['b'] = 2, ['c'] = 3 };
  
  test_frequency_counter("testfile.txt", freq_counter);

  
  size_t* freq_heap = calloc(256, sizeof(size_t));
  if (!freq_heap) {
    print_error("Failed to allocate memory for freq_heap test");
    return 1;
  }
  freq_heap['a'] = 5;
  freq_heap['b'] = 7;
  freq_heap['c'] = 2;
  test_heap_build_freq(freq_heap);
  free(freq_heap);
    
  FILE* file_huffman = fopen("testhuffman.txt", "w");
  if (!file_huffman) {
    print_error("Failed to open huffman test file");
    return 1;
  }
  fprintf(file_huffman, "hello world!");
  fclose(file_huffman);

  size_t* freq_huffman = frequency_counter("testhuffman.txt");
  if (!freq_huffman) {
    return 1;
  }
  Heap* heap_huffman = heap_build_freq(freq_huffman);
  if (!heap_huffman) {
    return 1;
  }
  test_build_huffman_tree(heap_huffman, freq_huffman);
  free(freq_huffman);
  
  FILE* file_codes = fopen("testcodes.txt", "w");
  if (!file_codes) {
    print_error("Failed to open codes test file");
    return 1;
  }

  fprintf(file_codes, "huffman codes test hello world!");
  fclose(file_codes);

  size_t* freq_codes = frequency_counter("testcodes.txt");
  if (!freq_codes) {
    return 1;
  }
  Heap* heap_codes = heap_build_freq(freq_codes);
  if (!heap_codes) {
    return 1;
  }
  Node* tree_huffman = build_huffman_tree(heap_codes);
  if (!tree_huffman) {
    return 1;
  }

  test_symbols_code(tree_huffman);

  free(freq_codes);
  return 0;
}
