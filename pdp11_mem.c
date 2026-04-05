#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned char byte;       //8 bit
typedef unsigned short int word;  //16 bit
typedef word address;              //16 bit

#define MEMESIZE (64*1024) // 64 KB
byte mem[MEMESIZE]; // память для нашей виртуальной машины
unsigned long long int coounter;

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
    fprintf(stderr, "2) b0 = %02hhx b1 = %02hhx\n   w = %04x wres = %04x\n", b0, b1, w, wres);
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