#include<stdio.h>
#include<stdbool.h>
#include<math.h>
#include<stdlib.h>

void exp_modular(int base, int expoente, int mod)
{      
    
    int potencia2 = 16;

    int* binario = (int*)malloc(sizeof(int)*potencia2);
    int i = 0;
    while(i < potencia2)
    {
        binario[i] = expoente % 2;
        expoente = (int)expoente / 2;
        i++;
    }
    binario[i] = expoente / 2;
    int posicao=0; 

    for(i=0; i==0; )
    {   
        i = binario[posicao];
        posicao++;
    }
    int posicaoFinal = potencia2 - i;
    int vetorFinal[posicaoFinal];
    int j = 0;

    for( ; i<= potencia2; i++)
    {
        vetorFinal[j] = binario[i];
        j++;
    }

    for(i = 0; i< posicaoFinal; i++)
    {
        printf("%d\n", vetorFinal[i]);
    }

}


int main()
{
    exp_modular(11,33,34);
}