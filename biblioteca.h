#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

/* ===========================================================================
 *  biblioteca.h  -  Missao 01: Teoria dos Numeros / Criptografia Aplicada
 *  Arthur . Myllena . Dylan . Henrique
 *
 *  CONTRATO COMBINADO PELA EQUIPE
 *  ------------------------------
 *  Arthur
 *    euclides(a: int, b: int) -> int
 *    euclides_estendido(a: int, b: int) -> tuple[int, int, int]
 *  Myllena
 *    mdc(a: int, b: int) -> int
 *    aritmetica_modular(a: int, b: int, m: int, op: str) -> int
 *    teorema_chines_resto(restos: list[int], modulos: list[int]) -> tuple[int, int]
 *  Dylan
 *    exp_modular(base: int, exp: int, m: int) -> int
 *    eh_primo(n: int) -> bool
 *  Henrique
 *    inverso_modular(a: int, m: int) -> int
 *    phi_euler(n: int) -> int
 *
 *  COMO O CONTRATO VIRA C
 *  ----------------------
 *  - "tuple" nao existe em C. As duas funcoes que devolvem tupla retornam
 *    um int* apontando para um vetor com os valores na ordem do contrato.
 *  - "list[int]" vira int[] mais um parametro "quantidade": C nao tem len().
 *  - Erro eh sinalizado com -1 (modulo <= 0, expoente negativo, divisao sem
 *    inverso, operador desconhecido, sistema sem solucao).
 *
 *  SOBRE O TAMANHO DO int
 *  ----------------------
 *  As assinaturas sao int, como combinado. So que o int de C tem 32 bits, e
 *  algumas contas INTERMEDIARIAS estouram esse limite antes de voltar para a
 *  faixa do modulo: em exp_modular, d*d ja passa de 2.147.483.647 quando
 *  mod > 46340. Nesses pontos a conta do meio eh feita em long long e o
 *  resultado volta para int - a assinatura continua sendo a combinada.
 *
 *  POR QUE TODO MUNDO PRECISA INCLUIR ESTE HEADER
 *  ----------------------------------------------
 *  Prototipo divergente entre arquivos .c NAO eh erro de compilacao em C:
 *  o linker casa as funcoes so pelo nome, os tipos nao sao conferidos.
 *
 *  O codigo de cada um esta como foi escrito. Toda mudanca esta marcada com
 *  um comentario "ALTERADO" ou "ACRESCENTADO" na propria linha.
 * ======================================================================== */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>   /* ACRESCENTADO: strcmp, usado na aritmetica_modular */

/* --- prototipos --------------------------------------------------------- */

/* ARTHUR   */ int   euclides(int a, int b);
/* ARTHUR   */ int  *euclides_estendido(int a, int b);   /* {mdc, x, y} */
/* MYLLENA  */ int   mdc(int a, int b);
/* MYLLENA  */ int   aritmetica_modular(int a, int b, int m, const char *op);
/* MYLLENA  */ int  *teorema_chines_resto(int restos[], int modulos[],
                                          int quantidade);   /* {resto, modulo} */
/* DYLAN    */ bool  eh_primo(int n);
/* DYLAN    */ int   exp_modular(int base, int expoente, int m);
/* HENRIQUE */ int   inverso_modular(int a, int m);
/* HENRIQUE */ int   phi_euler(int n);


/* ===========================================================================
 *  DYLAN - numerosPrimos.c
 * ======================================================================== */

bool eh_primo(int entrada)
{
    /* ALTERADO: era (entrada == 0 || entrada == 1). Com numero negativo o
       for la embaixo nao rodava nenhuma vez e caia no return true, ou seja,
       eh_primo(-3) devolvia 1. O "< 2" cobre negativo, 0 e 1 de uma vez. */
    if(entrada < 2)
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

    /* ALTERADO: a condicao era (contador < entrada). Se entrada = a*b com
       a <= b, entao a <= raiz(entrada) - parar na raiz ja basta. Para
       1000003 sao ~1000 divisoes em vez de 1000000.
       Escrito como divisao (e nao contador*contador <= entrada) porque o
       produto estouraria o int quando entrada fosse perto do maximo. */
    for(int contador = 2; contador <= entrada / contador; contador++)
    {
        if(entrada % contador == 0)
        {
            return false;
        }

    }
    return true;
}


/* ===========================================================================
 *  DYLAN - exponenciacaoModular.c
 * ======================================================================== */

int exp_modular(int base, int expoente, int mod)
{
    /* ACRESCENTADO: sem isso, expoente negativo devolvia 1 e mod = 0 dava
       divisao por zero. Pela convencao da equipe, erro eh -1. */
    if(expoente < 0 || mod <= 0)
    {
        return -1;
    }

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
        expoente = expoente / 2;   /* ALTERADO: tirado o cast (int), inutil */
        i++;
    }

    /* ACRESCENTADO: base fora da faixa [0, mod-1] (em especial base
       negativa) fazia o resultado sair negativo. */
    base = base % mod;
    if(base < 0)
    {
        base = base + mod;
    }

    int d = 1;

    for(i = contador - 1; i >= 0; i--)
    {
        /* ALTERADO: era d = (d * d) % mod. Com mod > 46340 o produto estoura
           o int ANTES do %, e o resultado saia errado (as vezes negativo).
           A conta do meio passa por long long e volta para int. */
        d = (int)(((long long)d * d) % mod);

        if(binario[i] == 1)
        {
            d = (int)(((long long)d * base) % mod);
        }
    }

    free(binario);   /* ACRESCENTADO: o malloc nunca era liberado */

    return d;

}


/* ===========================================================================
 *  ARTHUR - euclides.c e euclides_estendido.c
 *
 *  ACRESCENTADO: os dois arquivos .c do Arthur subiram com 0 bytes (so os
 *  .exe foram commitados), entao as duas funcoes abaixo foram escritas aqui
 *  para a biblioteca fechar. Substituir pelo codigo dele quando chegar.
 * ======================================================================== */

int euclides(int a, int b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b != 0)
    {
        int resto = a % b;
        a = b;
        b = resto;
    }

    return a;   /* quando b zera, "a" guarda o mdc */
}

/* Devolve a tupla (mdc, x, y) com a*x + b*y = mdc, carregando os
   coeficientes junto com os restos usando o mesmo quociente q.

   ATENCAO: o vetor eh static, ou seja, eh o MESMO a cada chamada. Copie os
   valores para variaveis suas antes de chamar a funcao de novo (direta ou
   indiretamente - inverso_modular tambem chama esta funcao). */
int* euclides_estendido(int a, int b)
{
    static int r[3];
    int x0 = 1, x1 = 0, y0 = 0, y1 = 1;

    while (b != 0) {
        int q = a / b;
        int t;
        t = a - q * b;   a = b;   b = t;
        t = x0 - q * x1; x0 = x1; x1 = t;
        t = y0 - q * y1; y0 = y1; y1 = t;
    }

    r[0] = a;  //mdc
    r[1] = x0; //fator 1
    r[2] = y0; //fator 2

    /* ACRESCENTADO: com entrada negativa o mdc saia negativo. Trocar o sinal
       dos tres de uma vez mantem a identidade a*x + b*y = mdc valida. */
    if (r[0] < 0)
    {
        r[0] = -r[0];
        r[1] = -r[1];
        r[2] = -r[2];
    }

    return r;
}


/* ===========================================================================
 *  MYLLENA - MDC.c
 * ======================================================================== */

int mdc(int a, int b)
{
    int resultado;

    resultado = euclides(a, b);

    return resultado;
}


/* ===========================================================================
 *  MYLLENA - aritmeticaModular.c
 * ======================================================================== */

/* Esta era a aritmetica_modular original, de 2 parametros: ela reduz um
   numero para a faixa [0, x-1].
   ALTERADO: virou funcao auxiliar com outro nome, porque o contrato define
   aritmetica_modular(a, b, m, op) com 4 parametros. O corpo eh o mesmo. */
static int reduz_modulo(int num, int x)
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

/* ACRESCENTADO: versao do contrato, (a OP b) mod m, com OP em
   {"+", "-", "*", "/", "^"}. Reaproveita reduz_modulo em todas elas. */
int aritmetica_modular(int a, int b, int m, const char *op)
{
    long long resultado;   /* long long so na conta do meio: a*b estoura int */

    if (m <= 0 || op == NULL)
    {
        return -1;
    }

    /* reduzir antes de operar nao muda a resposta e diminui o estouro */
    a = reduz_modulo(a, m);
    b = reduz_modulo(b, m);

    if (strcmp(op, "+") == 0)
    {
        resultado = (long long)a + b;
    }
    else if (strcmp(op, "-") == 0)
    {
        resultado = (long long)a - b;
    }
    else if (strcmp(op, "*") == 0)
    {
        resultado = (long long)a * b;
    }
    else if (strcmp(op, "/") == 0)
    {
        /* nao existe fracao aqui: dividir por b eh multiplicar pelo
           inverso de b */
        int inverso = inverso_modular(b, m);

        if (inverso == -1)
        {
            return -1;   /* b nao tem inverso: a divisao nao existe mod m */
        }

        resultado = (long long)a * inverso;
    }
    else if (strcmp(op, "^") == 0)
    {
        return exp_modular(a, b, m);
    }
    else
    {
        return -1;   /* operador invalido */
    }

    resultado = resultado % m;
    if (resultado < 0)
    {
        resultado = resultado + m;
    }

    return (int)resultado;
}


/* ===========================================================================
 *  MYLLENA - teoremaChines.c
 *
 *  Devolve a tupla (resto, modulo). Sistema sem solucao devolve (-1, -1),
 *  seguindo a convencao de erro da equipe - resto valido nunca eh negativo.
 *
 *  ATENCAO: o vetor de retorno tambem eh static, copie antes de chamar de novo.
 * ======================================================================== */

int* teorema_chines_resto(
    int restos[],
    int modulos[],
    int quantidade)   /* ACRESCENTADO: C nao tem len(), a contagem vem junto */
{
    static int solucao[2];   /* ALTERADO: a funcao devolvia so um int; o
                                contrato pede a tupla (resto, modulo) */
    long long M = 1;         /* ALTERADO: int -> long long apenas aqui dentro,
                                o produto dos modulos estoura muito rapido */
    long long M_linha;
    long long inverso;
    long long soma = 0;
    int coprimos = 1;

    solucao[0] = -1;
    solucao[1] = -1;

    if (quantidade <= 0)
    {
        return solucao;
    }

    /* ACRESCENTADO: a formula classica logo abaixo (M, M_linha, inverso) so
       vale quando os modulos sao coprimos dois a dois - ela depende de cada
       M_linha ter inverso. Aqui decidimos qual dos dois caminhos usar. */
    for (int i = 0; i < quantidade; i++)
    {
        if (modulos[i] <= 0)
        {
            return solucao;
        }

        for (int j = i + 1; j < quantidade; j++)
        {
            if (mdc(modulos[i], modulos[j]) != 1)
            {
                coprimos = 0;
            }
        }
    }

    if (coprimos)
    {
        /* ---- algoritmo original, sem mudanca de logica ---- */
        for (int i = 0; i < quantidade; i++)
        {
            M = M * modulos[i];
        }

        for (int i = 0; i < quantidade; i++)
        {
            M_linha = M / modulos[i];

            inverso = inverso_modular(
                reduz_modulo((int)(M_linha % modulos[i]), modulos[i]),
                modulos[i]);   /* ALTERADO: so o nome da auxiliar */

            /* ALTERADO: era soma = soma + restos[i] * M_linha * inverso.
               Sem reduzir a cada passo, o produto dos tres estoura antes de
               chegar no % M final. */
            soma = (soma + (long long)restos[i] % M
                           * (M_linha % M) % M
                           * (inverso % M) % M) % M;
        }

        if (soma < 0)
        {
            soma = soma + M;
        }

        solucao[0] = (int)soma;
        solucao[1] = (int)M;

        return solucao;
    }

    /* ACRESCENTADO: caminho para modulos NAO coprimos.
       Junta as congruencias duas a duas:
           x = r (mod m)  e  x = r2 (mod m2)
       Chamando g = mdc(m, m2), so ha solucao se (r2 - r) for multiplo de g.
       Quando ha, o par vira uma unica congruencia modulo mmc(m, m2). */
    {
        long long r = 0;
        long long m = 1;   /* x = 0 (mod 1) eh verdade para todo x */

        for (int i = 0; i < quantidade; i++)
        {
            long long m2 = modulos[i];
            long long r2 = reduz_modulo(restos[i], modulos[i]);

            int *b = euclides_estendido((int)m, (int)m2);
            long long g  = b[0];
            long long bx = b[1];
            long long diferenca = r2 - r;

            if (diferenca % g != 0)
            {
                return solucao;   /* (-1, -1): sistema impossivel */
            }

            long long mmc   = (m / g) * m2;
            long long passo = m2 / g;
            long long t = ((diferenca / g) % passo * (bx % passo)) % passo;

            if (t < 0)
            {
                t = t + passo;
            }

            r = (r + m * t) % mmc;
            if (r < 0)
            {
                r = r + mmc;
            }
            m = mmc;
        }

        solucao[0] = (int)r;
        solucao[1] = (int)m;
    }

    return solucao;
}


/* ===========================================================================
 *  HENRIQUE - inverso modular
 * ======================================================================== */

int inverso_modular(int numero, int modulo){
    int result;

    /* ACRESCENTADO: modulo <= 0 nao define classe de restos */
    if(modulo <= 0){
        return -1;
    }

    if(mdc(numero,modulo) != 1){
        /* ALTERADO: os dois printf de aviso sairam daqui. Biblioteca que
           imprime polui a saida dos testes; quem explica eh o main.c.
           O -1 continua sinalizando "nao tem inverso". */
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


/* ===========================================================================
 *  HENRIQUE - funcao phi de Euler
 *
 *  Fatora n por divisao sucessiva guardando os primos num vetor e depois
 *  aplica a formula do produto:
 *      n = p1^e1 * p2^e2 * ...   =>   phi(n) = PRODUTO (pi^ei - pi^(ei-1))
 * ======================================================================== */

int phi_euler(int n){

    /* ACRESCENTADO: phi nao eh definida para n <= 0. Antes, n = 0 e n = -5
       caiam direto no fim e devolviam 1 em silencio. */
    if(n <= 0){
        return -1;
    }

    int* primo = (int*)malloc(sizeof(int)*2);
    int contPrimos = 0;
    primo[0] = 2;
    int primoInvalido;

    while(n > 1){ //Enquanto n > 1 não encontramos todos os fatores
        primoInvalido = 1;
        while(primoInvalido){ //só é fator se for primo válido

            if(n % primo[contPrimos] == 0){//Se o primo encontrado dividir n, então é fator e sai do loop
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
        primo = (int*)realloc(primo,sizeof(int)*(contPrimos+2));
    }

    /* primo[contPrimos] eh a copia que serve de cursor da busca, nao eh
       fator: por isso ela eh zerada e o for para antes dela. */
    primo[contPrimos] = 0;
    int expoente = 1;
    int phi = 1;

    for(int i = 0; i < contPrimos; i++){
        if(primo[i] == primo [i+1]){
            expoente++;
            continue;
        }
        else{
            /* ALTERADO: era pow(), que devolve double e depende do
               arredondamento da biblioteca matematica. Aqui a potencia eh
               montada so com multiplicacao inteira, resultado exato. */
            int potencia = 1;
            for(int k = 0; k < expoente; k++){
                potencia = potencia * primo[i];
            }
            phi *= potencia - potencia / primo[i];   /* p^e - p^(e-1) */
            expoente = 1;
        }
    }

    free(primo);
    return phi;
}

#endif /* BIBLIOTECA_H */
