#include <stdio.h>
#include "heap.h"
#include "tree.h"
#include <assert.h>

void test_create_heap() {
  Heap* heap = create_heap();
  int result = (heap != NULL);
  free_heap(heap);
  assert(result);
  printf("test_create_heap PASSED\n");
}

void test_add_heap(Heap* heap, Node* node) {
  int size = heap->size;
  add_heap(heap, node);
  int result = (heap->size == size + 1);
  free_heap(heap);
  assert(result);
  printf("test_add_heap PASSED\n");
}

void test_get_min_heap(Heap* heap) {
  int size = heap->size;
  Node* min_node = get_min_heap(heap);
  int result = (size == heap->size + 1);

  for (int i = 0; i < heap->size; i++) {
    result = result && (min_node->frequency <= heap->arr[i]->frequency);
    free_tree(heap->arr[i]);
  }
  
  free_heap(heap);
  free_tree(min_node);

  assert(result);
  printf("test_get_min_heap PASSED \n");
}

int main() {
  test_create_heap();

  Heap* single_heap = create_heap();
  Node* single_node = create_node('a', 4, NULL, NULL);
  test_add_heap(single_heap, single_node);
  free_tree(single_node);

  Heap* heap = create_heap();
  Node* node1 = create_node('a', 3, NULL, NULL);
  Node* node2 = create_node('d', 5, NULL, NULL);
  Node* node3 = create_node('j', 8, NULL, NULL);
  Node* node4 = create_node('q', 9, NULL, NULL);
  Node* arr[] = {node1, node2, node3, node4};

  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
    add_heap(heap, arr[i]);
  }
  test_get_min_heap(heap);
  return 0;
}
