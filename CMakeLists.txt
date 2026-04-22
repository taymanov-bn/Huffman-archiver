cmake_minimum_required(VERSION 3.17)

project(huffman-archiver C)

set(CMAKE_C_STANDARD 99)

add_library(tree
    src/tree/tree.c
)

target_include_directories(tree PUBLIC ${CMAKE_SOURCE_DIR}/include)
