#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "error.h"

void test_encode_decode() {
  FILE* file_test = fopen("test_input.txt", "w");
  if (!file_test) {
    print_error("Failed to open test file");
    return;
  }
  fprintf(file_test, "hello world this is a huffman test");
  fclose(file_test);

  int enc_result = encode("test_input.txt", "test_compressed.txt");
  assert(enc_result == 0);

  int dec_result = decode("test_compressed.txt", "test_output.txt");
  assert(dec_result == 0);

  FILE* origin = fopen("test_input.txt", "rb");
  FILE* restored = fopen("test_output.txt", "rb");
  assert(origin && restored);

  int orig_byte;
  int restored_byte;
  int result = 1;
  while ((orig_byte = fgetc(origin)) != EOF) {
    restored_byte = fgetc(restored);
    result = result && (orig_byte == restored_byte);
  }
  result = result && (fgetc(restored) == EOF);

  fclose(origin);
  fclose(restored);
  remove("test_input.txt");
  remove("test_compressed.txt");
  remove("test_output.txt");
  assert(result);
  printf("test_encode_decode PASSED\n");
}

void test_encode_decode_empty() {
  FILE* file_test = fopen("test_empty.txt", "w");
  if (!file_test) {
    print_error("Failed to open test file");
    return;
  }
  fclose(file_test);

  int encode_result = encode("test_empty.txt", "test_empty_compressed.txt");
  assert(encode_result == 0);

  int decode_result = decode("test_empty_compressed.txt", "test_empty_output.txt");
  assert(decode_result == 0);

  remove("test_empty.txt");
  remove("test_empty_compressed.txt");
  remove("test_empty_output.txt");
  printf("test_encode_decode_empty PASSED\n");
}

void test_encode_decode_single_char() {
  FILE* file_test = fopen("test_single.txt", "w");
  if (!file_test) {
    print_error("Failed to open test file");
    return;
  }
  fprintf(file_test, "a");
  fclose(file_test);

  int encode_result = encode("test_single.txt", "test_single_compressed.txt");
  assert(encode_result == 0);

  int decode_result = decode("test_single_compressed.txt", "test_single_output.txt");
  assert(decode_result == 0);

  FILE* origin = fopen("test_single.txt", "rb");
  FILE* restored = fopen("test_single_output.txt", "rb");
  assert(origin && restored);

  int orig_byte = 1;
  int restored_byte = 1;
  int result = 1;

  while ((orig_byte = fgetc(origin)) != EOF) {
    restored_byte = fgetc(restored);
    result = result && (orig_byte == restored_byte);
  }
  result = result && (fgetc(restored) == EOF);

  fclose(origin);
  fclose(restored);
  remove("test_single.txt");
  remove("test_single_compressed.txt");
  remove("test_single_output.txt");
  assert(result);
  printf("test_encode_decode_single_char PASSED\n");
}

void test_encode_decode_repeated() {
  FILE* file_test = fopen("test_repeated.txt", "w");
  if (!file_test) {
    print_error("Failed to open test file");
    return;
  }

  for (int i = 0; i < 10000; i++) {
    fputc('a', file_test);
  }
  fclose(file_test);

  int encode_result = encode("test_repeated.txt", "test_repeated_compressed.txt");
  assert(encode_result == 0);

  int decode_result = decode("test_repeated_compressed.txt", "test_repeated_output.txt");
  assert(decode_result == 0);

  FILE* origin = fopen("test_repeated.txt", "rb");
  FILE* restored = fopen("test_repeated_output.txt", "rb");
  assert(origin && restored);

  int orig_byte = 1;
  int restored_byte = 1;
  int result = 1;

  while ((orig_byte = fgetc(origin)) != EOF) {
    restored_byte = fgetc(restored);
    result = result && (orig_byte == restored_byte);
  }
  result = result && (fgetc(restored) == EOF);

  fclose(origin);
  fclose(restored);
  remove("test_repeated.txt");
  remove("test_repeated_compressed.txt");
  remove("test_repeated_output.txt");
  assert(result);
  printf("test_encode_decode_repeated PASSED\n");
}

void test_encode_decode_all_bytes() {
  FILE* file_test = fopen("test_all_bytes.txt", "wb");
  if (!file_test) {
    print_error("Failed to open test file");
    return;
  }

  for (int i = 0; i < 256; i++) {
    fputc(i, file_test);
  }
  fclose(file_test);

  int encode_result = encode("test_all_bytes.txt", "test_all_bytes_compressed.txt");
  assert(encode_result == 0);

  int decode_result = decode("test_all_bytes_compressed.txt", "test_all_bytes_output.txt");
  assert(decode_result == 0);

  FILE* origin = fopen("test_all_bytes.txt", "rb");
  FILE* restored = fopen("test_all_bytes_output.txt", "rb");
  assert(origin && restored);

  int orig_byte = 1;
  int restored_byte = 1;
  int result = 1;

  while ((orig_byte = fgetc(origin)) != EOF) {
    restored_byte = fgetc(restored);
    result = result && (orig_byte == restored_byte);
  }
  result = result && (fgetc(restored) == EOF);

  fclose(origin);
  fclose(restored);
  remove("test_all_bytes.txt");
  remove("test_all_bytes_compressed.txt");
  remove("test_all_bytes_output.txt");
  assert(result);
  printf("test_encode_decode_all_bytes PASSED\n");
}

int main() {
  test_encode_decode();
  test_encode_decode_empty();
  test_encode_decode_single_char();
  test_encode_decode_repeated();
  test_encode_decode_all_bytes();
  return 0;
}
