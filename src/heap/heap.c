#include <stdlib.h>
#include "tree/tree.h"
#include "heap/heap.h"
#include "error/error.h"
#include <stdio.h>

#define HEAP_CAPACITY 256


Heap* create_heap() {
  Heap* heap;
  heap = malloc(sizeof(Heap));
  if (!heap) {
    print_error("Failed to allocate memory for heap");
    return NULL;
  }
  heap->arr = malloc(sizeof(Node*)*HEAP_CAPACITY);
  if (!heap->arr) {
    free(heap);
    print_error("Failed to allocate memory for heap array");
    return NULL;
  }
  heap->size = 0;
  return heap;
}

void add_heap(Heap* heap, Node* node) {
  if (heap->size >= HEAP_CAPACITY) {
    print_error("Heap is full");
    return;
  }
  heap->arr[heap->size] = node;
  if (heap->size > 0) {  
    int index = heap->size;

    while (index > 0) {
      if (heap->arr[index]->frequency < heap->arr[(index - 1) / 2]->frequency) {
        Node* temp = heap->arr[index];
        heap->arr[index] = heap->arr[(index - 1) / 2];
        index = (index - 1) / 2;
        heap->arr[index] = temp;
      }
      else if (heap->arr[index]->frequency ==  heap->arr[(index - 1) / 2]->frequency) {
        if (heap->arr[index]->symbol < heap->arr[(index - 1) / 2]->symbol) {
          Node* temp = heap->arr[index];
          heap->arr[index] = heap->arr[(index - 1) / 2];
          index = (index - 1) / 2;
          heap->arr[index] = temp;
        } else break;
      } else break;
    }  

  }
  heap->size++;

}


Node* get_min_heap(Heap* heap) {

  if (!heap || heap->size == 0) return NULL;

  Node* min_node = heap->arr[0];
  heap->arr[0] = heap->arr[heap->size-1];
  int index = 0;
  int child_idx = 0;
  Node* min_both = NULL;
  heap->size--;

  while (index < heap->size) {

    if ((index*2 + 1) >= heap->size && (index*2 + 2) >= heap->size) {
      break;
    }

    else if ((index*2 + 1) < heap->size && (index*2 + 2) >= heap->size) {
      min_both = heap->arr[index*2 + 1];
      child_idx = index*2 + 1;
    } else {
      if (heap->arr[index*2 + 1]->frequency < heap->arr[index*2 + 2]->frequency) {
        min_both = heap->arr[index*2 + 1];
        child_idx = index*2 + 1;
      } 
      else if (heap->arr[index*2 + 1]->frequency > heap->arr[index*2 + 2]->frequency) {
        min_both = heap->arr[index*2 + 2];
        child_idx = index*2 + 2;
      } else {
        if (heap->arr[index*2 + 1]->symbol < heap->arr[index*2 + 2]->symbol) {
          min_both = heap->arr[index*2 + 1];
          child_idx = index*2 + 1;
        } else {
          min_both = heap->arr[index*2 + 2];
          child_idx = index*2 + 2;

        }
      }
    }

    if (min_both == NULL) break;

    if (heap->arr[index]->frequency > min_both->frequency) {
      Node* temp = heap->arr[index];
      heap->arr[index] = min_both;
      heap->arr[child_idx] = temp;
      index = child_idx;
    } else break;


  }
  return min_node;
}


void free_heap(Heap* heap) {
  if (!heap) {
    return;
  }

  free(heap->arr);
  free(heap);
}
