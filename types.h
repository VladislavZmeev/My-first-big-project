#ifndef TYPES_H
#define TYPES_H

#define MEMESIZE (64*1024) // 64 KB


typedef unsigned char byte;       // 8 bit
typedef unsigned short int word;  // 16 bit
typedef word address;              // 16 bit

extern word reg[8];
extern byte mem[];
#endif