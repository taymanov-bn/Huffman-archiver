# Huffman Archiver

![CI](https://github.com/taymanov-bn/Huffman-archiver/actions/workflows/ci.yml/badge.svg)
![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=flat&logo=cmake&logoColor=white)

Консольный архиватор файлов на основе алгоритма Хаффмана. Поддерживает сжатие и разжатие произвольных файлов.

## Об алгоритме 

Алгоритм Хаффмана - способ сжатия файлов без потери данных. Идея простая - символы которые встречаются чаще кодируются короткими битовыми последовательностями, а редкие - длинными.
Главное правило, чтобы код был префиксным, то есть ни один код не является началом другого. Например если символ 'a' имеет код 001, то никакой другой не может начинаться с такого же.
Лучше всего алгоритм работает на больших текстовых файлах. На уже сжатых файлах (zip, jpeg) особо ничего не меняет.

## Зависимости

- gcc
- CMake
- python + pandas + matplotlib (для экспериментов)

## Установка

```bash
git clone https://github.com/taymanov-bn/Huffman-archiver.git
cd Huffman-archiver
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
