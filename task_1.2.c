#include <stdio.h>
#define N 1000

void ADD(int * a, int * b)
{
    *a += *b;
    return;
}
void SUB(int * a, int * b)
{
    *a -= *b;
    return;
}
void MOV(int * a, int b)
{
    *a = b;
    return;
}

int main() 
{
    int registers[4] = {0, 0, 0, 0};
    int program[N];
    int num = 0;
    for (int i = 0; scanf("%d", &num); i++) {
        program[i] = num;
    }

    return 0;
}