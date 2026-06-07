#include <stdlib.h>
#include <stdio.h>
#include "tree/tree.h"
#include "heap/heap.h"
#include <string.h>
#include "error/error.h"

#define PARENT_SYMBOL 0 // символ для родителя (не лучший выбор)

int* frequency_counter(const char* filename) { // функция по подсчету частот
  unsigned char byte;

  FILE* file = fopen(filename, "rb"); // открываем файл для чтения в бинарном режиме 
  if (!file) {
    print_error("Failed to open file for frequency counting");
    return NULL;
  }

  int* freq_arr = calloc(256, sizeof(int)); // выделяем память для массива с частотами (calloc чтобы обнулить все значения внутри)

  if (!freq_arr) {
    // проверка на выделение памяти 
    print_error("Failed to allocate memory for frequency array");
    return NULL;
  }

  while (fread(&byte, sizeof(byte), 1, file) == 1) { // цикл для побайтового чтения из файла и записи частот в массив частот  
    freq_arr[byte]++;
  }

  fclose(file); // закрываем поток файла 
  return freq_arr; // возвращаем массив с частотами


}

Heap* heap_build_freq(int* freq) { // функция для построения узлов и добавления их в массив кучи 
  if (!freq) {
    // проверка на массив с частотами из входных данных 
    print_error("Invalid frequency array");
    return NULL;
  }

  Heap* heap = create_heap(); // создаем кучу 
  int node_counter = 0; // переменная для подсчета узлов

  if (!heap) {
    // проверка на создание кучи 
    return NULL;
  }
  
  for (int i = 0; i < 256; i++) { // цикл для создания узлов и добавления их в массив кучи 
    if (freq[i] > 0) { // ну и создаем узлы только для частот > 0
      Node* node = create_node(i, freq[i], NULL, NULL); // создание самих узлов 
      if (!node) {
        // проверка на их создание 
        free_heap(heap);
        return NULL;
      }
      add_heap(heap, node); // добавляем созданный узел в массив кучи 
      node_counter++;
    }
  }

  if (node_counter == 0) { // проверка, если переменная по подсчету узлов = 0, то выводим ошибку
    free_heap(heap);
    print_error("Empty file or no valid symbols");
    return NULL;
  }

  return heap; // возвращаем кучу с узлами 

} 


Node* build_huffman_tree(Heap* heap) { // функция для построения дерева Хаффмана 
  if (!heap) {
    // проверка на входные данные 
    print_error("Invalid heap");
    return NULL;
  }

  while (heap->size > 1) { // цикл для создания дерева 
    Node* left_node = get_min_heap(heap); // создаем узел с левым ребенком 
    if (!left_node) {
      // проверка на создание левого ребенка 
      free_heap(heap);
      print_error("Failed to get min node from heap");
      return NULL;
    }

    // аналогично с правым ребенком
    Node* right_node = get_min_heap(heap); 
    if (!right_node) {
      free_tree(left_node);
      free_heap(heap);
      print_error("Failed to get min node from heap");
      return NULL;
    }
    
    Node* parent = create_node(PARENT_SYMBOL, left_node->frequency + right_node->frequency,
                               left_node, right_node); // создаем родителя с уже ранее созданным узлами детьми 
    if (!parent) {
      // проверка на создание родителя 
      free_tree(left_node);
      free_tree(right_node);
      free_heap(heap);
      print_error("Failed to create parent node");
      return NULL;
    }

    add_heap(heap, parent); // добавляем в массив кучу родителя, и так далее до конца, пока в массиве не останется 1 дерево 

  }

  if (heap->size == 1) { // если размер массива = 1, то берем это дерево которое осталось в массиве и дальше освобождаем кучу 
    Node* huffman_tree = heap->arr[0];
    free_heap(heap);
    return huffman_tree;
  }
  
  print_error("Failed to build huffman tree"); // если проверка выше не прошла, то выводим ошибку 
  return NULL;


}

void free_codes(char** codes) { // функция для освобождения памяти таблицы кодов 
  if (!codes) {
    // проверка на входные данные 
    return;
  }

  for (int i = 0; i < 256; i++) { // цикл для освобождения памяти таблицы 
    if (codes[i]) { // проверка не NULL ли codes[i]
      free(codes[i]);
    }
  }
  free(codes); // дальше освобождаем память самой переменной 
}

char* dfs_codes (Node* root, char* buff, char** code, int* idx) { // функция обхода в глубину дерева для построения таблицы кодов 
  if (!root) return NULL;

  if (root-> left == NULL && root->right == NULL) { // проверка если детей нет, в конец буфера ставим \0 и копируем весь буффер в таблицу кодов
    // *idx это путь от корня к листу. +1 для \0
    if (!(code[root->symbol] = malloc(*idx + 1))) { 
      // проверка на выделение памяти 
      print_error("Failed to allocate memory for symbol code");
      return NULL;
    }
    buff[*idx] = '\0'; // добавляем нуль-терминатор в конец для понимания, что это конец массива
    memcpy(code[root->symbol], buff, *idx + 1); // копируем в code из буффера
    return code[root->symbol];

  }
  buff[*idx] = '0'; // добавляем 0 если идет налево 
  (*idx)++; // увеличиваем индекс
  dfs_codes(root->left, buff, code, idx); // через рекурсию идем к левому ребенку 
  (*idx)--; // откатываем индекс после возврата из рекурсии  
  buff[*idx] = '1'; // аналогично но к правому ребенку 
  (*idx)++;
  dfs_codes(root->right, buff, code, idx);
  (*idx)--; // аналогично, откатываем индекс после возврата из рекурсии
  return NULL;

}

char** symbols_code(Node* root) {
  char** codes = calloc(256, sizeof(char*)); // выделяем память для таблицы кодов и обнуляем
  if (!codes) {
    // проверка на выделение памяти 
    print_error("Failed to allocate memory for codes table");
    return NULL;
  }
  char buffer[256] = {}; // создаем буффер 
  int index = 0;
  dfs_codes(root, buffer, codes, &index); // вызываем функция обхода в глубину 
  return codes;

}
