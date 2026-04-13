#ifndef TYPES_H
#define TYPES_H

#define MEMESIZE (64*1024) // 64 KB
// Уровни логирования
#define ERROR   0
#define INFO    1
#define TRACE   2
#define DEBUG   3

#define ADD_MASK 0170000
#define ADD_CODE 0060000
#define HALT_MASK 0177777
#define HALT_CODE 0000000
#define MOV_MASK 0170000
#define MOV_CODE 0010000

typedef unsigned char byte;       // 8 bit
typedef unsigned short int word;  // 16 bit
typedef word address;              // 16 bit

extern word reg[8];
extern byte mem[];
#define pc reg[7]

#endif