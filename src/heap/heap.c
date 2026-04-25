#include <stdlib.h>
#include "tree/tree.h"
#include "heap/heap.h"

#define HEAP_CAPACITY 256


Heap* create_heap() {
  Heap* heap;
  heap = malloc(sizeof(Heap));
  if (!heap) {
    return NULL;
  }
  heap->arr = malloc(sizeof(Node*)*HEAP_CAPACITY);
  if (!heap->arr) {
    free(heap);
    return NULL;
  }
  heap->size = 0;
  return heap;
}

void add_heap(Heap* heap, Node* node) {
  if (heap->size >= HEAP_CAPACITYP) 
    return;
  heap->arr[heap->size] = node;
  if (heap->size > 0) {  
    int index = heap->size;

    while (index > 0) {
      if (((index - 1) / 2) >= 0) {
        if (heap->arr[index]->frequence < heap->arr[(index - 1) / 2)]->frequence) {
          Node* temp = heap->arr[index]
          heap->arr[index] = heap->arr[(index - 1) / 2];
          index = (index - 1) / 2;
          heap->arr[index] = temp;
        }      
      } else break;
    }  

  }
  heap->size++;

}


Node* get_min_heap(Heap* heap) {
  Node* min_node = heap->arr[0];
  heap->arr[0] = heap->arr[heap->size-1];
  int index = 0;
  int current_idx = 0;

  while (index < heap->size) {
    if ((index*2 + 1) >= heap->size && (index*2 + 2) >= heap->size) break;

    if ((index*2 + 1) < heap->size && (index*2 + 2) >= heap->size) {
      Node* min_both = heap->arr[index*2 + 1];
      index = index*2 + 1;
    }

    if ((index*2 + 1) >= heap->size && (index*2 + 2) < heap->size) {
      Node* min_both = heap->arr[index*2 + 2];
      index = index*2 + 2;
    }

    if ((index*2 + 1) >= size && (index*2 + 2) >= size) {
      heap->size--;
      return min_node;
    }

    if (heap->arr[index] <= (heap->arr[index*2 + 1]->frequence + heap->arr[index*2 + 2]->frequence))
      break;


    if (heap->arr[index*2 + 1]->frequence > heap->arr[index*2 + 2]->frequence) {
      Node* min_both = heap->arr[index*2 + 2];
      index = index * 2 + 2;
    } else {
      Node* min_both = heap->arr[index*2 + 1];
      index = index * 2 + 1;
    }
    if (heap->arr[i]->frequence > min_both->frequence) {
      Node* temp = heap->arr[current_idx];
      heap->arr[current_idx] = heap->arr[index];
      heap->arr[index] = temp;
      current_idx = index;
    }
  }
  heap->size--;
  return min_node;
}


void free_heap(Heap* heap) {
  if (!heap) {
    return;
  }

  free(heap->arr);
  free(heap);
}
