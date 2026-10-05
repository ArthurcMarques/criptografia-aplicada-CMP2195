#ifndef SEGUNDO_TRABALHO_CRIPTO_H
#define SEGUNDO_TRABALHO_CRIPTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* ---- HENRIQUE: Afim.h ---- */

int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int eh_coprimo_26(int a) {
    return mdc(a, 26) == 1;
}

int inverso_modular(int a, int m) {
    a = (a % m + m) % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

void para_maiusculo(char str[500]) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}


int encript(char vetorEncript[500], int cofatorA, int cofatorB){
    if(eh_coprimo_26(cofatorA) == 0){
        printf("O cofator A não é coprimo com 26, portanto não é possível realizar a encriptação.\n");
        return 0;
    }

    int tamanhoEntrada = strlen(vetorEncript);

    for(int i = 0; i < tamanhoEntrada; i++){
        vetorEncript[i] = (((cofatorA * (vetorEncript[i] - 'A') +  cofatorB)) % 26) + 'A';
    }
    return 1;
}

int decript(char vetorDecript[500], int cofatorA, int cofatorB){
    int tamanhoEntrada = strlen(vetorDecript);

    int inverso = inverso_modular(cofatorA, 26);
    for(int i = 0; i < tamanhoEntrada; i++){
        vetorDecript[i] = ((((vetorDecript[i] - 'A') - cofatorB) * inverso) % 26);
        vetorDecript[i] = (vetorDecript[i] + 26) % 26 + 'A';
    }
    return 1;
}

char* afim(char vetorEntrada[500], int cofatorA, int cofatorB){

    para_maiusculo(vetorEntrada);

    encript(vetorEntrada, cofatorA, cofatorB);
    printf("\nVetor Encriptado: ");
    for(int i = 0; i < strlen(vetorEntrada); i++){
        printf("%c, ", vetorEntrada[i]);
    }

    decript(vetorEntrada, cofatorA, cofatorB);
    printf("\nVetor Decriptado: ");
    for(int i = 0; i < strlen(vetorEntrada); i++){
        printf("%c, ",vetorEntrada[i]);
    }

    return vetorEntrada;
}


/* ---- HENRIQUE: Substituicao.h ---- */
int encriptAte52Posicoes(char vetorEntrada[500], char vetorValor[500], char vetorChave[53], int tamanhoEntrada, char vetorSaida[52]){
    int j, i;

    for(i = 0; i < 52; i++){
        for(j = 0; j < 52 && vetorEntrada[i] != vetorValor[j]; j++){}
        if(j == 52 && vetorValor[i] == '\0'){
            printf("Algum caractere inserido não faz parte dos caracteres aceitos como valor de entrada.\n");
            return 0;
        }
        else{
            vetorSaida[i] = vetorChave[j];
        } 
    }
    return 1;
}

int decriptAte52Posicoes(char vetorEncript[500], char vetorValor[500], char vetorChave[53], int tamanhoEntrada,char vetorSaida[52]){
    char vetorDecript[500];
    int j, i;

    for(i = 0; i < 52; i++){
        for(j = 0; j < 52 && vetorEncript[i] != vetorChave[j]; j++){}
        if(j == 52 && vetorChave[i] == '\0'){
            printf("Algum caractere de encript não faz parte dos caracteres aceitos como valor de chave.\n");
            return 0;
        }
        else{
            vetorSaida[i] = vetorValor[j];
        }
    }
    return 1;
}

char* substituicaoTamanhoIndeterminado(char vetorEntrada[500]){
    int tamanhoEntrada = strlen(vetorEntrada);
    char vetorSaida[52];
    char vetorValor[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    char vetorChave[53];
    int verificacoes = (tamanhoEntrada + 51) / 52;

    srand(time(NULL));

    int tamanhoTotalPadded = verificacoes * 52;
    for(int i = tamanhoEntrada; i < tamanhoTotalPadded; i++){
        vetorEntrada[i] = 'A';
    }

    vetorEntrada[tamanhoTotalPadded] = '\0';

    printf("\nVetorEntrada: ");
    for(int i = 0; i < tamanhoEntrada; i++){
       printf("%c, ", vetorEntrada[i]);
    }

    for(int i = 0; i < 52; i++){
        vetorChave[i] = vetorValor[rand() % 52];
    }

    for(int i = 0; i < verificacoes; i++){
        encriptAte52Posicoes(vetorValor + (i * 52),vetorValor , vetorChave, 52, vetorSaida);
    }   

    printf("\nVetor Encriptado: ");
    for(int i = 0; i < tamanhoEntrada; i++){
       printf("%c, ", vetorSaida[i]);
    }

    for(int i = 0; i < verificacoes; i++){
        decriptAte52Posicoes(vetorEntrada + (i * 52), vetorValor, vetorChave, 52, vetorSaida);
    }

    printf("\nVetorDecript: ");
    for(int i = 0; i < tamanhoEntrada; i++){
       printf("%c, ", vetorEntrada[i]);
    }

    return vetorEntrada;
}


/* ---- ARTHUR: Transposicao e Cifras de Fluxo (branch feat/arthur-marques) ---- */


/* ---- MYLLENA: Cifra de Hill (branch myllena_rodrigues) ---- */


/* ---- DYLAN:  ---- */


#endif /* SEGUNDO_TRABALHO_CRIPTO_H */
