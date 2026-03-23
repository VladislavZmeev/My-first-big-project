#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned char byte;       //8 bit
typedef unsigned short int word;  //16 bit
typedef word Adress;              //16 bit

#define MEMESIZE (64*1024) // 64 KB
byte mem[MEMESIZE]; // память для нашей виртуальной машины

void b_write(Adress adr, byte b);
byte b_read(Adress adr);
void w_write(Adress adr, word w);
word w_read(Adress adr);

void test_mem()  
{
    printf("==============================\n");
    byte b0 = 0x0a;
    // пишем байт, читаем байт
    b_write(2, b0);
    byte bres = b_read(2);
    printf("bres = %02hhx, b0 = %02hhx\n", bres, b0);
    assert(bres == b0);
    printf("______________________________\n");

    // пишем 2 байта, читаем слово 
    Adress a = 4;
    byte b1 = 0x0b;
    b0 = 0x0a;
    word w = 0x0b0a;
    b_write(a, b0);
    b_write(a + 1, b1);
    word wres = w_read(a);
    printf("wres = %04hx\nb0 = %02hhx b1 = %02hhx\n", wres, b0, b1);
    assert(wres == w);
    printf("______________________________\n");
    
    // пишем слово, читаем 2 байта 
    a = 8;
    w = 0x0b0a;
    w_write(a, w);
    printf("w = %04hx\nb0 = %02hhx b1 = %02hhx\n", w, mem[a], mem[a+1]);
    printf("==============================\n");
}
int main() 
{
    test_mem();
    return 0;
}

void b_write(Adress adr, byte b)
{
    mem[adr] = b;
}
byte b_read(Adress adr)
{
    return mem[adr];
}

void w_write(Adress adr, word w)
{
    mem[adr] = w & 0xFF;
    mem[adr + 1] = (w >> 8) & 0xFF;
}
word w_read(Adress adr)
{
    word w = ((word)(mem[adr + 1])) << 8;
    w = w | (mem[adr] & 0xFF);
    return w;
}