#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree/tree.h"
#include "heap/heap.h"
#include "huffman_tree/huffman_tree.h"
#include "error/error.h"

void test_frequency_counter(char* filename, int* frequency) {
  int* freq = frequency_counter(filename);
  int result = 1; // 1 для того, чтобы в цикле не было проблем
  for (int i = 0; i < 256; i++) {
    result = result && (freq[i] == frequency[i]);
  } // проверка на одинаковую частоту байтов

  free(freq);
  assert(result);
  printf("test_frequency_counter PASSED\n");
}

void test_heap_build_freq(int* frequency) {
  Heap* heap_freq = heap_build_freq(frequency);

  if (!heap_freq) return;

  int result = 1; // аналогично
  int freq_counter = 0;

  for (int i = 0; i < 256; i++) {
    if (frequency[i] > 0) freq_counter++; // подсчет количества байтов имеющих частоту больше 0
  }

  for (int i = 0; i < heap_freq->size; i++) {
    result = result && (heap_freq->arr[0]->frequency <= heap_freq->arr[i]->frequency); // проверка на приоритетность очереди
  }

  result = result && (heap_freq->size == freq_counter); // проверка, что размер кучи равен количеству байтов у которых частота > 0 

  for (int i = 0; i < heap_freq->size; i++) {
    free_tree(heap_freq->arr[i]); // освобождение узлов внутри кучи
  }

  free_heap(heap_freq);
  assert(result);
  printf("test_heap_build_freq PASSED\n");
}


void test_build_huffman_tree(Heap* heap, int* freq) {
  Node* huffman_tree = build_huffman_tree(heap);
  if (!huffman_tree) return;
  int sum = 0;
  int result = 1; // аналогично 
  
  for (int i = 0; i < 256; i++) {
    sum += freq[i];
  } // проверка суммы частот чтобы дальше проверить корень созданного дерева 

result = result && (huffman_tree->frequency == sum); // а вот и проверка 
  
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
      // двойной цикл чтобы проверить префиксы 
      if (i == j) continue;
      if (codes[i] && codes[j]) { // проверка на то, что они оба не NULL
        // проверка, что закодированный код (байт) не является началом другого
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

  // тест функции test_frequency_counte
  
  FILE* freq_file = fopen("testfile.txt", "w");
  if (!freq_file) {
    print_error("Failed to open freq test file");
    return 1;
  }
  fprintf(freq_file, "aababccc");
  fclose(freq_file);

  int freq_counter[256] = { ['a'] = 3, ['b'] = 2, ['c'] = 3 };
  
  test_frequency_counter("testfile.txt", freq_counter);

  // тест test_heap_build_freq
  
  int* freq_heap = calloc(256, sizeof(int*));
  if (!freq_heap) {
    print_error("Failed to allocate memory for freq_heap test");
    return 1;
  }
  freq_heap['a'] = 5;
  freq_heap['b'] = 7;
  freq_heap['c'] = 2;
  test_heap_build_freq(freq_heap);
  free(freq_heap);

  // тест test_build_huffman_tree
    
  FILE* file_huffman = fopen("testhuffman.txt", "w");
  if (!file_huffman) {
    print_error("Failed to open huffman test file");
    return 1;
  }
  fprintf(file_huffman, "hello world!");
  fclose(file_huffman);

  int* freq_huffman = frequency_counter("testhuffman.txt");
  if (!freq_huffman) {
    return 1;
  }
  Heap* heap_huffman = heap_build_freq(freq_huffman);
  if (!heap_huffman) {
    return 1;
  }
  test_build_huffman_tree(heap_huffman, freq_huffman);
  free(freq_huffman);


  // тест на test_symbols_code 
  
  FILE* file_codes = fopen("testcodes.txt", "w");
  if (!file_codes) {
    print_error("Failed to open codes test file");
    return 1;
  }

  fprintf(file_codes, "huffman codes test hello world!");
  fclose(file_codes);

  int* freq_codes = frequency_counter("testcodes.txt");
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
