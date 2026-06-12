// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#include <stdlib.h>
#include "tree/tree.h"
#include "heap/heap.h"
#include "error/error.h"
#include <stdio.h>

#define HEAP_CAPACITY 256 // размер кучи, 256 потому что 1 байт это 256 значений, а значит максимум 256 разных значений байтов 


Heap* create_heap() { // функция для создания кучи 
  Heap* heap;
  heap = malloc(sizeof(Heap)); // выделение памяти 
  if (!heap) {
    // проверка на выделение памяти 
    print_error("Failed to allocate memory for heap");
    return NULL;
  }
  heap->arr = malloc(sizeof(Node*)*HEAP_CAPACITY); // выделение памяти для массива с узлами внутри структуры кучи 
  if (!heap->arr) {
    // проверка на выделение памяти 
    free(heap);
    print_error("Failed to allocate memory for heap array");
    return NULL;
  }
  heap->size = 0; // сразу обнуляем чтобы не забилось мусором (каким то рандомным значением)
  return heap;
}

void add_heap(Heap* heap, Node* node) { // функция для добавления в кучу узла 
  if (heap->size >= HEAP_CAPACITY) { // проверка на размер кучи, если больше HEAP_CAPACITY или == ему, то выводим ошибку
    print_error("Heap is full");
    return;
  }
  heap->arr[heap->size] = node; // добавляем в массив под индексом текущего размера кучи узел полученный на вход функции 
  if (heap->size > 0) {  
    int index = heap->size;

    while (index > 0) {
      if (heap->arr[index]->frequency < heap->arr[(index - 1) / 2]->frequency) { // проверка на то, что дети больше родителя
        Node* temp = heap->arr[index]; // временная переменная чтобы сделать swap 
        heap->arr[index] = heap->arr[(index - 1) / 2]; // меняем местами текущий узел с родителем 
        index = (index - 1) / 2; // переходим на уровень (итндекс) родителя 
        heap->arr[index] = temp; // записываем на место родителя текущий узел 
      }
      else if (heap->arr[index]->frequency ==  heap->arr[(index - 1) / 2]->frequency) { // проверка если одинаковая частота
        if (heap->arr[index]->symbol < heap->arr[(index - 1) / 2]->symbol) { // если одинаковые частоты, то сравниваем символы
          // ну и дальше аналогично
          Node* temp = heap->arr[index];
          heap->arr[index] = heap->arr[(index - 1) / 2];
          index = (index - 1) / 2;
          heap->arr[index] = temp;
        } else break; // в остальных случаях выходим с цикла 
      } else break; // выходим с цикла 
    }  

  }
  heap->size++;

}


Node* get_min_heap(Heap* heap) { // функция для получения узла с минимальной частотой 

  if (!heap || heap->size == 0) return NULL;

  Node* min_node = heap->arr[0]; // берем самый первый узел в куче, а дальше будет сравнение 
  heap->arr[0] = heap->arr[heap->size-1]; // ставим в начало последний элемент 
  int index = 0;
  int child_index = 0;
  Node* min_both = NULL; // переменная для сравнения минимального среди детей 
  heap->size--; // уменьшаем на 1 размер 

  while (index < heap->size) { // цикл пока индекс меньше размера кучи 

    if ((index*2 + 1) >= heap->size && (index*2 + 2) >= heap->size) { // если у текущего узла нет детей, то выходим из цикла 
      break;
    }

    else if ((index*2 + 1) < heap->size && (index*2 + 2) >= heap->size) { // проверка если правый ребенок вне границы а левый внутри, то берем левый
      min_both = heap->arr[index*2 + 1];
      child_index = index*2 + 1; // присваиваем индекс минимального ребенка  
    } else {
      if (heap->arr[index*2 + 1]->frequency < heap->arr[index*2 + 2]->frequency) { // проверка уже по частотам, берем ребенка у которого частот меньше 
        min_both = heap->arr[index*2 + 1];
        child_index = index*2 + 1;
      } 
      else if (heap->arr[index*2 + 1]->frequency > heap->arr[index*2 + 2]->frequency) { // аналогично как и с прежним 
        min_both = heap->arr[index*2 + 2];
        child_index = index*2 + 2;
      } else {
        if (heap->arr[index*2 + 1]->symbol < heap->arr[index*2 + 2]->symbol) { // ну и если частоты одинаковые, то сравниваем символы 
          min_both = heap->arr[index*2 + 1];
          child_index = index*2 + 1;
        } else {
          min_both = heap->arr[index*2 + 2];
          child_index = index*2 + 2;

        }
      }
    }

    if (min_both == NULL) break; // если ничего не получили, то прерываем цикл 

    if (heap->arr[index]->frequency > min_both->frequency) { // если текущий узел больше минимального ребенка, меняем местами и идем дальше вниз по дереву
      Node* temp = heap->arr[index];
      heap->arr[index] = min_both;
      heap->arr[child_index] = temp;
      index = child_index;
    } else break;


  }
  return min_node;
}


void free_heap(Heap* heap) { // функция для освобождения памяти кучи 
  if (!heap) {
    return;
  }

  free(heap->arr); // сначало освобождаем массив с узлами а дальше уже саму кучу
  free(heap);
}
