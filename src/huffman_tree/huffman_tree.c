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

} 
