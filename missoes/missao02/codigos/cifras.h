#ifndef CIFRAS_H
#define CIFRAS_H

#include "auxiliares.h"
#include "matematica.h"

/* Transposicao e fluxos. */
/* Em Z26, A = 0, B = 1, ..., Z = 25. A transposicao muda apenas
 * as posicoes dos simbolos, preservando seus valores modulo 26. */
/* A chave informa a posicao de origem de cada simbolo da saida.
 * Exemplo: chave 3 1 2 transforma ABC em CAB. */
/* As duas funcoes recebem vetores distintos, tamanho multiplo do bloco
 * e uma chave valida com indices de 0 a bloco - 1. */
static void encriptar_transposicao(const int entrada[], int saida[], size_t tamanho,
               const int chave[], int bloco)
{
    for (size_t inicio = 0; inicio < tamanho; inicio += bloco) {
        for (int i = 0; i < bloco; i++) {
            saida[inicio + i] = entrada[inicio + chave[i]];
        }
    }
}

/* Desfaz a permutacao usando a mesma chave da encriptacao. */
static void decriptar_transposicao(const int entrada[], int saida[], size_t tamanho,
               const int chave[], int bloco)
{
    for (size_t inicio = 0; inicio < tamanho; inicio += bloco) {
        for (int i = 0; i < bloco; i++) {
            saida[inicio + chave[i]] = entrada[inicio + i];
        }
    }
}

/* Fluxo autochave em Z26: z1 = chave e zi = x(i-1).
 * Entrada e chave devem conter valores de 0 a 25; tamanho >= 0.
 * Os vetores possuem pelo menos tamanho elementos e podem ser o mesmo
 * vetor, mas nao devem se sobrepor parcialmente. */
static void encriptar_fluxo(const int entrada[], int saida[], size_t tamanho,
                                  int chave)
{
    int z = chave;
    for (size_t i = 0; i < tamanho; i++) {
        int original = entrada[i];
        saida[i] = (original + z) % 26;
        z = original;
    }
}

static void decriptar_fluxo(const int entrada[], int saida[], size_t tamanho, int chave)
{
    int z = chave;
    for (size_t i = 0; i < tamanho; i++) {
        saida[i] = (entrada[i] - z + 26) % 26;
        z = saida[i]; /* A proxima letra usa o texto original recuperado. */
    }
}

/* O LFSR tem quatro bits. A cada passo, sai o primeiro bit
 * e entra o XOR dos dois primeiros. Semente: de 1 a 15. */
static void encriptar_fluxo_lfsr(const int entrada[], int saida[],
                               size_t tamanho, unsigned int semente)
{
    int bits[4];

    // Separa a semente nos quatro bits do registrador.
    for (int i = 3; i >= 0; i--) {
        bits[i] = semente % 2;
        semente = semente / 2;
    }

    for (size_t i = 0; i < tamanho; i++) {
        saida[i] = entrada[i] ^ bits[0]; // XOR cifra um bit.
        int novo = bits[0] ^ bits[1];

        bits[0] = bits[1];
        bits[1] = bits[2];
        bits[2] = bits[3];
        bits[3] = novo;
    }
}

static void decriptar_fluxo_lfsr(const int entrada[], int saida[],
                                      size_t tamanho, unsigned int semente)
{
    /* Em Z2, soma e subtracao sao XOR. */
    encriptar_fluxo_lfsr(entrada, saida, tamanho, semente);
}

/* Cesar e Vigenere. As saidas devem ser inicializadas com {0}. */
/* Entrada e saida podem ser a mesma estrutura; nao compartilhar ponteiros
 * entre estruturas diferentes. As saidas devem comecar com {0}. */
bool criptCesar(texto *Arquivo, texto *Encriptado, int chave)
{
    chave = aritmetica_modular(chave, 26);
    Encriptado->texto = memoria_dinamica(Encriptado->texto, Arquivo->TamVet, sizeof(int));
    for (size_t i = 0; i < Arquivo->TamVet; i++) {
        Encriptado->texto[i] = (Arquivo->texto[i] + chave) % 26;
    }
    Encriptado->TamVet = Arquivo->TamVet;
    return true;
}

bool decriptCesar(texto *Encriptado, texto *Decript, int chave)
{
    chave = aritmetica_modular(chave, 26);
    Decript->texto = memoria_dinamica(Decript->texto, Encriptado->TamVet, sizeof(int));
    for (size_t i = 0; i < Encriptado->TamVet; i++) {
        Decript->texto[i] = (Encriptado->texto[i] - chave + 26) % 26;
    }
    Decript->TamVet = Encriptado->TamVet;
    return true;
}

bool criptVigenere(texto *Arquivo, texto *Chave, texto *Encriptado)
{
    if (Chave->TamVet == 0 || Chave == Encriptado) return false;
    Encriptado->texto = memoria_dinamica(Encriptado->texto, Arquivo->TamVet, sizeof(int));
    for (size_t i = 0; i < Arquivo->TamVet; i++) {
        Encriptado->texto[i] = (Arquivo->texto[i] + Chave->texto[i % Chave->TamVet]) % 26;
    }
    Encriptado->TamVet = Arquivo->TamVet;
    return true;
}

bool decriptVigenere(texto *Encriptado, texto *Chave, texto *Decriptado)
{
    if (Chave->TamVet == 0 || Chave == Decriptado) return false;
    Decriptado->texto = memoria_dinamica(Decriptado->texto, Encriptado->TamVet, sizeof(int));
    for (size_t i = 0; i < Encriptado->TamVet; i++) {
        Decriptado->texto[i] = (Encriptado->texto[i] - Chave->texto[i % Chave->TamVet] + 26) % 26;
    }
    Decriptado->TamVet = Encriptado->TamVet;
    return true;
}

/* Afim. */
/* Afim altera a string no proprio vetor, independentemente do tamanho.
 * Letras sao convertidas em maiusculas; outros caracteres sao preservados. */
int encript(char *vetor, int a, int b)
{
    a = (a % 26 + 26) % 26;
    b = (b % 26 + 26) % 26;
    if (!eh_coprimo_26(a)) return 0;
    para_maiusculo(vetor);
    for (size_t i = 0; vetor[i] != '\0'; i++) {
        if (vetor[i] >= 'A' && vetor[i] <= 'Z') vetor[i] = (a * (vetor[i] - 'A') + b) % 26 + 'A';
    }
    return 1;
}

int decript(char *vetor, int a, int b)
{
    int inverso = inverso_modular(a, 26);
    if (inverso == -1) return 0;
    b = (b % 26 + 26) % 26;
    para_maiusculo(vetor);
    for (size_t i = 0; vetor[i] != '\0'; i++) {
        if (vetor[i] >= 'A' && vetor[i] <= 'Z') {
            int valor = inverso * (vetor[i] - 'A' - b) % 26;
            vetor[i] = (valor + 26) % 26 + 'A';
        }
    }
    return 1;
}

/* Substituicao. */
/* Alfabeto de 26 letras: maiusculas e minusculas sao equivalentes. */
static int validarAlfabetos(const char *valor, const char *chave)
{
    if (strlen(valor) != 26 || strlen(chave) != 26) return 0;
    int usadosValor[26] = {0}, usadosChave[26] = {0};
    for (size_t i = 0; i < 26; i++) {
        int a = toupper((unsigned char) valor[i]);
        int b = toupper((unsigned char) chave[i]);
        if (a < 'A' || a > 'Z' || b < 'A' || b > 'Z') return 0;
        if (usadosValor[a - 'A'] || usadosChave[b - 'A']) return 0;
        usadosValor[a - 'A'] = usadosChave[b - 'A'] = 1;
    }
    return 1;
}

/* Processa qualquer tamanho de mensagem, mantendo somente letras A-Z.
 * *saida deve iniciar em NULL ou apontar para memoria propria de malloc.
 * Saida distinta da entrada e dos alfabetos. */
int encriptSubstituicao(const char *entrada, const char *valor, const char *chave,
                       size_t tamanhoEntrada, char **saida)
{
    if (!validarAlfabetos(valor, chave) || tamanhoEntrada != strlen(entrada)) return 0;
    if (tamanhoEntrada == SIZE_MAX) return 0;
    char alfabeto[27], chaveMaiuscula[27];
    for (size_t i = 0; i < 26; i++) {
        alfabeto[i] = toupper((unsigned char) valor[i]);
        chaveMaiuscula[i] = toupper((unsigned char) chave[i]);
    }
    alfabeto[26] = chaveMaiuscula[26] = '\0';
    char *novo = memoria_dinamica(NULL, tamanhoEntrada + 1, sizeof(char));
    size_t tamanhoSaida = 0;
    for (size_t i = 0; i < tamanhoEntrada; i++) {
        int letra = toupper((unsigned char) entrada[i]);
        if (letra >= 'A' && letra <= 'Z') {
            const char *posicao = strchr(alfabeto, letra);
            novo[tamanhoSaida++] = chaveMaiuscula[posicao - alfabeto];
        }
    }
    novo[tamanhoSaida] = '\0';
    free(*saida);
    *saida = novo;
    return 1;
}

int decriptSubstituicao(const char *entrada, const char *valor, const char *chave,
                       size_t tamanhoEntrada, char **saida)
{
    return encriptSubstituicao(entrada, chave, valor, tamanhoEntrada, saida);
}

/* Hill: resultados alocados pelas funcoes e liberados pelo chamador. */
/* Retorna texto novo, somente A-Z, com preenchimento X.
 * Quem chama deve liberar o resultado. NULL indica ordem invalida. */
char *prepararTexto(const char *entrada, int ordem)
{
    if (ordem <= 0) return NULL;
    size_t tamanho = strlen(entrada);
    if (tamanho > SIZE_MAX - (size_t) ordem) return NULL;
    char *saida = memoria_dinamica(NULL, tamanho + (size_t) ordem, sizeof(char));
    size_t posicao = 0;
    for (size_t i = 0; entrada[i] != '\0'; i++) {
        int c = toupper((unsigned char) entrada[i]);
        if (c >= 'A' && c <= 'Z') saida[posicao++] = (char) c;
    }
    while (posicao % (size_t) ordem != 0) saida[posicao++] = 'X';
    saida[posicao] = '\0';
    return saida;
}

/* *resultado deve comecar em NULL ou apontar para memoria propria.
 * O resultado anterior e liberado somente em caso de sucesso. */
int cifraHill(const char *texto, int **chave, int ordem, char **resultado)
{
    char *preparado = prepararTexto(texto, ordem);
    if (preparado == NULL) return 0;
    size_t tamanho = strlen(preparado);
    char *saida = memoria_dinamica(NULL, tamanho + 1, sizeof(char));
    int *vetor = memoria_dinamica(NULL, (size_t) ordem, sizeof(int));
    int *produto = memoria_dinamica(NULL, (size_t) ordem, sizeof(int));
    for (size_t inicio = 0; inicio < tamanho; inicio += ordem) {
        for (int i = 0; i < ordem; i++) vetor[i] = preparado[inicio+i] - 'A';
        multiplicarVetorMatriz(vetor, chave, produto, ordem, 26);
        for (int i = 0; i < ordem; i++) saida[inicio+i] = produto[i] + 'A';
    }
    saida[tamanho] = '\0';
    free(preparado);
    free(vetor);
    free(produto);
    free(*resultado);
    *resultado = saida;
    return 1;
}

int decifraHill(const char *texto, int **chave, int ordem, char **resultado)
{
    size_t tamanho = strlen(texto);
    if (ordem <= 0 || tamanho % (size_t) ordem != 0) return 0;
    for (size_t i = 0; i < tamanho; i++) {
        int c = toupper((unsigned char) texto[i]);
        if (c < 'A' || c > 'Z') return 0;
    }
    int **inversa = criarMatriz(ordem);
    if (!calcularInversaHill(chave, inversa, ordem, 26)) {
        liberarMatriz(inversa, ordem);
        return 0;
    }
    int ok = cifraHill(texto, inversa, ordem, resultado);
    liberarMatriz(inversa, ordem);
    return ok;
}



#endif
