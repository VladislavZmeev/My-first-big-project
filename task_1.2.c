#include <stdio.h>
#define N 1000

int main() 
{
    int registers[4] = {0, 0, 0, 0}; // мы создаем 4 регистра
    int program[N];                  // и массив из чисел, куда будем вводить исходные значения

    int k = 0;                       // переменные  
    int num = 0;                     // для работы цикла 
    while (scanf("%d", &num) != EOF) {
        program[k++] = num;
    }
    
    int pc = 0; // program counter
    while (pc < k) 
    {
        int sheron = program[pc++];
        switch(sheron) {

            case 0: // HTL остановка
                pc = k;
                break;

            case 1: // ASS - сложение 
                int a1 = program[pc++];
                int a2 = program[pc++];
                int ind1 = a1 - 5; //преабразуем числа
                int ind2 = a2 - 5; //в индексы массива
                registers[ind1] += registers[ind2];
                break;

            case 2: // SUB - вычетание 
                int b1 = program[pc++];
                int b2 = program[pc++];
                int ind1 = b1 - 5; //преабразуем числа
                int ind2 = b2 - 5; //в индексы массива
                registers[ind1] -= registers[ind2];
                break;

            case 3: // MOV	(reg, число)	Записать число в регистр reg
                int a = program[pc++];
                int n = program[pc++];
                int ind = a - 5;
                registers[ind] = n;
                break;
            case 4: //IR	нет	напечатать числа из всех регистров через пробел
                for(int i = 0; i < 4; i++) {
                    printf("%d ", registers[i]);
                }
                break;
                





        }
    }


    return 0;
}