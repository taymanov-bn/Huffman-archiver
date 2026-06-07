#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tree/tree.h"
#include "heap/heap.h"
#include "huffman_tree/huffman_tree.h"
#include "error/error.h"

#define FREQUENCY_SIZE 256 // размер массива частот 

int encode(char* file_input, char* file_output) { // функция для кодирования файла 
  if (!file_input || !file_output) return -1; // проверка входных данных 

  FILE* inp_file = fopen(file_input, "rb"); // открываем входной файл для чтения в бинарном режиме 
  if (!inp_file) return -1; // проверка получилось ли открыть файл 

  FILE* out_file = fopen(file_output, "wb"); // аналогично, но для записи в бинарном режиме 
  if (!out_file) {
    fclose(inp_file); // закрываем входной файл 
    return -1;
  }

  int* freq = frequency_counter(file_input); // создаем массив частот из входного файла 
  if (!freq) {
    // проверка на создание массива частот
    // закрываем файлы 
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Heap* heap = heap_build_freq(freq); // создаем кучу с узлами из массива частот 
  if (!heap) {
    // проверка на создание кучи 
    free(freq);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  Node* tree = build_huffman_tree(heap); // создаем дерево Хаффмана 
  if (!tree) {
    // проверка на создание дерева Хаффмана 
    free_heap(heap);
    free(freq);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  char** codes = symbols_code(tree); // создаем таблицу кодов 
  if (!codes) {
    // проверка на создание таблицы кодов 
    // освобождать кучу уже не обязательно, ибо оно само освобождается при создании дерева Хаффмана 
    free(freq);
    free_tree(tree);
    fclose(inp_file);
    fclose(out_file);
    return -1;
  }

  if (!fwrite(freq, sizeof(int), FREQUENCY_SIZE, out_file)) { // записываем массив частот в выходной файл, чтобы при декодировании могли заново построить дерево Хаффмана 
    print_error("Failed to write frequency table");
    return -1;
  }
  
  uint8_t buffer_byte = 0; // создаем буффер байт чтобы записывали в него закодированные символы (байты)
  int bit_count = 0; // подсчет битов, чтобы знали когда нужно остановиться и записать байт в выходной файл 
  size_t symbol_code_size = 0; // переменная для подсчета размера закодированного символа 
  unsigned char symbol; // переменная куда будем читать из входного файла 

  while (fread(&symbol, sizeof(char), 1, inp_file) == 1) {
    
    char* code = codes[symbol]; // получаем код для текущего байта 
    symbol_code_size = strlen(code); // узнаем его размер 
    
    for (int i = 0; i < symbol_code_size; i++) { // цикл для записывания в буффер код 
      if (bit_count == 8) { // проверка, если буффер заполнен, то записываем его в выходной файл и обнуляем с счетчиком 
        if (!fwrite(&buffer_byte, sizeof(char), 1, out_file)) { // проверка и одновременно запись в выходной файл 
          // освобождение памяти и закрытие файлов 
          free(freq);
          free_tree(tree);
          free_codes(codes);
          fclose(inp_file);
          fclose(out_file);
          print_error("Failed to write compressed data");
          return -1;
        } 
        // обнуление буффера и счетчика 
        bit_count = 0;
        buffer_byte = 0;
      }
      buffer_byte = buffer_byte | ((code[i] - '0') << (7 - bit_count)); // записываем побитово в байт 
      // code[i] - '0' потому что code[i] это символ либо 0 либо 1, и если сделать - '0' то у нас останется либо число 0 либо 1
      // дальше делаем сдвиг налево 
      bit_count++; // увеличиваем счетчик 
    }
  }

  if (bit_count > 0) { // проверка если в конце буффер не заполнился, то записываем его таким какой он есть 
    if(!fwrite(&buffer_byte, sizeof(char), 1, out_file)) { // проверка и запись в выходной файл 
      free(freq);
      free_tree(tree);
      free_codes(codes);
      fclose(inp_file);
      fclose(out_file);
      print_error("Failed to write last byte");
      return -1;
    }
  }
 
  // ну и дальше освобождаем всю память и закрываем файлы 
  free(freq);
  free_tree(tree);
  free_codes(codes);
  fclose(inp_file);
  fclose(out_file);

  return 0;
}
