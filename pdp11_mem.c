#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "prototypes.h"


unsigned long long int coounter;
word reg[8];
Command command[] = {
    {0170000, 0060000, "add", do_add},
    {0170000, 0010000, "mov", do_mov},
    {0177777, 0000000, "halt", do_halt},
    // ...
    {0000000, 0000000, "unknown", do_nothing}

};

int halt_flag = 0;  // глобальная переменная


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
    pc = 01000;  // начальный адрес
    set_log_level(DEBUG);
    
    word w;
    int i = 0;

    while (!halt_flag) {
        w = w_read(pc);
        trace(TRACE, "%06o %06o: ", pc, w);
        pc += 2;
        
        for (i = 0; command[i].name != NULL; i++) 
        {
            if ((w & command[i].mask) == command[i].opcode) 
            {
                trace(TRACE, "%s ", command[i].name);
                command[i].do_command();
                break;
            }
        }
        putchar('\n');

        
    }
}

void do_halt()
{
    trace(INFO, "THE END!!!\n");
    halt_flag = 1;
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