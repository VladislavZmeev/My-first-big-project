#include "types.h"
#include "prototypes.h"
#include <stdio.h>
#include <assert.h>

byte mem[MEM_SIZE]; // память для нашей виртуальной машины

void b_write(address adr, byte b)
{
    if (adr < REGSIZE) {
        reg[adr] = (b & 0200) ? (0177400 | b) : b;
    } else if (adr == ODATA) {
        putchar(b);
        fflush(stdout);
    } else if (adr != OSTAT) {
        mem[adr] = b;
    }
}

byte b_read(address adr)
{
    if (adr < REGSIZE) {
        return reg[adr] & 0377;
    }
    if (adr == OSTAT) {
        return READY;
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
    if (adr == OSTAT) {
        return READY;
    }

    word w = ((word)(mem[adr + 1])) << 8;
    w = w | (mem[adr] & 0xFF);
    return w;
}
