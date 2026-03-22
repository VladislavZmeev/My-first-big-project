#include <stdio.h>
#include <stdlib.h>

#define MEMESIZE (64*1024) // 64 KB
byte memory[MEMESIZE]; // память для нашей виртуальной машины

typedef unsigned char byte;       //8 bit
typedef unsigned short int word;  //16 bit
typedef word Adress;              //16 bit

void b_write(Adress adr, byte b);
byte b_read(Adress adr);
void w_write(Adress adr, word w);
word w_read(Adress adr);

int main() 
{
    
}
