/* Necessario no MinGW antigo para o printf entender %lld */
#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Segundo_Trabalho_Cripto.h"

/* ===========================================================================
 *  main_segundo_trabalho.c - bateria de testes da Missao 02
 *
 *  Mesmo formato da Missao 01: cada secao mostra PRIMEIRO a cifra passo a
 *  passo (para explicar na apresentacao) e DEPOIS os testes que provam que
 *  ela funciona.
 *
 *  Duas marcacoes diferentes:
 *    [OK] / [FALHA]  -> integracao: as pecas conversando entre si
 *    [PENDENTE]      -> bug conhecido dentro do codigo de uma peca,
 *                       ainda nao corrigido pelo autor. Nao conta como
 *                       falha de integracao.
 *
 *  Para adicionar a sua parte: coloque as funcoes na sua secao do
 *  Segundo_Trabalho_Cripto.h e os testes na sua secao aqui embaixo, usando
 *  checa() para numeros e checa_texto() para textos.
 * ======================================================================== */

#define MENSAGEM          "TRANSFERIR DOCUMENTO PARA SERVIDOR CENTRAL"
#define MENSAGEM_SEM_ESP  "TRANSFERIRDOCUMENTOPARASERVIDORCENTRAL"
#define ALFABETO          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define CHAVE_FIXA        "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm"

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

static void checa_texto(const char *nome, const char *obtido, const char *esperado)
{
    total++;
    if (strcmp(obtido, esperado) == 0) {
        ok++;
        printf("  [OK]    %-44s = \"%s\"\n", nome, obtido);
    } else {
        printf("  [FALHA] %-44s = \"%s\"\n", nome, obtido);
        printf("          %-44s   \"%s\" (esperado)\n", "", esperado);
    }
}

/* Bug ja conhecido no codigo de uma peca: registra sem reprovar. */
static void pendente(const char *nome, long long obtido, long long correto)
{
    pend++;
    printf("  [PEND.] %-44s = %lld (correto %lld)\n", nome, obtido, correto);
}

static void pendente_texto(const char *nome, const char *obtido, const char *correto)
{
    pend++;
    printf("  [PEND.] %-44s = \"%s\"\n", nome, obtido);
    printf("          %-44s   \"%s\" (correto)\n", "", correto);
}

/* Bug que nao da para medir com um numero: so descreve. */
static void pendente_nota(const char *descricao)
{
    pend++;
    printf("  [PEND.] %s\n", descricao);
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

/* encriptAte52Posicoes / decriptAte52Posicoes preenchem exatamente 52
   posicoes, sem '\0' no fim; o vetor aqui tem uma a mais para fechar a string. */
static void fecha52(char vetor[53])
{
    vetor[52] = '\0';
}

/* ---------------------------------------------------------------------------
 *  Demonstracoes passo a passo
 * ------------------------------------------------------------------------ */

/* Mostra a conta letra a letra e depois roda a afim() do Henrique. */
static void demo_afim(const char *texto, int a, int b)
{
    char vetor[500];

    printf("  Chave: a = %d, b = %d    (inverso de %d mod 26 = %d)\n",
           a, b, a, inverso_modular(a, 26));
    printf("  Cifrando as primeiras letras com E(x) = (%d*x + %d) mod 26:\n", a, b);
    for (int i = 0; i < 5 && texto[i] != '\0'; i++) {
        int x = texto[i] - 'A';
        int y = (a * x + b) % 26;
        printf("      %c = %2d  ->  (%d*%2d + %d) mod 26 = %2d  ->  %c\n",
               texto[i], x, a, x, b, y, 'A' + y);
    }

    printf("\n  Saida da afim(\"%s\", %d, %d):", texto, a, b);
    strcpy(vetor, texto);
    afim(vetor, a, b);
    printf("\n");
}

/* Mostra a tabela alfabeto -> chave e cifra um bloco de 52 letras. */
static void demo_substituicao(const char *bloco, const char *chave)
{
    char alfabeto[] = ALFABETO;
    char vetorChave[53];
    char entrada[500];
    char cifrado[53], decifrado[53];

    strcpy(vetorChave, chave);
    strcpy(entrada, bloco);

    printf("  alfabeto: %.26s\n", alfabeto);
    printf("            ||||||||||||||||||||||||||\n");
    printf("  chave:    %.26s\n", vetorChave);
    printf("  (e o mesmo para as minusculas: %.26s -> %.26s)\n",
           alfabeto + 26, vetorChave + 26);

    encriptAte52Posicoes(entrada, alfabeto, vetorChave, 52, cifrado);
    fecha52(cifrado);
    decriptAte52Posicoes(cifrado, alfabeto, vetorChave, 52, decifrado);
    fecha52(decifrado);

    printf("\n      bloco ..... %s\n", entrada);
    printf("      cifrado ... %s\n", cifrado);
    printf("      decifrado . %s\n", decifrado);
}

/* ---------------------------------------------------------------------------
 *  main
 * ------------------------------------------------------------------------ */

int main(void)
{
    printf("############################################################\n");
    printf("#  MISSAO 02 - CRIPTOGRAFIA CLASSICA                        #\n");
    printf("#  Arthur . Myllena . Dylan . Henrique                      #\n");
    printf("############################################################\n");
    printf("\nMensagem interceptada: \"%s\"\n", MENSAGEM);

    /* ==================================================================== */
    titulo("HENRIQUE - Cifra Afim");
    /* ==================================================================== */

    printf("\nCada letra vira um numero (A=0 ... Z=25), passa por\n");
    printf("E(x) = (a*x + b) mod 26 e volta a ser letra. Para decifrar\n");
    printf("usa-se D(y) = a^-1 * (y - b) mod 26.\n\n");
    demo_afim(MENSAGEM_SEM_ESP, 5, 8);

    {
        int validos = 0;
        for (int a = 1; a < 26; a++) {
            validos += eh_coprimo_26(a);
        }
        printf("\nSo serve 'a' coprimo com 26: sao %d valores. Com 26 valores\n", validos);
        printf("de b, sao so %d chaves: forca bruta testa todas num instante.\n",
               validos * 26);
    }

    subtitulo("testes");
    {
        char t[500];

        strcpy(t, "AFFINECIPHER");
        checa("encript retorna 1 (a=5, b=8)", encript(t, 5, 8), 1);
        checa_texto("AFFINECIPHER (a=5, b=8)", t, "IHHWVCSWFRCP");
        checa("decript retorna 1 (a=5, b=8)", decript(t, 5, 8), 1);
        checa_texto("decriptando de volta", t, "AFFINECIPHER");

        strcpy(t, MENSAGEM_SEM_ESP);
        encript(t, 5, 8);
        checa_texto("mensagem da missao cifrada", t,
                    "ZPIVUHCPWPXASEQCVZAFIPIUCPJWXAPSCVZPIL");
        decript(t, 5, 8);
        checa_texto("mensagem da missao decifrada", t, MENSAGEM_SEM_ESP);

        strcpy(t, "ABCXYZ");
        encript(t, 1, 3);
        checa_texto("a=1, b=3 eh a Cifra de Cesar", t, "DEFABC");

        strcpy(t, "affinecipher");
        para_maiusculo(t);
        checa_texto("para_maiusculo", t, "AFFINECIPHER");

        checa("mdc(48, 18)", mdc(48, 18), 6);
        checa("inverso_modular(5, 26)", inverso_modular(5, 26), 21);
        checa("inverso_modular(13, 26) sem inverso", inverso_modular(13, 26), -1);
        checa("eh_coprimo_26(5)", eh_coprimo_26(5), 1);
        checa("eh_coprimo_26(13)", eh_coprimo_26(13), 0);

        printf("\n  (a linha impressa abaixo vem de dentro da biblioteca)\n");
        strcpy(t, "ABC");
        checa("encript com a=13 -> 0", encript(t, 13, 8), 0);
        checa_texto("texto fica intacto quando a chave eh invalida", t, "ABC");
    }

    /* ==================================================================== */
    titulo("HENRIQUE - Cifra de Substituicao");
    /* ==================================================================== */

    printf("\nA chave eh uma permutacao das 52 letras: cada letra do\n");
    printf("alfabeto troca pela letra na mesma posicao da chave. O texto\n");
    printf("vai em blocos de 52; aqui a mensagem foi completada com 'A',\n");
    printf("como faz a substituicaoTamanhoIndeterminado.\n\n");
    demo_substituicao(MENSAGEM_SEM_ESP "AAAAAAAAAAAAAA", CHAVE_FIXA);

    printf("\nSao 52! = %.2e chaves possiveis: forca bruta eh inviavel.\n",
           tgamma(53.0));
    printf("Mas cada letra sempre vira a mesma letra, entao a frequencia\n");
    printf("das letras do portugues continua aparecendo no texto cifrado.\n");

    subtitulo("testes");
    {
        char alfabeto[] = ALFABETO;
        char vetorChave[53];
        char entrada[500];
        char cifrado[53], decifrado[53];

        strcpy(vetorChave, CHAVE_FIXA);

        strcpy(entrada, ALFABETO);
        checa("encriptAte52Posicoes retorna 1",
              encriptAte52Posicoes(entrada, alfabeto, vetorChave, 52, cifrado), 1);
        fecha52(cifrado);
        checa_texto("cifrar o alfabeto devolve a chave", cifrado, CHAVE_FIXA);

        strcpy(entrada, MENSAGEM_SEM_ESP "AAAAAAAAAAAAAA");
        encriptAte52Posicoes(entrada, alfabeto, vetorChave, 52, cifrado);
        fecha52(cifrado);
        checa_texto("bloco da mensagem cifrado", cifrado,
                    "ZKQFLYTKOKRGEXDTFZGHQKQLTKCORGKETFZKQSQQQQQQQQQQQQQQ");
        checa("decriptAte52Posicoes retorna 1",
              decriptAte52Posicoes(cifrado, alfabeto, vetorChave, 52, decifrado), 1);
        fecha52(decifrado);
        checa_texto("bloco da mensagem decifrado", decifrado, entrada);

        /* Qualquer permutacao serve de chave: o alfabeto de tras para frente. */
        strcpy(vetorChave, "zyxwvutsrqponmlkjihgfedcbaZYXWVUTSRQPONMLKJIHGFEDCBA");
        encriptAte52Posicoes(entrada, alfabeto, vetorChave, 52, cifrado);
        decriptAte52Posicoes(cifrado, alfabeto, vetorChave, 52, decifrado);
        fecha52(decifrado);
        checa_texto("ida e volta com outra chave", decifrado, entrada);
    }

    /* ==================================================================== */
    titulo("ARTHUR - Transposicao e Cifras de Fluxo");
    /* ==================================================================== */

    printf("\n  (aguardando integracao)\n");

    /* ==================================================================== */
    titulo("MYLLENA - Cifra de Hill");
    /* ==================================================================== */

    printf("\n  (aguardando integracao)\n");

    /* ==================================================================== */
    titulo("DYLAN - (a definir)");
    /* ==================================================================== */

    printf("\n  (aguardando integracao)\n");

    /* ==================================================================== */
    titulo("PENDENCIAS - bugs conhecidos, cada um no codigo do autor");
    /* ==================================================================== */

    printf("\nNao sao falhas de integracao: as pecas se encaixam. Sao bugs\n");
    printf("dentro de uma peca, que cabe ao autor corrigir.\n\n");

    printf("  HENRIQUE - Cifra Afim:\n");
    {
        char t[500];

        strcpy(t, MENSAGEM);
        encript(t, 5, 8);
        decript(t, 5, 8);
        pendente_texto("espacos: cifrar e decifrar a mensagem", t, MENSAGEM);

        strcpy(t, "A");
        encript(t, 5, -18);
        pendente_texto("b negativo: encript(\"A\", 5, -18)", t, "I");

        strcpy(t, "ABC");
        pendente("decript com a=13 deveria retornar 0", decript(t, 13, 8), 0);
    }

    printf("\n  HENRIQUE - Cifra de Substituicao:\n");
    {
        char alfabeto[] = ALFABETO;
        char vetorChave[53];
        char entrada[500];
        char cifrado[53];

        strcpy(vetorChave, CHAVE_FIXA);
        strcpy(entrada, ALFABETO);
        entrada[0] = ' ';
        pendente("caractere fora do alfabeto deveria retornar 0",
                 encriptAte52Posicoes(entrada, alfabeto, vetorChave, 52, cifrado), 0);
    }
    pendente_nota("substituicaoTamanhoIndeterminado (linha 155) cifra vetorValor,\n"
                  "          o alfabeto, em vez de vetorEntrada");
    pendente_nota("chave sorteada com rand() % 52 (linha 151) pode repetir letras,\n"
                  "          e ai duas letras cifram igual e nao da para decifrar");
    pendente_nota("a decriptacao (linha 164) le vetorEntrada, o texto original, e o\n"
                  "          que se imprime como VetorDecript eh a propria entrada");
    pendente_nota("com mais de 52 letras, vetorSaida[52] (linha 131) eh sobrescrito\n"
                  "          a cada bloco e o printf le alem do fim do vetor");

    /* ==================================================================== */
    printf("\n============================================================\n");
    printf(" INTEGRACAO : %d/%d testes passaram\n", ok, total);
    printf(" PENDENCIAS : %d bugs conhecidos, listados acima\n", pend);
    if (ok == total) {
        printf("\n Todas as partes integradas estao funcionando.\n");
    } else {
        printf("\n ATENCAO: %d teste(s) de integracao falhando.\n", total - ok);
    }
    printf("============================================================\n");

    return ok == total ? 0 : 1;
}
