#ifndef TYPES_H
#define TYPES_H

#define MEMESIZE (64*1024) // 64 KB
// Уровни логирования
#define ERROR   0
#define INFO    1
#define TRACE   2
#define DEBUG   3

typedef unsigned char byte;       // 8 bit
typedef unsigned short int word;  // 16 bit
typedef word address;              // 16 bit

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*do_command)(void);
} Command;

extern word reg[8];
extern byte mem[];
#define pc reg[7]

typedef struct {
    word val;     // значение (что)
    address adr;    // адрес (куда)
} Arg;

#endif