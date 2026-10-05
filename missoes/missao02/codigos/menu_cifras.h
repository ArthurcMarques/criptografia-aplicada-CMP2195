#ifndef MENU_CIFRAS_H
#define MENU_CIFRAS_H

#include "cifras.h"

#define MAX_CHAVE 100

static int executar_autochave(void)
{
    int opcao = escolher_operacao();
    if (opcao == -1) return 1;
    printf("Texto sem acentos.\n");
    char *linha = ler_mensagem();
    if (linha == NULL) return 1;
    int *entrada = memoria_dinamica(NULL, strlen(linha), sizeof(int));
    size_t tamanho = normalizar(linha, entrada);
    free(linha);
    if (tamanho == 0) {
        free(entrada);
        printf("O texto deve conter letras.\n");
        return 1;
    }
    int chave = pedir_numero("Chave inicial (0 a 25): ", 0, 25);
    if (chave == -1) { free(entrada); return 1; }
    int *saida = memoria_dinamica(NULL, tamanho, sizeof(int));
    if (opcao == 1) encriptar_fluxo(entrada, saida, tamanho, chave);
    else decriptar_fluxo(entrada, saida, tamanho, chave);
    printf("Texto %s: ", opcao == 1 ? "cifrado" : "decifrado");
    imprimir_letras(saida, tamanho);
    free(entrada);
    free(saida);
    return 0;
}

static int ler_semente(unsigned int *semente)
{
    char *linha;
    while (1) {
        printf("Semente (4 bits, diferente de 0000): ");
        linha = ler_linha_dinamica();
        if (linha == NULL) return 0;
        int valida = strlen(linha) == 4;
        *semente = 0;
        for (int i = 0; valida && i < 4; i++) {
            if (linha[i] != '0' && linha[i] != '1') valida = 0;
            else *semente = *semente * 2 + linha[i] - '0';
        }
        free(linha);
        if (valida && *semente != 0) break;
        printf("Semente invalida. Tente novamente.\n");
    }
    return 1;
}

static int executar_lfsr(void)
{
    const char *hex = "0123456789ABCDEF";
    int opcao = escolher_operacao();
    if (opcao == -1) return 1;
    char *linha;
    while (1) {
        printf("Palavra hexadecimal (sem 0x): ");
        linha = ler_mensagem();
        if (linha == NULL) return 1;
        remover_espacos(linha);
        int valida = linha[0] != '\0';
        for (size_t i = 0; linha[i] != '\0'; i++) {
            if (strchr(hex, toupper((unsigned char) linha[i])) == NULL) valida = 0;
        }
        if (valida) break;
        free(linha);
        printf("Palavra invalida. Tente novamente.\n");
    }
    size_t digitos = strlen(linha);
    // Aloca quatro inteiros por digito sem multiplicar tamanhos sem verificar.
    int *entrada = memoria_dinamica(NULL, digitos, 4 * sizeof(int));
    size_t tamanho = digitos * 4;
    for (size_t i = 0; i < digitos; i++) {
        int valor = (int) (strchr(hex, toupper((unsigned char) linha[i])) - hex);
        for (int b = 3; b >= 0; b--) {
            entrada[i * 4 + b] = valor % 2;
            valor /= 2;
        }
    }
    free(linha);
    unsigned int semente;
    if (!ler_semente(&semente)) {
        free(entrada);
        return 1;
    }
    int *saida = memoria_dinamica(NULL, tamanho, sizeof(int));
    if (opcao == 1) encriptar_fluxo_lfsr(entrada, saida, tamanho, semente);
    else decriptar_fluxo_lfsr(entrada, saida, tamanho, semente);
    printf("Fluxo de chave (bits): ");
    for (size_t i = 0; i < tamanho; i++) putchar('0' + (entrada[i] ^ saida[i]));
    printf("\nTexto %s (hex): ", opcao == 1 ? "cifrado" : "decifrado");
    for (size_t i = 0; i < tamanho; i += 4) {
        int valor = saida[i]*8 + saida[i+1]*4 + saida[i+2]*2 + saida[i+3];
        putchar(hex[valor]);
    }
    putchar('\n');
    free(entrada);
    free(saida);
    return 0;
}

static int ler_permutacao(int chave[], int bloco)
{
    while (1) {
        int usados[MAX_CHAVE] = {0};
        int valida = 1;
        printf("Permutacao de 1 a %d, na mesma linha: ", bloco);
        char *linha = ler_linha_dinamica();
        if (linha == NULL) return 0;
        char *posicao = linha;
        for (int i = 0; i < bloco; i++) {
            char *fim;
            errno = 0;
            long valor = strtol(posicao, &fim, 10);
            if (fim == posicao || errno || valor < 1 || valor > bloco || usados[valor-1]) {
                valida = 0;
                break;
            }
            chave[i] = (int) valor - 1;
            usados[valor-1] = 1;
            posicao = fim;
        }
        while (isspace((unsigned char) *posicao)) posicao++;
        if (*posicao != '\0') valida = 0;
        free(linha);
        if (valida) break;
        printf("Chave invalida. Digite a chave completa novamente.\n");
    }
    return 1;
}

static int executar_transposicao(void)
{
    int opcao = escolher_operacao();
    if (opcao == -1) return 1;
    int bloco = pedir_numero("Tamanho do bloco (1 a 100): ", 1, MAX_CHAVE);
    if (bloco == -1) return 1;
    int chave[MAX_CHAVE];
    if (!ler_permutacao(chave, bloco)) return 1;
    printf("Texto sem acentos.\n");
    char *linha = ler_mensagem();
    if (linha == NULL) return 1;
    size_t capacidade = strlen(linha);
    if (capacidade > SIZE_MAX - (size_t) bloco) {
        free(linha);
        return 1;
    }
    int *entrada = memoria_dinamica(NULL, capacidade + bloco, sizeof(int));
    size_t tamanho = normalizar(linha, entrada);
    free(linha);
    if (tamanho == 0 || (opcao == 2 && tamanho % bloco != 0)) {
        printf("Texto vazio ou tamanho cifrado incompativel com o bloco.\n");
        free(entrada);
        return 1;
    }
    if (opcao == 1) {
        while (tamanho % bloco != 0) entrada[tamanho++] = 'X' - 'A';
    }
    int *saida = memoria_dinamica(NULL, tamanho, sizeof(int));
    if (opcao == 1) encriptar_transposicao(entrada, saida, tamanho, chave, bloco);
    else decriptar_transposicao(entrada, saida, tamanho, chave, bloco);
    printf("Texto %s: ", opcao == 1 ? "cifrado" : "decifrado");
    imprimir_letras(saida, tamanho);
    printf("O preenchimento X e mantido na decifragem.\n");
    free(entrada);
    free(saida);
    return 0;
}

static bool chaveCesar(int *chave)
{
    if (!ler_numero("Digite o valor da chave: ", INT_MIN, INT_MAX, chave)) return false;
    *chave = (*chave % 26 + 26) % 26;
    return true;
}

char *afim(char *vetor, int a, int b)
{
    if (!encript(vetor, a, b)) return NULL;
    printf("\nVetor Encriptado: %s\n", vetor);
    decript(vetor, a, b);
    printf("Vetor Decriptado: %s\n", vetor);
    return vetor;
}

#include <time.h>

char *substituicaoTamanhoIndeterminado(char *entrada)
{
    const char valor[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char chave[27];
    char *cifrado = NULL, *decifrado = NULL;
    strcpy(chave, valor);
    // Embaralhar garante uma chave sem letras repetidas.
    static int inicializado = 0;
    if (!inicializado) { srand((unsigned int) time(NULL)); inicializado = 1; }
    for (int i = 25; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp = chave[i]; chave[i] = chave[j]; chave[j] = temp;
    }
    size_t tamanho = strlen(entrada);
    encriptSubstituicao(entrada, valor, chave, tamanho, &cifrado);
    decriptSubstituicao(cifrado, valor, chave, strlen(cifrado), &decifrado);
    printf("\nChave: %s\nVetor Encriptado: %s\nVetor Decriptado: %s\n", chave, cifrado, decifrado);
    free(cifrado);
    free(decifrado);
    return entrada;
}

static void executar_cesar_vigenere(int vigenere)
{
    int opcao = escolher_operacao();
    if (opcao == -1) return;
    texto entrada = {0}, chave = {0}, saida = {0};
    printf("Texto sem acentos: ");
    if (!ler_mensagem_letras(&entrada)) return;
    if (entrada.TamVet == 0) {
        printf("O texto deve conter letras.\n");
        liberarTexto(&entrada);
        return;
    }
    int ok;
    if (vigenere) {
        do {
            printf("Palavra-chave: ");
            if (!lerTexto(&chave)) {
                liberarTexto(&entrada);
                liberarTexto(&chave);
                return;
            }
            if (chave.TamVet == 0) printf("Chave invalida: informe letras.\n");
        } while (chave.TamVet == 0);
        if (opcao == 1) ok = criptVigenere(&entrada, &chave, &saida);
        else ok = decriptVigenere(&entrada, &chave, &saida);
    } else {
        int deslocamento;
        ok = chaveCesar(&deslocamento);
        if (ok) {
            if (opcao == 1) ok = criptCesar(&entrada, &saida, deslocamento);
            else ok = decriptCesar(&entrada, &saida, deslocamento);
        }
    }
    if (ok) {
        printf("Resultado: ");
        imprimir_letras(saida.texto, saida.TamVet);
    }
    liberarTexto(&entrada);
    liberarTexto(&chave);
    liberarTexto(&saida);
}

static void executar_afim(void)
{
    int opcao = escolher_operacao();
    if (opcao == -1) return;
    int a;
    do {
        a = pedir_numero("Chave A (0 a 25): ", 0, 25);
        if (a == -1) return;
        if (!eh_coprimo_26(a)) printf("Chave invalida: A deve ser coprimo com 26.\n");
    } while (!eh_coprimo_26(a));
    int b = pedir_numero("Chave B (0 a 25): ", 0, 25);
    if (b == -1) return;
    printf("Texto sem acentos: ");
    char *linha = ler_mensagem();
    if (linha == NULL) return;
    if (opcao == 1) encript(linha, a, b);
    else decript(linha, a, b);
    printf("Resultado: %s\n", linha);
    free(linha);
}

static void executar_substituicao(void)
{
    const char *alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int opcao = escolher_operacao();
    if (opcao == -1) return;
    char *chave;
    while (1) {
        printf("Chave: 26 letras sem repeticao, correspondentes a A-Z:\n");
        chave = ler_linha_dinamica();
        if (chave == NULL) return;
        if (validarAlfabetos(alfabeto, chave)) break;
        printf("Chave invalida. Tente novamente.\n");
        free(chave);
    }
    printf("Texto (letras convertidas para A-Z; demais caracteres serao ignorados): ");
    char *linha = ler_mensagem();
    char *saida = NULL;
    if (linha != NULL) {
        if (opcao == 1) encriptSubstituicao(linha, alfabeto, chave, strlen(linha), &saida);
        else decriptSubstituicao(linha, alfabeto, chave, strlen(linha), &saida);
        printf("Resultado: %s\n", saida);
    }
    free(linha);
    free(saida);
    free(chave);
}

static void executar_hill(void)
{
    int opcao = escolher_operacao();
    if (opcao == -1) return;
    int ordem;
    if (!ler_numero("Ordem da matriz: ", 1, INT_MAX, &ordem)) return;
    int **chave = criarMatriz(ordem);
    int **inversa = criarMatriz(ordem);
    int terminou = 0;
    do {
        terminou = !ler_matriz_hill(chave, ordem);
        if (terminou || calcularInversaHill(chave, inversa, ordem, 26)) break;
        printf("Chave invalida: matriz sem inversa em Z26. Digite outra matriz.\n");
    } while (1);
    liberarMatriz(inversa, ordem);
    if (!terminou) {
        printf("Texto sem acentos: ");
        char *linha = ler_mensagem();
        char *saida = NULL;
        if (linha != NULL) {
            int ok;
            if (opcao == 2) remover_espacos(linha);
            if (opcao == 1) ok = cifraHill(linha, chave, ordem, &saida);
            else ok = decifraHill(linha, chave, ordem, &saida);
            if (ok) printf("Resultado: %s\n", saida);
            else printf("Texto invalido: use letras e blocos completos para decifrar.\n");
        }
        free(linha);
        free(saida);
    }
    liberarMatriz(chave, ordem);
}


#endif
