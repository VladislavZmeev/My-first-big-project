#include "types.h"
#include "prototypes.h"
#include <stdio.h>
#include <assert.h>

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