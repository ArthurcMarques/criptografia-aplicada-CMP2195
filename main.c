#include <stdio.h>
#include "biblioteca.h"

/* ===========================================================================
 *  main.c - bateria de testes + demonstracao didatica da Missao 01
 *
 *  Cada secao mostra PRIMEIRO o algoritmo funcionando passo a passo
 *  (para explicar na apresentacao) e DEPOIS a bateria de testes
 *  automatizados que prova que a implementacao esta correta.
 * ======================================================================== */

static int total = 0, ok = 0;

static void checa(const char *nome, int obtido, int esperado)
{
    total++;
    if (obtido == esperado) {
        ok++;
        printf("  [OK]    %-42s = %d\n", nome, obtido);
    } else {
        printf("  [FALHA] %-42s = %d (esperado %d)\n", nome, obtido, esperado);
    }
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
 *  Demonstracoes passo a passo (so para a apresentacao)
 * ------------------------------------------------------------------------ */

/* Mostra cada divisao do algoritmo de Euclides ate o resto zerar. */
static void demo_euclides(int a, int b)
{
    printf("  Calculando mdc(%d, %d) por divisoes sucessivas:\n", a, b);
    while (b != 0) {
        int q = a / b;
        int r = a % b;
        printf("      %4d = %2d x %4d + %d\n", a, q, b, r);
        a = b;
        b = r;
    }
    printf("  O ultimo resto nao nulo eh o mdc  ->  %d\n", a);
}

/* Mostra a identidade de Bezout com os numeros substituidos.
   Copia a tupla para variaveis locais logo apos a chamada, porque o vetor
   devolvido eh static - a proxima chamada o sobrescreve. */
static void demo_bezout(int a, int b)
{
    int *t = euclides_estendido(a, b);
    int g = t[0], x = t[1], y = t[2];

    printf("  Bezout para (%d, %d):\n", a, b);
    printf("      mdc = %d,  x = %d,  y = %d\n", g, x, y);
    printf("      %d x (%d) + %d x (%d) = %d   <- confere com o mdc\n",
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
    if (resto > 1) {
        printf("%s%d^1", primeiro ? "" : " x ", resto);
    }
    printf("\n");

    /* refaz mostrando cada fator da formula */
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
    if (resto > 1) {
        printf("%s(%d - 1)", primeiro ? "" : " x ", resto);
        phi *= resto - 1;
    }
    printf(" = %d\n", phi);
}

/* Mostra o sistema de congruencias e confere a solucao encontrada. */
static void demo_tcr(int r[], int m[], int q, const char *rotulo)
{
    int *s;
    int resto, modulo;

    printf("  %s\n", rotulo);
    for (int i = 0; i < q; i++) {
        printf("      x = %d (mod %d)\n", r[i], m[i]);
    }

    s = teorema_chines_resto(r, m, q);
    resto  = s[0];
    modulo = s[1];

    if (resto == -1) {
        printf("      -> sistema IMPOSSIVEL (nenhum x satisfaz tudo)\n");
        return;
    }

    printf("      -> x = %d (mod %d)\n", resto, modulo);
    printf("         conferindo: ");
    for (int i = 0; i < q; i++) {
        printf("%d mod %d = %d%s", resto, m[i], resto % m[i],
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
    {
        int *t = euclides_estendido(240, 46);
        int g = t[0], x = t[1], y = t[2];
        checa("euclides_estendido(240,46) -> mdc", g, 2);
        checa("240*x + 46*y", 240 * x + 46 * y, g);
        printf("          -> x = %d, y = %d\n", x, y);

        t = euclides_estendido(7, 26);
        g = t[0]; x = t[1]; y = t[2];
        checa("euclides_estendido(7,26) -> mdc", g, 1);
        checa("7*x + 26*y", 7 * x + 26 * y, 1);
    }

    /* ==================================================================== */
    titulo("MYLLENA - MDC, Aritmetica Modular e Teorema Chines do Resto");
    /* ==================================================================== */

    printf("\nNa aritmetica modular tudo eh reduzido para a faixa [0, m-1].\n");
    printf("Somar, subtrair e multiplicar eh direto. DIVIDIR eh diferente:\n");
    printf("nao existe fracao, entao a/b vira a * (inverso de b).\n\n");
    printf("  (3 / 7) mod 26  ->  inverso de 7 mod 26 eh %d\n",
           inverso_modular(7, 26));
    printf("                  ->  3 x %d = %d  ->  mod 26 = %d\n",
           inverso_modular(7, 26), 3 * inverso_modular(7, 26),
           aritmetica_modular(3, 7, 26, "/"));
    printf("  Se o divisor nao tiver inverso, a divisao NAO existe:\n");
    printf("  (3 / 6) mod 12  ->  mdc(6,12) = %d  ->  retorna %d\n",
           mdc(6, 12), aritmetica_modular(3, 6, 12, "/"));

    printf("\nO TCR junta varias congruencias numa unica resposta.\n\n");
    {
        int r[3] = {2, 3, 2};
        int m[3] = {3, 5, 7};
        demo_tcr(r, m, 3, "Sistema classico (modulos coprimos):");
    }
    printf("\n");
    {
        int r[2] = {3, 5};
        int m[2] = {4, 6};
        demo_tcr(r, m, 2, "Modulos NAO coprimos, mas compativel:");
    }
    printf("\n");
    {
        int r[2] = {1, 2};
        int m[2] = {4, 6};
        demo_tcr(r, m, 2, "Sistema sem solucao (1 e 2 discordam mod 2):");
    }

    subtitulo("testes");
    checa("mdc(48, 18)", mdc(48, 18), 6);
    checa("mdc(0, 5)", mdc(0, 5), 5);
    checa("mdc(13, 17)", mdc(13, 17), 1);

    checa("(14 + 25) mod 12", aritmetica_modular(14, 25, 12, "+"), 3);
    checa("(4 - 9) mod 7", aritmetica_modular(4, 9, 7, "-"), 2);
    checa("(123 * 456) mod 1000", aritmetica_modular(123, 456, 1000, "*"), 88);
    checa("(3 / 7) mod 26  = 3 * 15", aritmetica_modular(3, 7, 26, "/"), 19);
    checa("(2 ^ 10) mod 1000", aritmetica_modular(2, 10, 1000, "^"), 24);
    checa("(3 / 6) mod 12  -> sem inverso", aritmetica_modular(3, 6, 12, "/"), -1);
    checa("operador invalido", aritmetica_modular(3, 6, 12, "%"), -1);
    checa("modulo invalido", aritmetica_modular(3, 6, 0, "+"), -1);

    {
        /* x = 2 (mod 3), x = 3 (mod 5), x = 2 (mod 7)  ->  x = 23 (mod 105) */
        int r[3] = {2, 3, 2};
        int m[3] = {3, 5, 7};
        int *s = teorema_chines_resto(r, m, 3);
        checa("TCR classico: x", s[0], 23);
        checa("TCR classico: M", s[1], 105);
    }
    {
        /* modulos nao coprimos, compativel: x = 3 (mod 4), x = 5 (mod 6) */
        int r[2] = {3, 5};
        int m[2] = {4, 6};
        int *s = teorema_chines_resto(r, m, 2);
        checa("TCR nao coprimo: x", s[0], 11);
        checa("TCR nao coprimo: M (mmc)", s[1], 12);
    }
    {
        /* sistema impossivel: x = 1 (mod 4) e x = 2 (mod 6) -> (-1, -1) */
        int r[2] = {1, 2};
        int m[2] = {4, 6};
        int *s = teorema_chines_resto(r, m, 2);
        checa("TCR impossivel: resto", s[0], -1);
        checa("TCR impossivel: modulo", s[1], -1);
    }

    /* ==================================================================== */
    titulo("DYLAN - Primalidade e Exponenciacao Modular");
    /* ==================================================================== */

    printf("\nPara testar se n eh primo basta procurar divisor ate sqrt(n):\n");
    printf("se n = a x b com a <= b, entao a <= sqrt(n).\n");
    printf("  1000003 tem sqrt ~ 1000  ->  ~1000 divisoes em vez de 1000000\n");
    printf("  eh_primo(1000003) = %d\n", eh_primo(1000003));

    printf("\nA exponenciacao rapida le o expoente em binario: a cada bit\n");
    printf("eleva a base ao quadrado, e so multiplica quando o bit eh 1.\n\n");
    demo_exp_modular(7, 128, 13);
    printf("\n");
    demo_exp_modular(2, 10, 1000);

    subtitulo("testes");
    checa("(2 ^ 10) mod 1000", exp_modular(2, 10, 1000), 24);
    checa("(7 ^ 128) mod 13", exp_modular(7, 128, 13), 3);
    checa("(123456 ^ 0) mod 7", exp_modular(123456, 0, 7), 1);
    checa("(5 ^ 3) mod 1", exp_modular(5, 3, 1), 0);
    checa("expoente negativo", exp_modular(2, -1, 7), -1);
    checa("modulo invalido", exp_modular(2, 10, 0), -1);
    checa("mod grande (123456^5 mod 1000003)", exp_modular(123456, 5, 1000003), 63218);
    checa("eh_primo(1)", eh_primo(1), 0);
    checa("eh_primo(2)", eh_primo(2), 1);
    checa("eh_primo(97)", eh_primo(97), 1);
    checa("eh_primo(91) = 7*13", eh_primo(91), 0);
    checa("eh_primo(1000003)", eh_primo(1000003), 1);
    checa("eh_primo(-3) negativo", eh_primo(-3), 0);

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
    printf("  Sem mdc = 1 nao ha inverso: mdc(6,12) = %d  ->  %d\n",
           mdc(6, 12), inverso_modular(6, 12));

    printf("\nphi(n) conta quantos numeros de 1 a n sao primos com n.\n");
    printf("Fatorando n, cada primo contribui com (p^e - p^(e-1)).\n\n");
    demo_phi(360);
    printf("\n");
    demo_phi(33);
    printf("\n");
    demo_phi(13);
    printf("  (para p primo, phi(p) = p - 1: todos os menores servem)\n");

    subtitulo("testes");
    checa("inverso_modular(7, 26)", inverso_modular(7, 26), 15);
    checa("inverso_modular(3, 11)", inverso_modular(3, 11), 4);
    checa("inverso_modular(6, 12) sem inverso", inverso_modular(6, 12), -1);
    checa("inverso_modular(17, 3120)", inverso_modular(17, 3120), 2753);
    checa("phi_euler(1)", phi_euler(1), 1);
    checa("phi_euler(9)", phi_euler(9), 6);
    checa("phi_euler(13)", phi_euler(13), 12);
    checa("phi_euler(3 * 11)", phi_euler(33), 20);
    checa("phi_euler(360)", phi_euler(360), 96);
    checa("phi_euler(1024) = 2^10", phi_euler(1024), 512);
    checa("phi_euler(1000003) primo", phi_euler(1000003), 1000002);
    checa("phi_euler(0) invalido", phi_euler(0), -1);

    /* ==================================================================== */
    titulo("FECHAMENTO - as 4 partes juntas dentro do RSA");
    /* ==================================================================== */

    {
        int p = 61, q = 53;
        int n = p * q;
        int phi = phi_euler(n);            /* HENRIQUE */
        int e = 17;
        int d = inverso_modular(e, phi);   /* HENRIQUE + ARTHUR */
        int mensagem = 65;
        int cifrado, decifrado;

        printf("\n1) DYLAN escolhe dois primos:  p = %d (primo? %d)  "
               "q = %d (primo? %d)\n", p, eh_primo(p), q, eh_primo(q));
        printf("2) O modulo publico eh n = p x q = %d\n", n);
        printf("3) HENRIQUE calcula phi(n) = %d    "
               "(confere: (p-1)(q-1) = %d)\n", phi, (p - 1) * (q - 1));
        printf("4) Escolhe-se e = %d, com mdc(e, phi) = %d  "
               "(MYLLENA/ARTHUR)\n", e, mdc(e, phi));
        printf("5) HENRIQUE acha a chave privada d = inverso de e mod phi = %d\n", d);
        printf("   conferindo: (e x d) mod phi = %d\n",
               aritmetica_modular(e, d, phi, "*"));

        cifrado   = exp_modular(mensagem, e, n);   /* DYLAN */
        decifrado = exp_modular(cifrado, d, n);    /* DYLAN */

        printf("\n6) Mensagem original .... %d\n", mensagem);
        printf("7) Cifrando  m^e mod n .. %d\n", cifrado);
        printf("8) Decifrando c^d mod n . %d\n", decifrado);
        printf("\n   Todas as 4 partes da equipe foram usadas nesse ciclo.\n\n");

        checa("RSA: phi(3233)", phi, 3120);
        checa("RSA: chave privada d", d, 2753);
        checa("RSA: e*d = 1 (mod phi)", aritmetica_modular(e, d, phi, "*"), 1);
        checa("RSA: mensagem cifrada", cifrado, 2790);
        checa("RSA: decifrado == original", decifrado, mensagem);
    }

    /* ==================================================================== */
    printf("\n============================================================\n");
    printf(" RESULTADO FINAL: %d/%d testes passaram\n", ok, total);
    if (ok == total) {
        printf(" Biblioteca completa e integrada.\n");
    } else {
        printf(" ATENCAO: %d teste(s) falhando.\n", total - ok);
    }
    printf("============================================================\n");

    return ok == total ? 0 : 1;
}
