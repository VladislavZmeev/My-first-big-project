#ifndef TYPES_H
#define TYPES_H

#define MEM_SIZE (64*1024) // 64 KB
// Уровни логирования
typedef enum {
    ERROR = 0,
    INFO,
    TRACE,
    DEBUG,
} LogLevel;

typedef unsigned int uint;
typedef unsigned char byte;       // 8 bit
typedef unsigned short int word;  // 16 bit
typedef word address;             // 16 bit

#define REGSIZE 8
#define NO_ARGS 0
#define HAS_SS 1
#define HAS_DD (1 << 1)
#define HAS_NN (1 << 2 )

#define OSTAT 0177564
#define ODATA 0177566
#define READY 0200

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (* do_command)(void);
    short int operands; // has ss,, dd, nn, xx, r, etc
} Command;

extern uint N;
extern uint Z;
extern uint V;
extern uint C; 

extern word reg[8];
extern byte mem[];
#define pc reg[7]
#define sp reg[6]

typedef struct {
    word val;     // значение (что)
    address adr;    // адрес (куда)
} Arg;

#endif