#include "cifras.h"

int main(void)
{
    char linha[MAX_TEXTO + 2];
    while (1) {
        printf("Cifras\n1 - Binaria com LFSR (Z2)\n"
               "2 - Autochave (Z26)\n3 - Transposicao (Z26)\n0 - Sair\nCifra: ");
        int leitura = ler_linha(linha);
        if (leitura == 0) {
            printf("Entrada encerrada.\n");
            return 1;
        }
        int opcao;
        if (leitura != 1 || !numero_valido(linha, 0, 3, &opcao)) {
            printf("Cifra invalida. Tente novamente.\n");
            continue;
        }

        switch (opcao) {
            case 1:
                executar_lfsr();
                break;
            case 2:
                executar_autochave();
                break;
            case 3:
                executar_transposicao();
                break;
            case 0:
                printf("Programa encerrado.\n");
                return 0;
        }
        printf("\n");
    }
}
