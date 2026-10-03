#ifndef CIFRAS_H
#define CIFRAS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#define MAX_TEXTO 4096
#define MAX_CHAVE 100


/* Em Z26, A = 0, B = 1, ..., Z = 25. A transposicao muda apenas
 * as posicoes dos simbolos, preservando seus valores modulo 26. */
static int normalizar(const char texto[], int valores[])
{
    int tamanho = 0;

    for (int i = 0; texto[i] != '\0'; i++) {
        unsigned char c = (unsigned char) texto[i];

        if (c >= 'a' && c <= 'z') {
            c = (unsigned char) (c - 'a' + 'A');
        }
        if (c >= 'A' && c <= 'Z') {
            valores[tamanho++] = c - 'A';
        }
    }
    return tamanho;
}

/* A chave informa a posicao de origem de cada simbolo da saida.
 * Exemplo: chave 3 1 2 transforma ABC em CAB. */
/* As duas funcoes recebem vetores distintos, tamanho multiplo do bloco
 * e uma chave valida com indices de 0 a bloco - 1. */
static void encriptar_transposicao(const int entrada[], int saida[], int tamanho,
               const int chave[], int bloco)
{
    for (int inicio = 0; inicio < tamanho; inicio += bloco) {
        for (int i = 0; i < bloco; i++) {
            saida[inicio + i] = entrada[inicio + chave[i]];
        }
    }
}

/* Desfaz a permutacao usando a mesma chave da encriptacao. */
static void decriptar_transposicao(const int entrada[], int saida[], int tamanho,
               const int chave[], int bloco)
{
    for (int inicio = 0; inicio < tamanho; inicio += bloco) {
        for (int i = 0; i < bloco; i++) {
            saida[inicio + chave[i]] = entrada[inicio + i];
        }
    }
}

/* Fluxo autochave em Z26: z1 = chave e zi = x(i-1).
 * Entrada e chave devem conter valores de 0 a 25; tamanho >= 0.
 * Os vetores possuem pelo menos tamanho elementos e podem ser o mesmo
 * vetor, mas nao devem se sobrepor parcialmente. */
static void encriptar_fluxo(const int entrada[], int saida[], int tamanho,
                                  int chave)
{
    int z = chave;
    for (int i = 0; i < tamanho; i++) {
        int original = entrada[i];
        saida[i] = (original + z) % 26;
        z = original;
    }
}

static void decriptar_fluxo(const int entrada[], int saida[], int tamanho, int chave)
{
    int z = chave;
    for (int i = 0; i < tamanho; i++) {
        saida[i] = (entrada[i] - z + 26) % 26;
        z = saida[i]; /* A proxima letra usa o texto original recuperado. */
    }
}

/* O LFSR tem quatro bits. A cada passo, sai o primeiro bit
 * e entra o XOR dos dois primeiros. Semente: de 1 a 15. */
static void encriptar_fluxo_lfsr(const int entrada[], int saida[],
                               int tamanho, unsigned int semente)
{
    int bits[4];

    // Separa a semente nos quatro bits do registrador.
    for (int i = 3; i >= 0; i--) {
        bits[i] = semente % 2;
        semente = semente / 2;
    }

    for (int i = 0; i < tamanho; i++) {
        saida[i] = entrada[i] ^ bits[0]; // XOR cifra um bit.
        int novo = bits[0] ^ bits[1];

        bits[0] = bits[1];
        bits[1] = bits[2];
        bits[2] = bits[3];
        bits[3] = novo;
    }
}

static void decriptar_fluxo_lfsr(const int entrada[], int saida[],
                                      int tamanho, unsigned int semente)
{
    /* Em Z2, soma e subtracao sao XOR. */
    encriptar_fluxo_lfsr(entrada, saida, tamanho, semente);
}

/* Entrada e execucao das cifras pelo menu. */
/* Retorna -1 para linha longa e 0 para fim da entrada. */
static int ler_linha(char *linha)
{
    if (fgets(linha, MAX_TEXTO + 2, stdin) == NULL) {
        return 0;
    }
    linha[strcspn(linha, "\r\n")] = '\0';
    if (strlen(linha) > MAX_TEXTO) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        return -1;
    }
    return 1;
}

// Le um numero de uma linha e verifica se esta no intervalo permitido.
static int numero_valido(char texto[], int minimo, int maximo, int *numero)
{
    char *fim;
    errno = 0;
    long valor = strtol(texto, &fim, 10);
    if (fim == texto || errno != 0) {
        return 0;
    }
    while (isspace((unsigned char) *fim)) {
        fim++;
    }
    if (*fim != '\0' || valor < minimo || valor > maximo) {
        return 0;
    }
    *numero = (int) valor;
    return 1;
}

static int executar_autochave(void)
{
    char linha[MAX_TEXTO + 2];
    int entrada[MAX_TEXTO];
    int saida[MAX_TEXTO];
    int chave;
    int opcao;
    int c;

    printf("Cifra de fluxo autochave em Z26\n");
    printf("1 - Cifrar\n2 - Decifrar\nOpcao: ");
    if (scanf("%d", &opcao) != 1 || (opcao != 1 && opcao != 2)) {
        printf("Opcao invalida.\n");
        return 1;
    }
    while ((c = getchar()) != '\n' && c != EOF) {
    }

    printf("Texto (sem acentos; espacos e pontuacao serao ignorados): ");
    if (ler_linha(linha) != 1) {
        printf("Erro de leitura ou texto maior que %d bytes.\n", MAX_TEXTO);
        return 1;
    }
    int tamanho = normalizar(linha, entrada);
    if (tamanho == 0) {
        printf("O texto deve conter pelo menos uma letra de A a Z.\n");
        return 1;
    }

    while (1) {
        printf("Chave inicial K (inteiro de 0 a 25): ");
        int leitura = ler_linha(linha);
        if (leitura == 0) {
            printf("Entrada encerrada.\n");
            return 1;
        }
        if (leitura == 1 && numero_valido(linha, 0, 25, &chave)) {
            break;
        }
        printf("Chave invalida: informe um inteiro de 0 a 25. Tente novamente.\n");
    }

    if (opcao == 1) {
        encriptar_fluxo(entrada, saida, tamanho, chave);
    } else {
        decriptar_fluxo(entrada, saida, tamanho, chave);
    }
    printf("Texto %s: ", opcao == 1 ? "cifrado" : "decifrado");
    for (int i = 0; i < tamanho; i++) {
        putchar('A' + saida[i]);
    }
    putchar('\n');
    return 0;
}


static int executar_lfsr(void)
{
    char linha[MAX_TEXTO + 2];
    int entrada[MAX_TEXTO * 4];
    int saida[MAX_TEXTO * 4];
    int tamanho;
    int opcao;
    unsigned int semente;
    const char *hex = "0123456789ABCDEF";

    printf("1 - Cifrar\n2 - Decifrar\nOpcao: ");
    if (ler_linha(linha) != 1 ||
        (strcmp(linha, "1") != 0 && strcmp(linha, "2") != 0)) {
        printf("Opcao invalida.\n");
        return 1;
    }
    opcao = linha[0] - '0';

    while (1) {
        printf("Palavra hexadecimal (exemplo: DF0E01; sem prefixo 0x): ");
        int leitura = ler_linha(linha);
        if (leitura == 0) {
            printf("Entrada encerrada.\n");
            return 1;
        }
        int valida = leitura == 1 && linha[0] != '\0';
        tamanho = 0;
        for (int i = 0; valida && linha[i] != '\0'; i++) {
            char letra = toupper((unsigned char) linha[i]);
            int valor;
            if (letra >= '0' && letra <= '9') {
                valor = letra - '0';
            } else if (letra >= 'A' && letra <= 'F') {
                valor = letra - 'A' + 10;
            } else {
                valida = 0;
                break;
            }
            // Cada digito hexadecimal corresponde a quatro bits.
            for (int bit = 3; bit >= 0; bit--) {
                entrada[tamanho + bit] = valor % 2;
                valor = valor / 2;
            }
            tamanho = tamanho + 4;
        }
        if (valida) {
            break;
        }
        printf("Palavra invalida: use de 1 a %d digitos hexadecimais.\n", MAX_TEXTO);
    }

    while (1) {
        printf("Semente (4 bits, diferente de 0000; exemplo: 0111): ");
        int leitura = ler_linha(linha);
        if (leitura == 0) {
            printf("Entrada encerrada.\n");
            return 1;
        }
        int valida = leitura == 1 && strlen(linha) == 4;
        semente = 0;
        for (int i = 0; valida && i < 4; i++) {
            if (linha[i] != '0' && linha[i] != '1') {
                valida = 0;
            } else {
                semente = semente * 2 + (linha[i] - '0');
            }
        }
        if (valida && semente != 0) {
            break;
        }
        printf("Semente invalida. Tente novamente.\n");
    }

    if (opcao == 1) {
        encriptar_fluxo_lfsr(entrada, saida, tamanho, semente);
    } else {
        decriptar_fluxo_lfsr(entrada, saida, tamanho, semente);
    }
    printf("Fluxo de chave (bits): ");
    for (int i = 0; i < tamanho; i++) {
        putchar('0' + (entrada[i] ^ saida[i]));
    }
    printf("\nTexto %s (hex): ", opcao == 1 ? "cifrado" : "decifrado");
    for (int i = 0; i < tamanho; i += 4) {
        int valor = saida[i] * 8 + saida[i+1] * 4 +
                    saida[i+2] * 2 + saida[i+3];
        putchar(hex[valor]);
    }
    putchar('\n');
    return 0;
}

static int executar_transposicao(void)
{
    char texto[MAX_TEXTO + 2];
    int entrada[MAX_TEXTO + MAX_CHAVE];
    int saida[MAX_TEXTO + MAX_CHAVE];
    int chave[MAX_CHAVE];
    int opcao;
    int bloco;
    int c;

    printf("Cifra de transposicao em Z26\n");
    printf("1 - Cifrar\n2 - Decifrar\nOpcao: ");
    if (scanf("%d", &opcao) != 1 || (opcao != 1 && opcao != 2)) {
        printf("Opcao invalida.\n");
        return 1;
    }

    printf("Tamanho do bloco (1 a %d): ", MAX_CHAVE);
    if (scanf("%d", &bloco) != 1 || bloco < 1 || bloco > MAX_CHAVE) {
        printf("Tamanho de bloco invalido.\n");
        return 1;
    }

    while (1) {
        int usados[MAX_CHAVE] = {0};
        int chave_valida = 1;

        printf("Digite uma permutacao de 1 a %d (exemplo para 3: 3 1 2): ", bloco);
        for (int i = 0; i < bloco; i++) {
            int posicao;
            if (scanf("%d", &posicao) != 1 || posicao < 1 || posicao > bloco) {
                printf("Chave invalida: use apenas posicoes de 1 a %d.\n", bloco);
                chave_valida = 0;
                break;
            }
            if (usados[posicao - 1]) {
                printf("Chave invalida: as posicoes nao podem se repetir.\n");
                chave_valida = 0;
                break;
            }
            chave[i] = posicao - 1;
            usados[posicao - 1] = 1;
        }

        while ((c = getchar()) != '\n' && c != EOF) {
            /* Descarta o restante da tentativa antes de ler outra entrada. */
        }
        if (c == EOF) {
            printf("Entrada encerrada.\n");
            return 1;
        }
        if (chave_valida) {
            break;
        }
        printf("Digite a chave completa novamente.\n");
    }
    printf("Texto (sem acentos; espacos e pontuacao serao ignorados): ");
    if (fgets(texto, sizeof(texto), stdin) == NULL) {
        printf("Erro ao ler o texto.\n");
        return 1;
    }
    texto[strcspn(texto, "\r\n")] = '\0';
    if (strlen(texto) > MAX_TEXTO) {
        printf("Texto muito longo: limite de %d caracteres.\n", MAX_TEXTO);
        return 1;
    }

    int tamanho = normalizar(texto, entrada);
    if (tamanho == 0) {
        printf("O texto deve conter pelo menos uma letra de A a Z.\n");
        return 1;
    }

    if (opcao == 1) {
        int preenchimento = (bloco - tamanho % bloco) % bloco;
        for (int i = 0; i < preenchimento; i++) {
            entrada[tamanho++] = 'X' - 'A';
        }
        printf("Preenchimento: %d letra(s) X.\n", preenchimento);
    } else if (tamanho % bloco != 0) {
        printf("O tamanho do texto cifrado deve ser multiplo do bloco.\n");
        return 1;
    }

    if (opcao == 1) {
        encriptar_transposicao(entrada, saida, tamanho, chave, bloco);
    } else {
        decriptar_transposicao(entrada, saida, tamanho, chave, bloco);
    }
    printf("Texto %s: ", opcao == 1 ? "cifrado" : "decifrado");
    for (int i = 0; i < tamanho; i++) {
        putchar('A' + saida[i]);
    }
    putchar('\n');
    if (opcao == 2) {
        printf("O preenchimento X e mantido para preservar letras do texto original.\n");
    }
    return 0;
}

#endif
