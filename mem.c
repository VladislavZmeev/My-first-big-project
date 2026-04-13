#include "types.h"
#include "prototypes.h"
#include <stdio.h>

byte mem[MEMESIZE]; // память для нашей виртуальной машины

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
