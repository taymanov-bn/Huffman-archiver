#include <stdio.h>

void print_error(char* msg) {
  fprintf(stderr, "Error: %s\n", msg);
}
