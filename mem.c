#include "types.h"
#include "prototypes.h"
#include <stdio.h>
#include <assert.h>

byte mem[MEM_SIZE]; // память для нашей виртуальной машины

void b_write(address adr, byte b)
{
    if (adr < REGSIZE) { // если передаем в адрес от 0 до 8, то мы считаем, что записываем в регистр а не RAM
        reg[adr] = (b & 0200) ? (0177400 | b) : b; // делает знаковое расширение байта до слова
                                                   // 0177400 — это маска, у которой старшие 8 бит слова заполнены единицами, а младшие 8 бит остаются под значение b
    } else {
        mem[adr] = b;
    }
}

byte b_read(address adr)
{
    if (adr < REGSIZE) {
        return reg[adr] & 0377;
    }
    return mem[adr];
}
void w_write(address adr, word w)
{
    if (adr < REGSIZE) {
        reg[adr] = w;
    } else {
        assert((adr & 1) == 0);
        mem[adr] = w & 0xFF;
        mem[adr + 1] = (w >> 8) & 0xFF;
    }
}
word w_read(address adr)
{
    word w = ((word)(mem[adr + 1])) << 8;
    w = w | (mem[adr] & 0xFF);
    return w;
}
