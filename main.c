/* Necessario no MinGW antigo para o printf entender %lld */
#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include "Primeiro_Trabalho_Cripto.h"

/* ===========================================================================
 *  main.c - bateria de testes da Missao 01
 *
 *  Cada secao mostra PRIMEIRO o algoritmo passo a passo (para explicar na
 *  apresentacao) e DEPOIS os testes que provam que ele funciona.
 *
 *  Duas marcacoes diferentes:
 *    [OK] / [FALHA]  -> integracao: as pecas conversando entre si
 *    [PENDENTE]      -> bug conhecido dentro do codigo de uma peca,
 *                       ainda nao corrigido pelo autor. Nao conta como
 *                       falha de integracao.
 * ======================================================================== */

static int total = 0, ok = 0, pend = 0;

static void checa(const char *nome, long long obtido, long long esperado)
{
    total++;
    if (obtido == esperado) {
        ok++;
        printf("  [OK]    %-44s = %lld\n", nome, obtido);
    } else {
        printf("  [FALHA] %-44s = %lld (esperado %lld)\n", nome, obtido, esperado);
    }
}

/* Bug ja conhecido no codigo de outra pessoa: registra sem reprovar. */
static void pendente(const char *nome, long long obtido, long long correto)
{
    pend++;
    printf("  [PEND.] %-44s = %lld (correto %lld)\n", nome, obtido, correto);
}

static void titulo(const char *texto)
{
    printf("\n============================================================\n");
    printf(" %s\n", texto);
    printf("============================================================\n");
}

static void subtitulo(const char *texto)
{
    printf("\n--- %s ---\n", texto);
}

/* ---------------------------------------------------------------------------
 *  Demonstracoes passo a passo
 * ------------------------------------------------------------------------ */

/* Mostra cada divisao do algoritmo de Euclides ate o resto zerar. */
static void demo_euclides(long long a, long long b)
{
    printf("  Calculando mdc(%lld, %lld) por divisoes sucessivas:\n", a, b);
    while (b != 0) {
        long long q = a / b;
        long long r = a % b;
        printf("      %4lld = %2lld x %4lld + %lld\n", a, q, b, r);
        a = b;
        b = r;
    }
    printf("  O ultimo resto nao nulo eh o mdc  ->  %lld\n", a);
}

/* Mostra a identidade de Bezout com os numeros substituidos.
   Copia a tupla para variaveis locais logo apos a chamada: o vetor devolvido
   eh static, a proxima chamada o sobrescreve. */
static void demo_bezout(long long a, long long b)
{
    long long *t = euclides_estendido(a, b);
    long long g = t[0], x = t[1], y = t[2];

    printf("  Bezout para (%lld, %lld):\n", a, b);
    printf("      mdc = %lld,  x = %lld,  y = %lld\n", g, x, y);
    printf("      %lld x (%lld) + %lld x (%lld) = %lld   <- confere com o mdc\n",
           a, x, b, y, a * x + b * y);
}

/* Mostra o expoente em binario e quantas multiplicacoes o metodo economiza. */
static void demo_exp_modular(int base, int expoente, int mod)
{
    int e = expoente;
    int bits[32], n = 0;

    while (e > 0) { bits[n++] = e % 2; e /= 2; }

    printf("  Calculando %d^%d mod %d\n", base, expoente, mod);
    printf("      expoente %d em binario = ", expoente);
    for (int i = n - 1; i >= 0; i--) printf("%d", bits[i]);
    printf("  (%d bits)\n", n);
    printf("      metodo ingenuo ...... %d multiplicacoes\n", expoente);
    printf("      square-and-multiply . ~%d multiplicacoes\n", 2 * n);
    printf("      resultado = %d\n", exp_modular(base, expoente, mod));
}

/* Mostra a fatoracao de n e a formula do produto usada no phi. */
static void demo_phi(int n)
{
    int resto = n;
    int phi = 1;
    int primeiro = 1;

    printf("  Fatorando %d: ", n);
    for (int p = 2; p <= resto / p; p++) {
        if (resto % p == 0) {
            int e = 0;
            while (resto % p == 0) { resto /= p; e++; }
            printf("%s%d^%d", primeiro ? "" : " x ", p, e);
            primeiro = 0;
        }
    }
    if (resto > 1) printf("%s%d^1", primeiro ? "" : " x ", resto);
    printf("\n");

    resto = n;
    primeiro = 1;
    printf("  phi(%d) = ", n);
    for (int p = 2; p <= resto / p; p++) {
        if (resto % p == 0) {
            int pot = 1;
            while (resto % p == 0) { resto /= p; pot *= p; }
            printf("%s(%d - %d)", primeiro ? "" : " x ", pot, pot / p);
            phi *= pot - pot / p;
            primeiro = 0;
        }
    }
    if (resto > 1) { printf("%s(%d - 1)", primeiro ? "" : " x ", resto); phi *= resto - 1; }
    printf(" = %d\n", phi);
}

/* Mostra o sistema de congruencias e confere a solucao encontrada.
   teorema_chines_resto devolve so o resto, entao o mmc eh calculado aqui
   para exibicao, usando o mdc da equipe. */
static void demo_tcr(int r[], int m[], int q, const char *rotulo)
{
    int x, mmc = 1;

    printf("  %s\n", rotulo);
    for (int i = 0; i < q; i++) {
        printf("      x = %d (mod %d)\n", r[i], m[i]);
        mmc = mmc / mdc(mmc, m[i]) * m[i];
    }

    x = teorema_chines_resto(r, m, q);

    printf("      -> x = %d (mod %d)\n", x, mmc);
    printf("         conferindo: ");
    for (int i = 0; i < q; i++) {
        printf("%d mod %d = %d%s", x, m[i], x % m[i],
               i == q - 1 ? "\n" : ",  ");
    }
}

/* ---------------------------------------------------------------------------
 *  main
 * ------------------------------------------------------------------------ */

int main(void)
{
    printf("############################################################\n");
    printf("#  MISSAO 01 - TEORIA DOS NUMEROS APLICADA A CRIPTOGRAFIA   #\n");
    printf("#  Arthur . Myllena . Dylan . Henrique                      #\n");
    printf("############################################################\n");

    /* ==================================================================== */
    titulo("ARTHUR - Algoritmo de Euclides");
    /* ==================================================================== */

    printf("\nO mdc nao muda se trocarmos (a, b) por (b, a mod b). Repetindo\n");
    printf("isso, os numeros encolhem rapido ate o resto zerar.\n\n");
    demo_euclides(270, 192);

    printf("\nO Euclides ESTENDIDO guarda, alem do mdc, dois coeficientes\n");
    printf("x e y com a*x + b*y = mdc. Eles sao a base do inverso modular.\n\n");
    demo_bezout(240, 46);

    subtitulo("testes");
    checa("euclides(48, 18)", euclides(48, 18), 6);
    checa("euclides(270, 192)", euclides(270, 192), 6);
    checa("euclides(17, 0)", euclides(17, 0), 17);
    checa("euclides(-48, 18)", euclides(-48, 18), 6);
    checa("euclides(0, 0)", euclides(0, 0), 0);
    {
        long long *t = euclides_estendido(240, 46);
        long long g = t[0], x = t[1], y = t[2];
        checa("euclides_estendido(240,46) -> mdc", g, 2);
        checa("240*x + 46*y = mdc", 240 * x + 46 * y, g);

        t = euclides_estendido(7, 26);
        g = t[0]; x = t[1]; y = t[2];
        checa("euclides_estendido(7,26) -> mdc", g, 1);
        checa("7*x + 26*y = mdc", 7 * x + 26 * y, g);

        /* entrada negativa: o Arthur trata invertendo o coeficiente inicial */
        t = euclides_estendido(-48, 18);
        g = t[0]; x = t[1]; y = t[2];
        checa("euclides_estendido(-48,18) -> mdc", g, 6);
        checa("(-48)*x + 18*y = mdc", -48 * x + 18 * y, g);

        /* numero grande: so cabe porque o Arthur usou long long */
        t = euclides_estendido(123456789012LL, 987654321098LL);
        checa("euclides_estendido(1.2e11, 9.8e11) -> mdc", t[0], 2);
    }

    /* ==================================================================== */
    titulo("MYLLENA - MDC, Aritmetica Modular e Teorema Chines do Resto");
    /* ==================================================================== */

    printf("\nO mdc dela reaproveita o euclides do Arthur - eh o primeiro\n");
    printf("ponto onde duas partes da equipe se encaixam.\n");
    printf("  mdc(270, 192) = %d\n", mdc(270, 192));

    printf("\nA aritmetica_modular reduz qualquer numero para a faixa\n");
    printf("[0, m-1]. Operacao composta se monta em cima dela:\n\n");
    printf("  (14 + 25) mod 12  ->  aritmetica_modular(14 + 25, 12)   = %d\n",
           aritmetica_modular(14 + 25, 12));
    printf("  (4 - 9)   mod 7   ->  aritmetica_modular(4 - 9, 7)      = %d\n",
           aritmetica_modular(4 - 9, 7));
    printf("  (123*456) mod 1000->  aritmetica_modular(123*456, 1000) = %d\n",
           aritmetica_modular(123 * 456, 1000));
    printf("\n  DIVIDIR eh diferente: nao existe fracao, a/b vira a * (inverso de b).\n");
    printf("  (3 / 7)   mod 26  ->  3 x %d = %d  ->  mod 26 = %d\n",
           inverso_modular(7, 26), 3 * inverso_modular(7, 26),
           aritmetica_modular(3 * inverso_modular(7, 26), 26));
    printf("  (2 ^ 10)  mod 1000->  exp_modular(2, 10, 1000)          = %d\n",
           exp_modular(2, 10, 1000));

    printf("\nO TCR junta varias congruencias numa unica resposta.\n\n");
    {
        int r[3] = {2, 3, 2};
        int m[3] = {3, 5, 7};
        demo_tcr(r, m, 3, "Sistema classico (modulos coprimos):");
    }
    printf("\n");
    {
        int r[2] = {2, 3};
        int m[2] = {5, 7};
        demo_tcr(r, m, 2, "Duas congruencias:");
    }

    subtitulo("testes");
    checa("mdc(48, 18)", mdc(48, 18), 6);
    checa("mdc(0, 5)", mdc(0, 5), 5);
    checa("mdc(13, 17)", mdc(13, 17), 1);
    checa("mdc(-48, 18)", mdc(-48, 18), 6);
    checa("mdc usa o euclides do Arthur", mdc(270, 192), euclides(270, 192));

    checa("(14 + 25) mod 12", aritmetica_modular(14 + 25, 12), 3);
    checa("(4 - 9) mod 7  (negativo)", aritmetica_modular(4 - 9, 7), 2);
    checa("(123 * 456) mod 1000", aritmetica_modular(123 * 456, 1000), 88);
    checa("(3 / 7) mod 26 = 3 * inv(7,26)",
          aritmetica_modular(3 * inverso_modular(7, 26), 26), 19);
    checa("(2 ^ 10) mod 1000", exp_modular(2, 10, 1000), 24);
    checa("modulo invalido -> -1", aritmetica_modular(3, 0), -1);

    {
        int r[3] = {2, 3, 2};
        int m[3] = {3, 5, 7};
        checa("TCR {2,3,2} / {3,5,7}", teorema_chines_resto(r, m, 3), 23);
    }
    {
        int r[2] = {2, 3};
        int m[2] = {5, 7};
        checa("TCR {2,3} / {5,7}", teorema_chines_resto(r, m, 2), 17);
    }
    {
        int r[2] = {1, 2};
        int m[2] = {2, 3};
        checa("TCR {1,2} / {2,3}", teorema_chines_resto(r, m, 2), 5);
    }
    {
        int r[4] = {1, 2, 3, 4};
        int m[4] = {2, 3, 5, 7};
        checa("TCR com 4 congruencias", teorema_chines_resto(r, m, 4), 53);
    }

    /* ==================================================================== */
    titulo("DYLAN - Primalidade e Exponenciacao Modular");
    /* ==================================================================== */

    printf("\nA exponenciacao rapida le o expoente em binario: a cada bit\n");
    printf("eleva a base ao quadrado, e so multiplica quando o bit eh 1.\n\n");
    demo_exp_modular(7, 128, 13);
    printf("\n");
    demo_exp_modular(2, 10, 1000);

    subtitulo("testes");
    checa("eh_primo(1)", eh_primo(1), 0);
    checa("eh_primo(2)", eh_primo(2), 1);
    checa("eh_primo(97)", eh_primo(97), 1);
    checa("eh_primo(91) = 7*13", eh_primo(91), 0);
    checa("eh_primo(1000003)", eh_primo(1000003), 1);
    checa("(2 ^ 10) mod 1000", exp_modular(2, 10, 1000), 24);
    checa("(7 ^ 128) mod 13", exp_modular(7, 128, 13), 3);
    checa("(123456 ^ 0) mod 7", exp_modular(123456, 0, 7), 1);
    checa("(5 ^ 3) mod 1", exp_modular(5, 3, 1), 0);
    checa("(3 ^ 100) mod 7", exp_modular(3, 100, 7), 4);

    /* ==================================================================== */
    titulo("HENRIQUE - Inverso Modular e Funcao Phi de Euler");
    /* ==================================================================== */

    printf("\nO inverso de a mod m eh o x com a*x = 1 (mod m). Ele sai de\n");
    printf("graca do Euclides estendido: se a*x + m*y = 1, ao reduzir mod m\n");
    printf("o termo m*y some e sobra a*x = 1. Logo o proprio x eh o inverso.\n\n");
    demo_bezout(7, 26);
    printf("      reduzindo x mod 26: %d  ->  7 x %d = %d = 1 (mod 26)\n",
           inverso_modular(7, 26), inverso_modular(7, 26),
           7 * inverso_modular(7, 26));

    printf("\nphi(n) conta quantos numeros de 1 a n sao primos com n.\n");
    printf("Fatorando n, cada primo contribui com (p^e - p^(e-1)).\n\n");
    demo_phi(360);
    printf("\n");
    demo_phi(33);

    subtitulo("testes");
    checa("inverso_modular(7, 26)", inverso_modular(7, 26), 15);
    checa("inverso_modular(3, 11)", inverso_modular(3, 11), 4);
    checa("inverso_modular(17, 3120)", inverso_modular(17, 3120), 2753);
    checa("7 * inv(7,26) = 1 (mod 26)",
          aritmetica_modular(7 * inverso_modular(7, 26), 26), 1);
    checa("phi_euler(1)", phi_euler(1), 1);
    checa("phi_euler(9)", phi_euler(9), 6);
    checa("phi_euler(13) primo", phi_euler(13), 12);
    checa("phi_euler(3 * 11)", phi_euler(33), 20);
    checa("phi_euler(360)", phi_euler(360), 96);
    checa("phi_euler(1024) = 2^10", phi_euler(1024), 512);
    checa("phi_euler(1000003) primo", phi_euler(1000003), 1000002);

    printf("\n  (a linha impressa abaixo vem de dentro da biblioteca)\n");
    checa("inverso_modular(6, 12) sem inverso", inverso_modular(6, 12), -1);

    /* ==================================================================== */
    titulo("FECHAMENTO - as 4 partes juntas dentro do RSA");
    /* ==================================================================== */

    {
        int p = 61, q = 53;
        int n = p * q;
        int phi = phi_euler(n);
        int e = 17;
        int d = inverso_modular(e, phi);
        int mensagem = 65;
        int cifrado, decifrado;

        printf("\n1) DYLAN escolhe dois primos:  p = %d (primo? %d)  "
               "q = %d (primo? %d)\n", p, eh_primo(p), q, eh_primo(q));
        printf("2) O modulo publico eh n = p x q = %d\n", n);
        printf("3) HENRIQUE calcula phi(n) = %d    "
               "(confere: (p-1)(q-1) = %d)\n", phi, (p - 1) * (q - 1));
        printf("4) Escolhe-se e = %d. MYLLENA/ARTHUR conferem mdc(e, phi) = %d\n",
               e, mdc(e, phi));
        printf("5) HENRIQUE acha a chave privada d = inverso de e mod phi = %d\n", d);
        printf("   conferindo: (e x d) mod phi = %d\n",
               aritmetica_modular(e * d, phi));

        cifrado   = exp_modular(mensagem, e, n);
        decifrado = exp_modular(cifrado, d, n);

        printf("\n6) Mensagem original .... %d\n", mensagem);
        printf("7) Cifrando  m^e mod n .. %d\n", cifrado);
        printf("8) Decifrando c^d mod n . %d\n", decifrado);
        printf("\n   As 4 partes da equipe foram usadas nesse ciclo.\n\n");

        checa("RSA: phi(3233)", phi, 3120);
        checa("RSA: mdc(e, phi) = 1", mdc(e, phi), 1);
        checa("RSA: chave privada d", d, 2753);
        checa("RSA: e*d = 1 (mod phi)", aritmetica_modular(e * d, phi), 1);
        checa("RSA: mensagem cifrada", cifrado, 2790);
        checa("RSA: decifrado == original", decifrado, mensagem);
    }

    /* ==================================================================== */
    titulo("PENDENCIAS - bugs conhecidos, cada um no codigo do autor");
    /* ==================================================================== */

    printf("\nNao sao falhas de integracao: as pecas se encaixam. Sao bugs\n");
    printf("dentro de uma peca, que cabe ao autor corrigir.\n\n");

    printf("  DYLAN - eh_primo / exp_modular:\n");
    pendente("eh_primo(-3) negativo", eh_primo(-3), 0);
    pendente("exp_modular(2, -1, 7) expoente negativo", exp_modular(2, -1, 7), -1);
    pendente("exp_modular(123456, 5, 1000003) overflow",
             exp_modular(123456, 5, 1000003), 63218);

    printf("\n  MYLLENA - TCR com modulos NAO coprimos (a formula classica\n");
    printf("  exige coprimos; as linhas soltas vem do inverso_modular):\n");
    {
        int r[2] = {3, 5};
        int m[2] = {4, 6};
        pendente("TCR {3,5} / {4,6} nao coprimo",
                 teorema_chines_resto(r, m, 2), 11);
    }

    printf("\n  HENRIQUE - phi_euler com entrada invalida:\n");
    pendente("phi_euler(0) deveria acusar erro", phi_euler(0), -1);

    /* ==================================================================== */
    printf("\n============================================================\n");
    printf(" INTEGRACAO : %d/%d testes passaram\n", ok, total);
    printf(" PENDENCIAS : %d bugs conhecidos, listados acima\n", pend);
    if (ok == total) {
        printf("\n As quatro partes estao integradas e funcionando.\n");
    } else {
        printf("\n ATENCAO: %d teste(s) de integracao falhando.\n", total - ok);
    }
    printf("============================================================\n");

    return ok == total ? 0 : 1;
}
