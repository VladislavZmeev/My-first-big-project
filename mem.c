#include "types.h"
#include "prototypes.h"
#include <stdio.h>
#include <assert.h>

byte mem[MEM_SIZE]; // память для нашей виртуальной машины

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
    // пишем значение (слово) val по адресу adr
    assert((adr & 1) == 0);
    mem[adr] = w;
    // на адресах от 0 до 7 считаем, что адресуются регистры, а не RAM

    if (adr < REGSIZE) {
        // пишем в регистр
        reg[adr] = w;
    }
    else {
        // пишем в RAM
        assert((adr & 1) == 0);
        mem[adr] = w;
    }
}
word w_read(address adr)
{
    word w = ((word)(mem[adr + 1])) << 8;
    w = w | (mem[adr] & 0xFF);
    return w;
}
