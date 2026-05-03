#include <stdlib.h>
#include <assert.h>
#include "types.h"
#include "prototypes.h"

unsigned long long int coounter;
word reg[8];
const Command command[] = {
    {0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},     // 2 операнда
    {0070000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},     // 2 операнда
    {0177700, 0005000, "clr", do_clr, HAS_DD},              // 1 операнд
    {0177000, 0077000, "sob", do_sob, NO_ARGS},             // 2 операнда
    {0177777, 0000000, "halt", do_halt, NO_ARGS},           // 0 операндов
    {0000000, 0000000, "unknown", do_nothing, 0}
};
static const size_t COMMANDS_SIZE = sizeof(command) / sizeof(command[0]);

int halt_flag = 0;  // глобальная переменная


int main(int argc, char *argv[]) 
{ 
    const char * filename = (argc > 1) ? argv[1] : "no file";
    trace(INFO, "\nИспользуется файл: %s\n", filename);
    load_file(filename);
    
    run();
    
    return 0;
}

Arg ss, dd;           // глобальные переменные
word current;         // глобальные переменные
unsigned int is_byte; // 0 если word, 1 если byte

void run()
{
    pc = 01000;
    set_log_level(TRACE);
    
    word w = 0;
    trace(INFO, "---------------- running --------------\n");
    while (!halt_flag) {
        w = w_read(pc);
        current = w;
        is_byte = (w & 0100000) != 0; // 0 если word, 1 если byte
        trace(TRACE, "%06o: ", pc);
        pc += 2;

        for (size_t i = 0; i < COMMANDS_SIZE; i++) 
        {
            if ((w & command[i].mask) == command[i].opcode)
            {
                trace(TRACE, "%s ", command[i].name);
                
                // Разбираем операнды в зависимости от количества
                if (command[i].operands & HAS_SS) 
                {
                    ss = get_mr(w >> 6);
                }

                if (command[i].operands & HAS_DD) 
                {
                    dd = get_mr(w);
                }
                
                // отладочная печать:
                command[i].do_command();
                if (command[i].opcode == 0010000) // mov
                { 
                    trace(TRACE, "             [%06o]=%06o ", ss.adr, ss.val);
                }

                if (command[i].opcode == 0060000) // add
                { 
                    int sreg = (current >> 6) & 7;
                    int dreg = current & 7;
                    trace(TRACE, "             R%d=%06o R%d=%06o ", sreg, reg[sreg], dreg, reg[dreg]);
                } 
                print_reg();               
                break;
            }
        }
        trace(TRACE, "\n");
    }
    trace(INFO, "\n---------------- halted ---------------\n");
    trace(INFO, "r0=%06o r2=%06o r4=%06o sp=%06o\n", reg[0], reg[2], reg[4], reg[6]);
    trace(INFO, "r1=%06o r3=%06o r5=%06o pc=%06o\n", reg[1], reg[3], reg[5], reg[7]);
}

void print_reg()
{
    trace(DEBUG, "\nr0:%o r1:%o r2:%o r3:%o r4:%o r5:%o r6:%o r7:%o\n",
        reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
}

void do_halt()
{
    halt_flag = 1;
}

void do_mov()
{
    if (is_byte) {
        b_write(dd.adr, ss.val);
    } else {
        w_write(dd.adr, ss.val);
    }
}

void do_add()
{
    w_write(dd.adr, ss.val + dd.val);
}

void do_sob()
{
    int r = (current >> 6) & 7;
    word nn = current & 077;
    address target = pc - 2 * nn;

    trace(TRACE, "R%d, %06o", r, target);

    reg[r]--;

    if (reg[r] != 0) {
        pc = target;
    }
}

void do_clr()
{
    w_write(dd.adr, 0);
}

void do_nothing() 
{

}

Arg get_mr(word w)
{
    Arg res;
    int r = w & 7;          // номер регистра
    int m = (w >> 3) & 7;   // номер моды
    int step = (is_byte && r != 6 && r != 7) ? 1 : 2; //для мод 2 и 4 нужен разный шаг

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

    case 2:
        res.adr = reg[r];
        res.val = is_byte ? b_read(res.adr) : w_read(res.adr);
        reg[r] += step;
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

    case 4:
        reg[r] -= step;
        res.adr = reg[r];
        res.val = is_byte ? b_read(res.adr) : w_read(res.adr);
        break;

    case 5:
        reg[r] -= 2;
        res.adr = w_read(reg[r]);
        res.val = w_read(res.adr);
        trace(TRACE, "@-(R%d) ", r);
        break;
     
    default:
        trace(ERROR, "Mode %d not implemented yet!\n", m);
        exit(1);
    }
    return res;
}