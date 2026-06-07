# Huffman Archiver

![CI](https://github.com/taymanov-bn/Huffman-archiver/actions/workflows/ci.yml/badge.svg)
![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=flat&logo=cmake&logoColor=white)

Консольный архиватор файлов на основе алгоритма Хаффмана. Поддерживает сжатие и разжатие произвольных файлов.

## Сборка

```bash
mkdir build
cd build
cmake ..
make
```

## Использование

Сжатие:
```bash
./huffman -e input.txt compressed.bin
```

Разжатие:
```bash
./huffman -d compressed.bin output.txt
```

## Запуск тестов

```bash
cd build
ctest
```

## Структура проекта

- `src/` - исходный код
- `include/` - заголовочные файлы
- `tests/` - тесты
- `exp_files/` - файлы для экспериментов
- `experiments/` - результаты экспериментов и скрипт анализа
