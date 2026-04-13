#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "prototypes.h"

unsigned long long int coounter;



int main(int argc, char *argv[]) 
{
    test_mem_with_loaded_data();
    
    const char *filename = (argc > 1) ? argv[1] : "no file";
    
    printf("Используется файл: %s\n", filename);
    load_file(filename);
    
    return 0;
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

void mem_dump(address adr, int size)
{
    // Печатаем побайтово, но в выводе группируем по словам (2 байта)
    for(int i = 0; i < size; i += 2)
    {
        if(i + 1 < size)
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