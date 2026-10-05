#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

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

/* ---- ARTHUR: euclides.c (branch feat/arthur-marques) ---- */
long long euclides(long long a, long long b)
{
    if (a < 0) {
        a = -a;
    }
    if (b < 0) {
        b = -b;
    }

    while (b != 0) {
        long long resto = a % b;
        a = b;
        b = resto;
    }

    return a;
}

/* ---- ARTHUR: euclides_estendido.c (branch feat/arthur-marques) ---- */
long long *euclides_estendido(long long a, long long b)
{
    // O vetor permanece valido apos o retorno e e sobrescrito a cada chamada.
    static long long resultado[3];
    long long x_atual = 1;
    long long x_proximo = 0;
    long long y_atual = 0;
    long long y_proximo = 1;

    if (a < 0) {
        a = -a;
        x_atual = -1;
    }
    if (b < 0) {
        b = -b;
        y_proximo = -1;
    }

    while (b != 0) {
        long long quociente = a / b;
        long long resto = a % b;
        long long novo_x = x_atual - quociente * x_proximo;
        long long novo_y = y_atual - quociente * y_proximo;

        a = b;
        b = resto;
        // Atualiza os coeficientes da identidade de Bezout.
        x_atual = x_proximo;
        x_proximo = novo_x;
        y_atual = y_proximo;
        y_proximo = novo_y;
    }

    resultado[0] = a;       // MDC
    resultado[1] = x_atual; // Coeficiente x
    resultado[2] = y_atual; // Coeficiente y

    return resultado;
}

/* ---- MYLLENA: MDC.c (branch myllena_rodrigues) ---- */
int mdc(int a, int b)
{
    int resultado;

    resultado = euclides(a, b);

    return resultado;
}

int aritmetica_modular(int num, int x)
{
    int m;

    if (x <= 0)
    {
        return -1;
    }

    m = num % x;

    if (m < 0)
    {
        m = m + x;
    }

    return m;
}

int inverso_modular(int numero, int modulo){
    int result;
    if(mdc(numero,modulo) != 1){
        printf("O nÃºemero e o mÃ³dulo nÃ£o sÃ£o primos entre si\n");
        printf("Portanto, numero nÃ£o tem inverso modular\n");
        return -1;
    }
    else{
        result = euclides_estendido(numero,modulo)[1];
        if(result < 0){
            result += modulo;
        }
    }
    return result;
}

int phi_euler(int n){

    long long int* primo = (long long int*)malloc(sizeof(long long int)*2);
    int contPrimos = 0;
    primo[0] = 2;
    int primoInvalido;

    while(n > 1){ //Enquanto n > 1 nÃ£o encontramos todos os fatores
        primoInvalido = 1;
        while(primoInvalido){ //sÃ³ Ã© fator se for primo vÃ¡lido
            
            if(n % primo[contPrimos] == 0){//Se o primo encontrado dividir n, entÃ£o Ã© fator e sai do loop
                n = n / primo[contPrimos];
                primoInvalido = 0;
                contPrimos++;
                primo[contPrimos] = primo[contPrimos-1];
            }
            else{
                primo[contPrimos]++;
                while(eh_primo(primo[contPrimos]) == false){
                    primo[contPrimos]++;
                }
            }
        }
        primo = (long long int*)realloc(primo,sizeof(long long int)*(contPrimos+2));
    }
    primo[contPrimos] = 0;
    int expoente = 1;
    int phi = 1;
    long long int fator = primo[0];
    for(int i = 0; i < contPrimos; i++){
        if(primo[i] == primo [i+1]){
            expoente++;
            continue;
        }
        else{
            phi *= pow(primo[i],expoente) - pow(primo[i], (expoente - 1));
            expoente = 1;
        }
    }
    free(primo);
    return phi;
}

int teorema_chines_resto(
    int restos[],
    int modulos[],
    int quantidade)
{
    int M = 1;
    int M_linha;
    int inverso;
    int soma = 0;
    int resultado;

    for (int i = 0; i < quantidade; i++)
    {
        M = M * modulos[i];
    }

    for (int i = 0; i < quantidade; i++)
    {
        M_linha = M / modulos[i];

        inverso = inverso_modular(
            aritmetica_modular(M_linha, modulos[i]),
            modulos[i]);

        soma = soma + restos[i] * M_linha * inverso;
    }

    resultado = aritmetica_modular(soma, M);

    return resultado;
}

#endif /* BIBLIOTECA_H */
