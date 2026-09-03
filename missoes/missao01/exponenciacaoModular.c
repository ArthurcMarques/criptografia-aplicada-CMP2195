#include<stdio.h>
#include<stdbool.h>
#include<math.h>
#include<stdlib.h>

int exp_modular(int base, int expoente, int mod)
{      
    int flagParada = 0;
    int contador = 0;
    
    int substituiExp = expoente;

    while(flagParada != 1)
    {
        substituiExp = substituiExp / 2;
        contador ++;
        if(substituiExp == 1)
        {
            contador ++;
            flagParada = 1;
        }
        if(substituiExp == 0)
        {
            flagParada = 1;
        }
    }

    int* binario = (int*)malloc(sizeof(int)*contador);
    
    int i = 0;
    while(i < contador)
    {
        binario[i] = expoente % 2;
        expoente = (int)expoente / 2;
        i++;
    }

    int d = 1;

    for(i = contador - 1; i >= 0; i--)
    {
        d = (d * d) % mod;

        if(binario[i] == 1)
        {
            d = (d * base) % mod;
        }
    }

    return d;

}

int main()
{
    int x = exp_modular(11,33,34);
    printf("%d", x);
}