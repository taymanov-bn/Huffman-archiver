// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan


#ifndef HEAP_H
#define HEAP_H

#include "tree.h"

// структура кучи
typedef struct {
    // массив узлов
    Node** arr;
    // размер кучи
    int size;
} Heap;

// создает новую пустую кучу
Heap* create_heap();

// добавляет узел в кучу
void add_heap(Heap* arr, Node* node);

// извлекает узел с минимальной частотой
Node* get_min_heap(Heap* arr);

// освобождает память кучи
void free_heap(Heap* arr);

#endif
