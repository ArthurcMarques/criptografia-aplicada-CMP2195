#include "menu_cifras.h"

int main(void)
{
    while (1) {
        int opcao = pedir_numero("Cifras\n1 - Binaria com LFSR (Z2)\n"
                                "2 - Autochave (Z26)\n3 - Transposicao (Z26)\n4 - Cesar\n5 - Vigenere\n"
                                "6 - Afim\n7 - Substituicao\n8 - Hill\n0 - Sair\nCifra: ", 0, 8);
        if (opcao == -1) return 0;

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
            case 4:
                executar_cesar_vigenere(0);
                break;
            case 5:
                executar_cesar_vigenere(1);
                break;
            case 6:
                executar_afim();
                break;
            case 7:
                executar_substituicao();
                break;
            case 8:
                executar_hill();
                break;
            case 0:
                printf("Programa encerrado.\n");
                return 0;
        }
        printf("\n");
    }
}
