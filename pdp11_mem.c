#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "prototypes.h"


unsigned long long int coounter;
word reg[8];


int main(int argc, char *argv[]) 
{
    const char *filename = (argc > 1) ? argv[1] : "no file";
    
    printf("Используется файл: %s\n", filename);
    load_file(filename);
        // printf("----------------\n");
        // mem_dump(0x40, 10);
        // printf("----------------\n");
        // mem_dump(0x200, 18);
    
    mem_dump(01000, 12);
    run();
    
    return 0;
}



void run()
{
    // следующее слово будем читать по адресу 1000 (восьмеричное)
    pc = 01000;
    set_log_level(DEBUG);

    word w;     // текущее слово, которое содержит команду
    // главный цикл выполнения программы
    while(1) {
        // читаем текущее слово
        w = w_read(pc);
        // печатаем адрес и слово по этому адресу, как в листинге
        trace(TRACE, "%06o %06o: ", pc, w);
        // pc сразу же указывает на следующее неразобранное слово
        pc += 2;

        if ((w & HALT_MASK) == HALT_CODE) {
            trace(TRACE, "halt");
            do_halt();
            break;
        }
        else if ((w & MOV_MASK) == MOV_CODE) {
            trace(TRACE, "mov");
            // Здесь будет обработка MOV
        }
        else if ((w & ADD_MASK) == ADD_CODE) {
            trace(TRACE, "add");
            // Здесь будет обработка ADD
        }
        else {
            trace(TRACE, "unknown");
        }


    }
    do_halt();
}

void do_halt()
{
    trace(INFO, "THE END!!!\n");
    exit(0);
}
void do_add() 
{

}
void do_mov() 
{

}
void do_nothing() 
{

}