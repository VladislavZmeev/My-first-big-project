#ifndef MEM_TESTS_H
#define MEM_TESTS_H
#include "types.h" 
#include <stdio.h>

void test1_mem(address adr, word w, byte expb1, byte expb0);
void test_mem();
void test_mem_with_loaded_data();

void b_write(address adr, byte b);
byte b_read(address adr);
void w_write(address adr, word w);
word w_read(address adr);

void load_data();
// Все числа шестнадцатеричные.
// Файл состоит из 1 или более блоков.
// Блок начинается со строки где через пробел записаны адрес начала блока и N сколько байт в этом блоке.
// далее идут N строк, по 1 байту на строку.
void mem_dump(address adr, int size);
// печатает size байт, начиная с адреса adr в виде слов по формату "%06o: %06o %04x":
// адрес: восьмеричное_слово шестнадцатеричное_слово

int set_log_level(int level);
void log_message(int level, const char* format, ...);

void load_data(FILE * file);
void load_file(const char * filename);


#endif