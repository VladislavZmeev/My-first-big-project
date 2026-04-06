#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "prototypes.h"

#define MEMESIZE (64*1024) // 64 KB
byte mem[MEMESIZE]; // память для нашей виртуальной машины
unsigned long long int coounter;




int main() 
{
    test_mem();
    return 0;
}

void b_write(address adr, byte b)
{
    mem[adr] = b;
}
byte b_read(address adr)
{
    return mem[adr];
}

void w_write(address adr, word w)
{
    mem[adr] = w & 0xFF;
    mem[adr + 1] = (w >> 8) & 0xFF;
}
word w_read(address adr)
{
    word w = ((word)(mem[adr + 1])) << 8;
    w = w | (mem[adr] & 0xFF);
    return w;
}

void load_data()
{
    address adr;
    unsigned long int n;
    byte b;
    
    while(scanf("%hx %lx", &adr, &n) == 2)
    {
        for(unsigned long int i = 0; i < n; i++)
        {
            scanf("%hhx", &b);
            if(adr + i < MEMESIZE)  // проверка границ
                mem[adr + i] = b;
        }
    }
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