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
    
    run();
    
    return 0;
}

Arg ss, dd;
void run()
{
    pc = 01000;  // начальный адрес
    set_log_level(DEBUG);
    
    word w;
    int i = 0;
    putchar('\n');
    while (!halt_flag) {
        w = w_read(pc);
        trace(TRACE, "%06o %06o: ", pc, w);
        pc += 2;
        
        for (i = 0; command[i].name != NULL; i++) 
        {
            if ((w & command[i].mask) == command[i].opcode) 
            {
                ss = get_mr(w);
                dd = get_mr(w);
                trace(TRACE, "%s ", command[i].name);
                command[i].do_command();
                break;
            }
        }
        putchar('\n');
        print_reg();
    }
}
void print_reg()
{
    trace(TRACE, "r0:%o r1:%o r2:%o r3:%o r4:%o r5:%o r6:%o r7:%o\n",
        reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
}

void do_halt()
{
    trace(INFO, "\nTHE END!!!\n");
    halt_flag = 1;
    exit(0);
}
void do_mov()
{
    // значение аргумента ss пишем по адресу аргумента dd
    w_write(dd.adr, ss.val);
}
void do_add()
{
    // сумму значений аргументов ss и dd пишем по адресу аргумента dd
    w_write(dd.adr, ss.val + dd.val);
}
void do_nothing() 
{

}

Arg get_mr(word w)
{
    Arg res;
    int r = w & 7;          // номер регистра
    int m = (w >> 3) & 7;   // номер моды

    switch (m) {
    // мода 0, R1
    case 0:
        res.adr = r;        // адрес - номер регистра
        res.val = reg[r];   // значение - число в регистре
        trace(TRACE, "R%d ", r);
        break;


    // мода 1, (R1)
    case 1:
        res.adr = reg[r];           // в регистре адрес
        res.val = w_read(res.adr);  // по адресу - значение
        trace(TRACE, "(R%d) ", r);
        break;


    // мода 2, (R1)+ или #3
    case 2:
        res.adr = reg[r];           // в регистре адрес
        res.val = w_read(res.adr);  // по адресу - значение
        reg[r] += 2;                // TODO: +1
        // печать разной мнемоники для PC и других регистров
        if (r == 7)
            trace(TRACE, "#%o ", res.val);
        else
            trace(TRACE, "(R%d)+ ", r);
        break;


    // мы еще не дописали другие моды
    default:
        trace(ERROR, "Mode %d not implemented yet!\n", m);
        exit(1);
}
    return res;
}