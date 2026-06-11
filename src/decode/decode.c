// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "huffman_tree/huffman_tree.h"
#include "heap/heap.h"
#include "tree/tree.h"
#include "error/error.h"

#define FREQUENCY_SIZE 256 // размер массива частот

int decode (char* file_input, char* file_output) {
  if (!file_input || !file_output) { // проверка входных данных 
    print_error("Files are not provided");
    return -1;
  }

  FILE* inp_file = fopen(file_input, "rb"); // открываем входной файл для чтения в бинарном режиме 
  if (!inp_file) {
    // проверка на открытие файла 
    print_error("Invalid input file path");
    return -1;
  }

  FILE* out_file = fopen(file_output, "wb"); // открываем выходной файл для записи в бинарном режиме 
  if (!out_file) {
    // проверка на открытие файла 
    fclose(inp_file); // закрываем входной файл 
    print_error("Invalid output file path");
    return -1;
  }

  int* freq_arr = calloc(256, sizeof(int)); // создаем массив частот и выделяем для него память и сразу обнуляем  
  if (!freq_arr) {
    // проверка на выделение памяти для массива частот 
    print_error("Failed to allocate memory for frequency array during decoding");
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  if (!fread(freq_arr, sizeof(int), FREQUENCY_SIZE, inp_file)) { // проверка и читаем первые 256 * sizeof(int) байт в массив частот
    free(freq_arr);
    fclose(inp_file);
    fclose(out_file);
    print_error("Failed to read frequency table to freq array");
    return -1;
  }

  Heap* heap = heap_build_freq(freq_arr); // создаем кучу из массива частот 
  if (!heap) {
    // проверка на создание кучи 
    free(freq_arr);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Node* tree = build_huffman_tree(heap); // создаем дерево Хаффмана
  if (!tree) {
    // проверка на создание дерева Хаффмана 
    free(freq_arr);
    free_heap(heap);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  unsigned char byte = 0;
  uint8_t buffer_byte = 0;
  int freq_counter = 0; // подсчет общего количества символов чтобы в конце понять, когда остановиться

  for (int i = 0; i < 256; i++) {
    if (freq_arr[i] > 0) freq_counter += freq_arr[i]; // подсчет всех частот 
  }

  Node* current_node = tree; // переменная текущего узла в дереве  


  while (fread(&byte, sizeof(char), 1, inp_file) == 1) { // читаем байт и декодируем 
    if (freq_counter == 0) break; // если обшее колво частот = 0, выходим с цикла 

    for (int i = 0; i < 8; i++) { // побитово читаем 

      buffer_byte = (byte >> (7 - i)) & 1; // сдвигаем каждый бит к младшему и делаем битовую операцию and & чтобы избавиться от лишних битов 

      if (buffer_byte == 0) { // если бит равен 0
        if (current_node->left != NULL) { // переходим к левому ребенку 
          current_node = current_node->left;
          if (current_node->left == NULL && current_node->right == NULL) { // если у текущего узла нет детей, то записываем символ(байт) в выходной файл
            if (!fwrite(&current_node->symbol, sizeof(char), 1, out_file)) {
              free(freq_arr);
              free_tree(tree);
              fclose(inp_file);
              fclose(out_file);
              print_error("Failed to write decoded data");
              return -1;
            }
            freq_counter--; // уменьшаем колво частот символов 
            if (freq_counter == 0) break; // вот и проверка для которой нужна эта перемення, если она равна 0, ты прерываем цикл 
            current_node = tree; // перехожим к корню 
          }
        }
      }

      if (buffer_byte == 1) { // все аналогично но уже с 1 и идем к правому ребенку 
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
