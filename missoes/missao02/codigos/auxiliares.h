#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

/* Sem memoria, encerra com erro em vez de continuar com ponteiro nulo. */
static void *memoria_dinamica(void *anterior, size_t quantidade, size_t tamanho)
{
    if (tamanho == 0 || quantidade > SIZE_MAX / tamanho) {
        fprintf(stderr, "Tamanho de memoria invalido.\n");
        exit(EXIT_FAILURE);
    }
    if (quantidade == 0) quantidade = 1;
    void *novo = realloc(anterior, quantidade * tamanho);
    if (novo == NULL) {
        fprintf(stderr, "Memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    return novo;
}

/* Le ate Enter. Retorna NULL no fim da entrada; o chamador libera a linha.
 * 128 e apenas a capacidade inicial, nao um limite para a mensagem. */
static char *ler_linha_dinamica(void)
{
    size_t tamanho = 0, capacidade = 128;
    char *linha = memoria_dinamica(NULL, capacidade, sizeof(char));
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (tamanho == capacidade - 1) {
            if (capacidade > SIZE_MAX / 2) {
                free(linha);
                fprintf(stderr, "Linha grande demais para este computador.\n");
                exit(EXIT_FAILURE);
            }
            capacidade *= 2;
            linha = memoria_dinamica(linha, capacidade, sizeof(char));
        }
        linha[tamanho++] = (char) c;
    }
    if (ferror(stdin)) {
        free(linha);
        fprintf(stderr, "Erro de leitura.\n");
        exit(EXIT_FAILURE);
    }
    if (c == EOF && tamanho == 0) {
        free(linha);
        return NULL;
    }
    if (tamanho > 0 && linha[tamanho - 1] == '\r') tamanho--;
    linha[tamanho] = '\0';
    return linha;
}


/* Valida a linha inteira, inclusive limites e caracteres depois do numero. */
int numero_valido(const char *texto, int minimo, int maximo, int *numero)
{
    char *fim;
    errno = 0;
    long valor = strtol(texto, &fim, 10);
    if (fim == texto || errno != 0 || valor < minimo || valor > maximo) return 0;
    while (isspace((unsigned char) *fim)) fim++;
    if (*fim != '\0') return 0;
    *numero = (int) valor;
    return 1;
}

/* Retorna 0 em EOF; o numero fica separado do indicador de sucesso,
 * permitindo ler tambem -1 e outros valores negativos. */
int ler_numero(const char *mensagem, int minimo, int maximo, int *numero)
{
    while (1) {
        printf("%s", mensagem);
        char *linha = ler_linha_dinamica();
        if (linha == NULL) return 0;
        int valido = numero_valido(linha, minimo, maximo, numero);
        free(linha);
        if (valido) return 1;
        printf("Entrada invalida. Tente novamente.\n");
    }
}

/* Atalho para menus e intervalos nao negativos: -1 indica EOF. */
int pedir_numero(const char *mensagem, int minimo, int maximo)
{
    int numero;
    if (!ler_numero(mensagem, minimo, maximo, &numero)) return -1;
    return numero;
}

int escolher_operacao(void)
{
    return pedir_numero("1 - Cifrar\n2 - Decifrar\nOpcao: ", 1, 2);
}

/* Le o arquivo inteiro. Retorna NULL em erro e fecha o arquivo em todos
 * os caminhos. O conteudo retornado deve ser liberado com free. */
char *ler_arquivo_texto(const char *caminho)
{
    FILE *arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo");
        return NULL;
    }
    size_t tamanho = 0, capacidade = 128;
    char *conteudo = memoria_dinamica(NULL, capacidade, 1);
    int c;
    while ((c = fgetc(arquivo)) != EOF) {
        if (c == 0) {
            printf("Use um arquivo de texto UTF-8 ou ASCII, sem bytes nulos.\n");
            free(conteudo);
            fclose(arquivo);
            return NULL;
        }
        if (tamanho == capacidade - 1) {
            if (capacidade > SIZE_MAX / 2) {
                printf("Arquivo grande demais.\n");
                free(conteudo);
                fclose(arquivo);
                return NULL;
            }
            capacidade *= 2;
            conteudo = memoria_dinamica(conteudo, capacidade, 1);
        }
        conteudo[tamanho++] = (char) c;
    }
    int erro = ferror(arquivo);
    if (fclose(arquivo) != 0) erro = 1;
    if (erro) {
        printf("Erro ao ler o arquivo.\n");
        free(conteudo);
        return NULL;
    }
    conteudo[tamanho] = '\0';
    /* Ignora a marca BOM que alguns editores adicionam ao UTF-8. */
    if (tamanho >= 3 && (unsigned char) conteudo[0] == 0xEF &&
        (unsigned char) conteudo[1] == 0xBB && (unsigned char) conteudo[2] == 0xBF) {
        memmove(conteudo, conteudo + 3, tamanho - 3 + 1);
    }
    return conteudo;
}

/* Somente a mensagem usa esta escolha; chaves continuam no teclado. */
char *ler_mensagem(void)
{
    while (1) {
        int origem = pedir_numero("Mensagem: 1 - Teclado, 2 - Arquivo .txt, 0 - Cancelar: ", 0, 2);
        if (origem <= 0) return NULL;
        if (origem == 1) {
            printf("Digite a mensagem: ");
            return ler_linha_dinamica();
        }
        printf("Caminho do arquivo: ");
        char *caminho = ler_linha_dinamica();
        if (caminho == NULL) return NULL;
        size_t tamanho = strlen(caminho);
        // Aceita caminhos com espacos e aspas externas opcionais.
        char *inicio = caminho;
        if (tamanho >= 2 && caminho[0] == '"' && caminho[tamanho-1] == '"') {
            caminho[tamanho-1] = '\0';
            inicio++;
        }
        char *conteudo = ler_arquivo_texto(inicio);
        free(caminho);
        if (conteudo != NULL) return conteudo;
        printf("Escolha a origem novamente ou cancele.\n");
    }
}

/* Usado para hexadecimais e blocos cifrados de Hill formatados em linhas. */
void remover_espacos(char *texto)
{
    size_t destino = 0;
    for (size_t i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char) texto[i])) texto[destino++] = texto[i];
    }
    texto[destino] = '\0';
}

size_t normalizar(const char texto[], int valores[])
{
    size_t tamanho = 0;

    for (size_t i = 0; texto[i] != '\0'; i++) {
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


void imprimir_letras(const int valores[], size_t tamanho)
{
    for (size_t i = 0; i < tamanho; i++) putchar('A' + valores[i]);
    putchar('\n');
}


#include <stdbool.h>

/* Texto dinamico em Z26. Inicializar com {0}; cada estrutura possui seu vetor. */
typedef struct {
    size_t TamVet;
    int *texto;
} texto;

void liberarTexto(texto *arquivo)
{
    free(arquivo->texto);
    arquivo->texto = NULL;
    arquivo->TamVet = 0;
}

/* Le letras ASCII ate Enter e converte A=0,...,Z=25.
 * Retorna false em EOF. A estrutura deve ter sido inicializada com {0}. */
bool lerTexto(texto *arquivo)
{
    char *linha = ler_linha_dinamica();
    if (linha == NULL) return false;
    int *valores = memoria_dinamica(NULL, strlen(linha), sizeof(int));
    size_t tamanho = normalizar(linha, valores);
    free(linha);
    liberarTexto(arquivo);
    arquivo->texto = valores;
    arquivo->TamVet = tamanho;
    return true;
}

bool ler_mensagem_letras(texto *arquivo)
{
    char *linha = ler_mensagem();
    if (linha == NULL) return false;
    int *valores = memoria_dinamica(NULL, strlen(linha), sizeof(int));
    size_t tamanho = normalizar(linha, valores);
    free(linha);
    liberarTexto(arquivo);
    arquivo->texto = valores;
    arquivo->TamVet = tamanho;
    return true;
}

/* O chamador libera a linha com free. */
char *lerTextoSegundoTrabalho(void)
{
    return ler_linha_dinamica();
}

void para_maiusculo(char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++) str[i] = toupper((unsigned char) str[i]);
}

/* Leitura compartilhada pelo menu geral e pelo programa separado. */
int ler_matriz_hill(int **matriz, int ordem)
{
    printf("Digite cada elemento da matriz em uma linha (0 a 25):\n");
    for (int i = 0; i < ordem; i++) {
        for (int j = 0; j < ordem; j++) {
            if (!ler_numero("Elemento: ", 0, 25, &matriz[i][j])) return 0;
        }
    }
    return 1;
}



/* Gerador pseudoaleatorio didatico. Inicializado uma vez por execucao. */
int sortear_numero(int limite)
{
    static int iniciado = 0;
    if (!iniciado) {
        srand((unsigned int) time(NULL));
        iniciado = 1;
    }
    return rand() % limite; /* O chamador fornece limite positivo. */
}

/* Na decifragem, sempre utiliza a chave informada pelo usuario. */
int escolher_origem_chave(int operacao)
{
    if (operacao == 2) return 1;
    return pedir_numero("Chave: 1 - Informar, 2 - Gerar aleatoria, 0 - Cancelar: ", 0, 2);
}

void gerar_permutacao(int chave[], int tamanho)
{
    for (int i = 0; i < tamanho; i++) chave[i] = i;
    for (int i = tamanho - 1; i > 0; i--) {
        int j = sortear_numero(i + 1);
        int temporario = chave[i];
        chave[i] = chave[j];
        chave[j] = temporario;
    }
}

char *gerar_chave_substituicao(void)
{
    int indices[26];
    gerar_permutacao(indices, 26);
    char *chave = memoria_dinamica(NULL, 27, sizeof(char));
    for (int i = 0; i < 26; i++) chave[i] = 'A' + indices[i];
    chave[26] = '\0';
    return chave;
}

#endif
