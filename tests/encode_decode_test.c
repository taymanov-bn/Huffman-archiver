#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "encode/encode.h"
#include "decode/decode.h"
#include "error/error.h"

void test_encode_decode() {
    // создаём тестовый файл
    FILE* file_test = fopen("test_input.txt", "w");
    if (!file_test) {
        print_error("Failed to open test file");
        return;
    }
    fprintf(file_test, "hello world this is a huffman test");
    fclose(file_test);
    
    // сжимаем
    int enc_result = encode("test_input.txt", "test_compressed.txt");
    assert(enc_result == 0);
    
    // разжимаем
    int dec_result = decode("test_compressed.txt", "test_output.txt");
    assert(dec_result == 0);
    
    // сравниваем побайтово
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

int main() {
    test_encode_decode();
    return 0;
}
