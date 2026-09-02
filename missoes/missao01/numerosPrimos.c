#include<stdio.h>
#include<stdbool.h>

bool eh_primo(int entrada)
{
    if(entrada == 0 || entrada == 1)
    {
        return false;
    }

    if(entrada == 2)
    {
        return true;
    }
    
    if(entrada % 2 == 0)
    {
        return false;
    }
    
    for(int contador = 2; contador < entrada; contador++)
    {
        if(entrada % contador == 0)
        {
            return false;
        }

    }
    return true;
}

int main()
{
    int x;
    printf("Digite um numero: ");
    scanf("%d", &x);

    if(eh_primo(x))
    {
        printf("\nE primo");
    }else{
        printf("Nao e primo\n");
    }
}