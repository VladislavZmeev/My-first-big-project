#include "types.h"
#include "prototypes.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void test1_mem(address adr, word w, byte expb1, byte expb0)  
{
    /*
    Набор тестов чтения-записи байтов и слов.
    1. пишем байт, читаем байт.
    ...
    Начиная с адреса adr, каждый тест на новом слове.
    */

    printf("==============================\n");
    // пишем байт, читаем байт
    b_write(adr, expb0);
    byte bres = b_read(adr);
    printf("1) bres = %02hhx\n   b0 = %02hhx\n", bres, expb0);
    assert(bres == expb0);
    adr += 2;

    printf("______________________________\n");

    // пишем 2 байта, читаем слово 
    byte b1 = expb1;
    byte b0 = expb0;
    b_write(adr, b0);
    b_write(adr + 1, b1);
    word wres = w_read(adr);
    printf("2) b0 = %02hhx b1 = %02hhx\n   w = %04x wres = %04x\n", b0, b1, w, wres);
    assert(wres == w);
    adr += 2;
    printf("______________________________\n");
    
    // пишем слово, читаем 2 байта 
    w_write(adr, w);
    b0 = b_read(adr);
    b1 = b_read(adr+1);
    printf("3) w = %04hx\n   b0 = %02hhx b1 = %02hhx\n", w, b0, b1);
    assert(b0 == expb0);
    assert(b1 == expb1);
    printf("______________________________\n");

    // пишем слово, итаем слово
    w_write(adr, w);
    wres = w_read(adr);
    printf("4) w =  %04hx\n   wres = %04hx\n", w, wres);
    assert(wres == w);
    adr += 2;
    printf("==============================\n\n\n");



}
void test_mem()
{
    test1_mem(0, 0x0a0b, 0x0a, 0x0b);
    test1_mem(010, 0xcadb, 0xca, 0xdb); 
    test1_mem(67, 0xffff, 0xff, 0xff);
}

void test_mem_with_loaded_data(address test_adr)
{
    printf("\n=== ТЕСТЫ НА ЗАГРУЖЕННЫХ ДАННЫХ ===\n\n");
    
    // Берём адреса и данные из загруженной памяти
    word test_word = w_read(test_adr);
    byte test_b1 = b_read(test_adr + 1);
    byte test_b0 = b_read(test_adr);
    
    printf("Используем данные из памяти по адресу 0x%04x:\n", test_adr);
    printf("  слово: 0x%04x\n", test_word);
    printf("  старший байт: 0x%02x\n", test_b1);
    printf("  младший байт: 0x%02x\n", test_b0);
    
    // Запускаем стандартный тест с этими данными
    test1_mem(test_adr + 4, test_word, test_b1, test_b0);
}

void load_file(const char * filename)
{
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Ошибка открытия файла '%s': ", filename);
        perror("");
        exit(1);
    }
    
    printf("Загрузка тестовых данных из: %s\n", filename);
    load_data(file);
    fclose(file);
    printf("Тестовые данные успешно загружены\n");
}

void mem_dump(address adr, int size_bytes)
{
    // Печатаем побайтово, но в выводе группируем по словам (2 байта)
    for(int i = 0; i < size_bytes; i += 2)
    {
        if(i + 1 < size_bytes)
        {
            word w = b_read(adr + i) | (b_read(adr + i + 1) << 8);
            printf("%06o: %06o %04x\n", adr + i, w, w);
        }
        else
        {
            // Если остался один байт (нечетное количество)
            word w = b_read(adr + i);
            printf("%06o: %06o %04x\n", adr + i, w, w);
        }
    }
}

void load_data(FILE * file)
{
    address adr;
    unsigned long int n;
    byte b;
    
    while(fscanf(file, "%hx %lx", &adr, &n) == 2)
    {
        for(unsigned long int i = 0; i < n; i++)
        {
            fscanf(file, "%hhx", &b);
            if(adr + i < MEMESIZE)  // проверка границ
                mem[adr + i] = b;
        }
    }
    
}