#ifndef MATEMATICA_H
#define MATEMATICA_H

#include <stdbool.h>
#include <limits.h>
#include "auxiliares.h"

/* Funcoes com implementacao no cabecalho: incluir por um unico .c. */
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


int mdc(int a, int b)
{
    return (int) euclides(a, b);
}

int aritmetica_modular(int numero, int modulo)
{
    if (modulo <= 0) return -1;
    int resto = numero % modulo;
    if (resto < 0) resto += modulo;
    return resto;
}

int inverso_modular(int numero, int modulo)
{
    if (modulo < 2) return -1;
    numero = aritmetica_modular(numero, modulo);
    if (mdc(numero, modulo) != 1) return -1;
    return aritmetica_modular((int) euclides_estendido(numero, modulo)[1], modulo);
}

bool eh_primo(int numero)
{
    if (numero < 2) return false;
    for (int divisor = 2; divisor <= numero / divisor; divisor++) {
        if (numero % divisor == 0) return false;
    }
    return true;
}

int exp_modular(int base, int expoente, int modulo)
{
    if (modulo <= 0 || expoente < 0) return -1;
    long long resultado = 1 % modulo;
    long long fator = aritmetica_modular(base, modulo);
    while (expoente > 0) {
        if (expoente % 2 == 1) resultado = resultado * fator % modulo;
        fator = fator * fator % modulo;
        expoente /= 2;
    }
    return (int) resultado;
}

int phi_euler(int numero)
{
    if (numero < 1) return -1;
    int resultado = numero;
    for (int fator = 2; fator <= numero / fator; fator++) {
        if (numero % fator == 0) {
            resultado -= resultado / fator;
            while (numero % fator == 0) numero /= fator;
        }
    }
    if (numero > 1) resultado -= resultado / numero;
    return resultado;
}

/* Modulos maiores que 1 e coprimos entre si; -1 indica entrada invalida
 * ou produto que nao cabe em int. */
int teorema_chines_resto(int restos[], int modulos[], int quantidade)
{
    if (quantidade <= 0) return -1;
    int produto = 1;
    for (int i = 0; i < quantidade; i++) {
        if (modulos[i] < 2 || produto > INT_MAX / modulos[i]) return -1;
        for (int j = 0; j < i; j++) {
            if (mdc(modulos[i], modulos[j]) != 1) return -1;
        }
        produto *= modulos[i];
    }
    long long soma = 0;
    for (int i = 0; i < quantidade; i++) {
        int parte = produto / modulos[i];
        int inverso = inverso_modular(parte, modulos[i]);
        long long termo = (long long) aritmetica_modular(restos[i], modulos[i]) * parte % produto;
        termo = termo * inverso % produto;
        soma = (soma + termo) % produto;
    }
    return (int) soma;
}


/* Operacoes de matrizes em Z26 para Hill. */
int **criarMatriz(int ordem)
{
    if (ordem <= 0) return NULL;
    int **matriz = memoria_dinamica(NULL, (size_t) ordem, sizeof(int *));

    for (int i = 0; i < ordem; i++)
    {
        matriz[i] = memoria_dinamica(NULL, (size_t) ordem, sizeof(int));
    }

    return matriz;
}

void liberarMatriz(int **matriz, int ordem)
{
    for (int i = 0; i < ordem; i++)
    {
        free(matriz[i]);
    }

    free(matriz);
}

void obterMenor(int **matriz, int **menor, int ordem, int linhaRemover, int colunaRemover)
{
    int linhaMenor = 0;
    int colunaMenor;

    for (int i = 0; i < ordem; i++)
    {
        if (i == linhaRemover)
        {
            continue;
        }

        colunaMenor = 0;

        for (int j = 0; j < ordem; j++)
        {
            if (j == colunaRemover)
            {
                continue;
            }

            menor[linhaMenor][colunaMenor] = matriz[i][j];
            colunaMenor++;
        }

        linhaMenor++;
    }
}

/* Determinante reduzido modulo 26 para evitar crescimento dos produtos.
 * Elementos da matriz devem estar entre 0 e 25. */
int determinanteHill(int **matriz, int ordem)
{
    if (ordem == 1)
    {
        return aritmetica_modular(matriz[0][0], 26);
    }

    if (ordem == 2)
    {
        return aritmetica_modular(matriz[0][0] * matriz[1][1] - matriz[0][1] * matriz[1][0], 26);
    }

    int det = 0;
    int sinal = 1;

    for (int coluna = 0; coluna < ordem; coluna++)
    {
        if (matriz[0][coluna] != 0)
        {
            int **menor = criarMatriz(ordem - 1);
            obterMenor(matriz, menor, ordem, 0, coluna);
            det = aritmetica_modular(det + sinal * matriz[0][coluna] * determinanteHill(menor, ordem - 1), 26);
            liberarMatriz(menor, ordem - 1);
        }

        sinal = -sinal;
    }

    return det;
}

void calcularCofatores(int **matriz, int **cofatores, int ordem, int modulo)
{
    if (ordem == 1)
    {
        cofatores[0][0] = 1;
        return;
    }

    for (int i = 0; i < ordem; i++)
    {
        for (int j = 0; j < ordem; j++)
        {
            int **menor = criarMatriz(ordem - 1);
            obterMenor(matriz, menor, ordem, i, j);

            int detMenor = determinanteHill(menor, ordem - 1);
            int sinal;

            if ((i + j) % 2 == 0)
            {
                sinal = 1;
            }
            else
            {
                sinal = -1;
            }

            cofatores[i][j] = aritmetica_modular(sinal * detMenor, modulo);
            liberarMatriz(menor, ordem - 1);
        }
    }
}

void calcularTransposta(int **matriz, int **transposta, int ordem)
{
    for (int i = 0; i < ordem; i++)
    {
        for (int j = 0; j < ordem; j++)
        {
            transposta[j][i] = matriz[i][j];
        }
    }
}

int calcularInversaHill(int **matriz, int **inversa, int ordem, int modulo)
{
    if (ordem <= 0 || modulo != 26) return 0;
    int det = determinanteHill(matriz, ordem);
    det = aritmetica_modular(det, modulo);

    if (mdc(det, modulo) != 1)
    {
        return 0;
    }

    int inversoDet = inverso_modular(det, modulo);
    inversoDet = aritmetica_modular(inversoDet, modulo);

    int **cofatores = criarMatriz(ordem);

    calcularCofatores(matriz, cofatores, ordem, modulo);

    for (int i = 0; i < ordem; i++)
    {
        for (int j = 0; j < ordem; j++)
        {
            inversa[i][j] = aritmetica_modular(inversoDet * cofatores[j][i], modulo);
        }
    }

    liberarMatriz(cofatores, ordem);

    return 1;
}

void multiplicarVetorMatriz(int *vetor, int **matriz, int *resultado, int ordem, int modulo)
{
    for (int j = 0; j < ordem; j++)
    {
        resultado[j] = 0;

        for (int i = 0; i < ordem; i++)
        {
            resultado[j] = aritmetica_modular(resultado[j] + vetor[i] * matriz[i][j], modulo);
        }

        resultado[j] = aritmetica_modular(resultado[j], modulo);
    }
}


int eh_coprimo_26(int a)
{
    a = (a % 26 + 26) % 26;
    return mdc(a, 26) == 1;
}


#endif
