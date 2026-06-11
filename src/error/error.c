// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Taymanov Balkan

#include <stdio.h>

void print_error(char* msg) { // функция для вывода ошибок в поток stderr
  fprintf(stderr, "Error: %s\n", msg);
}
