cmake_minimum_required(VERSION 3.17)

project(huffman-archiver C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -g")

add_library(tree
    src/tree/tree.c
)

target_include_directories(tree PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(heap
    src/heap/heap.c
)

target_include_directories(heap PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(huffman_tree
    src/huffman_tree/huffman_tree.c
)

target_include_directories(huffman_tree PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(encode
    src/encode/encode.c
)

target_include_directories(encode PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(decode
    src/decode/decode.c
)

target_include_directories(decode PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(error
    src/error/error.c
)

target_include_directories(error PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(huffman src/main.c)
target_link_libraries(huffman tree heap huffman_tree encode decode error)
target_link_libraries(encode huffman_tree tree heap error)
target_link_libraries(decode huffman_tree tree heap error)
target_link_libraries(huffman_tree tree heap error)

add_executable(tree_test tests/tree_test.c)
target_link_libraries(tree_test tree error)

add_executable(heap_test tests/heap_test.c)
target_link_libraries(heap_test tree heap error)

add_executable(huffman_test tests/huffman_test.c)
target_link_libraries(huffman_test tree heap huffman_tree)



