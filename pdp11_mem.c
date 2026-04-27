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
    {0777700, 0770000, "sob", do_sob},  
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
    pc = 01000;
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
                // Для MOV и ADD 
                if (command[i].do_command == do_mov || command[i].do_command == do_add) {
                    // Извлекаем 6-битные поля src и dst
                    // Каждое поле состоит из: [режим (3 бита) + регистр (3 бита)]
                    word src_field = (w >> 6) & 077;   // биты 6-11
                    word dst_field = w & 077;          // биты 0-5
                    
                    ss = get_mr(src_field);
                    dd = get_mr(dst_field);
                    trace(TRACE, "%s ", command[i].name);
                    command[i].do_command();
                } 
                else {
                    ss = get_mr(w);
                    trace(TRACE, "%s ", command[i].name);
                    command[i].do_command();
                }
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
    w_write(dd.adr, ss.val);
}

void do_add()
{
    w_write(dd.adr, ss.val + dd.val);
}

void do_sob()
{   
    int reg_num = (current_instruction >> 6) & 7;  // биты 6-8
    word offset = current_instruction & 077;       // младшие 6 бит
    
    reg[reg_num]--;
    
    if (reg[reg_num] != 0) {
        pc = pc - (offset * 2);
    }
}

void do_nothing() 
{

}

Arg get_mr(word w)
{
    Arg res;
    int r = w & 7;          // номер регистра
    int m = (w >> 3) & 7;   // номер моды
    word displacement;
    word temp_adr;

    switch (m) {
    case 0:  // Rn
        res.adr = r;
        res.val = reg[r];
        trace(TRACE, "R%d ", r);
        break;

    case 1:  // (Rn)
        res.adr = reg[r];
        res.val = w_read(res.adr);
        trace(TRACE, "(R%d) ", r);
        break;

    case 2:  // (Rn)+ или #константа
        res.adr = reg[r];
        res.val = w_read(res.adr);
        reg[r] += 2;
        if (r == 7)
            trace(TRACE, "#%o ", res.val);
        else
            trace(TRACE, "(R%d)+ ", r);
        break;

    case 3:  // @(Rn)+
        res.adr = w_read(reg[r]);
        res.val = w_read(res.adr);
        reg[r] += 2;
        if (r == 7)
            trace(TRACE, "@#%o ", res.adr);
        else
            trace(TRACE, "@(R%d)+ ", r);
        break;

    case 4:  // -(Rn)
        reg[r] -= 2;
        res.adr = reg[r];
        res.val = w_read(res.adr);
        trace(TRACE, "-(R%d) ", r);
        break;

    case 5:  // @-(Rn)
        reg[r] -= 2;
        res.adr = w_read(reg[r]);
        res.val = w_read(res.adr);
        trace(TRACE, "@-(R%d) ", r);
        break;

    case 6:  // d(Rn)
        displacement = w_read(pc);
        pc += 2;
        res.adr = reg[r] + displacement;
        res.val = w_read(res.adr);
        if (r == 7)
            trace(TRACE, "%o ", res.adr);
        else
            trace(TRACE, "%o(R%d) ", displacement, r);
        break;

    case 7:  // @d(Rn)
        displacement = w_read(pc);
        pc += 2;
        temp_adr = reg[r] + displacement;
        res.adr = w_read(temp_adr);
        res.val = w_read(res.adr);
        if (r == 7)
            trace(TRACE, "@%o ", temp_adr);
        else
            trace(TRACE, "@%o(R%d) ", displacement, r);
        break;

    default:
        trace(ERROR, "Mode %d not implemented yet!\n", m);
        exit(1);
    }
    return res;
}