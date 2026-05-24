#include <string.h>
#include "decode/decode.h"
#include "encode/encode.h"

#define FLAG argv[1]
#define INPUT_FILE argv[2]
#define OUTPUT_FILE argv[3]

int main (int argc, char* argv[]) {
  if (argc != 4) {
    // too few argc
    return -1;
  }

  if (strcmp(FLAG, "-e") != 0 && strcmp(FLAG, "-d") != 0) {
    // invalid argument
    return -2;
  }

  if (strcmp(FLAG, "-e") == 0) {
    return encode(INPUT_FILE, OUTPUT_FILE);
  }
  if (strcmp(FLAG, "-d") == 0) {
    return decode(INPUT_FILE, OUTPUT_FILE);
  }

  return 0;
  
}
