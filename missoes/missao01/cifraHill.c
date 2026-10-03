#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../biblioteca.h"

int **criarMatriz(int ordem)
{
    int **matriz = malloc(ordem * sizeof(int *));

    for (int i = 0; i < ordem; i++)
    {
        matriz[i] = malloc(ordem * sizeof(int));
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

int determinanteHill(int **matriz, int ordem)
{
    if (ordem == 1)
    {
        return matriz[0][0];
    }

    if (ordem == 2)
    {
        return matriz[0][0] * matriz[1][1] - matriz[0][1] * matriz[1][0];
    }

    int det = 0;
    int sinal = 1;

    for (int coluna = 0; coluna < ordem; coluna++)
    {
        if (matriz[0][coluna] != 0)
        {
            int **menor = criarMatriz(ordem - 1);
            obterMenor(matriz, menor, ordem, 0, coluna);
            det += sinal * matriz[0][coluna] * determinanteHill(menor, ordem - 1);
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
    int det = determinanteHill(matriz, ordem);
    det = aritmetica_modular(det, modulo);

    if (mdc(det, modulo) != 1)
    {
        return 0;
    }

    int inversoDet = inverso_modular(det, modulo);
    inversoDet = aritmetica_modular(inversoDet, modulo);

    int **cofatores = criarMatriz(ordem);
    int **adjunta = criarMatriz(ordem);

    calcularCofatores(matriz, cofatores, ordem, modulo);
    calcularTransposta(cofatores, adjunta, ordem);

    for (int i = 0; i < ordem; i++)
    {
        for (int j = 0; j < ordem; j++)
        {
            inversa[i][j] = aritmetica_modular(inversoDet * adjunta[i][j], modulo);
        }
    }

    liberarMatriz(cofatores, ordem);
    liberarMatriz(adjunta, ordem);

    return 1;
}

void multiplicarVetorMatriz(int *vetor, int **matriz, int *resultado, int ordem, int modulo)
{
    for (int j = 0; j < ordem; j++)
    {
        resultado[j] = 0;

        for (int i = 0; i < ordem; i++)
        {
            resultado[j] += vetor[i] * matriz[i][j];
        }

        resultado[j] = aritmetica_modular(resultado[j], modulo);
    }
}

void prepararTexto(char *entrada, char *saida, int ordem)
{
    int posicao = 0;

    for (int i = 0; entrada[i] != '\0'; i++)
    {
        if (isalpha((unsigned char)entrada[i]))
        {
            saida[posicao] = toupper((unsigned char)entrada[i]);
            posicao++;
        }
    }

    while (posicao % ordem != 0)
    {
        saida[posicao] = 'X';
        posicao++;
    }

    saida[posicao] = '\0';
}

void cifraHill(char *texto, int **chave, int ordem, char *resultado)
{
    char textoPreparado[1000];
    prepararTexto(texto, textoPreparado, ordem);

    int vetor[ordem];
    int vetorResultado[ordem];
    int posicaoResultado = 0;

    for (int inicio = 0; textoPreparado[inicio] != '\0'; inicio += ordem)
    {
        for (int i = 0; i < ordem; i++)
        {
            vetor[i] = textoPreparado[inicio + i] - 'A';
        }

        multiplicarVetorMatriz(vetor, chave, vetorResultado, ordem, 26);

        for (int i = 0; i < ordem; i++)
        {
            resultado[posicaoResultado] = vetorResultado[i] + 'A';
            posicaoResultado++;
        }
    }

    resultado[posicaoResultado] = '\0';
}

int decifraHill(char *textoCifrado, int **chave, int ordem, char *resultado)
{
    int **inversa = criarMatriz(ordem);

    if (!calcularInversaHill(chave, inversa, ordem, 26))
    {
        liberarMatriz(inversa, ordem);
        return 0;
    }

    int vetor[ordem];
    int vetorResultado[ordem];
    int posicaoResultado = 0;

    for (int inicio = 0; textoCifrado[inicio] != '\0'; inicio += ordem)
    {
        for (int i = 0; i < ordem; i++)
        {
            vetor[i] = toupper((unsigned char)textoCifrado[inicio + i]) - 'A';
        }

        multiplicarVetorMatriz(vetor, inversa, vetorResultado, ordem, 26);

        for (int i = 0; i < ordem; i++)
        {
            resultado[posicaoResultado] = vetorResultado[i] + 'A';
            posicaoResultado++;
        }
    }

    resultado[posicaoResultado] = '\0';
    liberarMatriz(inversa, ordem);

    return 1;
}

int main()
{
    int ordem;

    printf("Digite a ordem da matriz chave: ");
    scanf("%d", &ordem);

    int **chave = criarMatriz(ordem);

    printf("\nDigite os elementos da matriz chave:\n");

    for (int i = 0; i < ordem; i++)
    {
        for (int j = 0; j < ordem; j++)
        {
            scanf("%d", &chave[i][j]);
        }
    }

    char texto[1000];
    char cifrado[1000];
    char decifrado[1000];

    printf("\nDigite a mensagem: ");
    scanf(" %999[^\n]", texto);

    cifraHill(texto, chave, ordem, cifrado);

    printf("\nMensagem original: %s\n", texto);
    printf("Mensagem cifrada: %s\n", cifrado);

    if (decifraHill(cifrado, chave, ordem, decifrado))
    {
        printf("Mensagem decifrada: %s\n", decifrado);
    }
    else
    {
        printf("\nErro: a matriz chave nao possui inversa em Z26.\n");
    }

    liberarMatriz(chave, ordem);

    return 0;
}