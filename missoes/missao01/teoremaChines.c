int inverso_modular(int numero, int modulo);

int teorema_chines_resto(
    int restos[],
    int modulos[],
    int quantidade)
{
    int M = 1;
    int M_linha;
    int inverso;
    int soma = 0;
    int resultado;

    for (int i = 0; i < quantidade; i++)
    {
        M = M * modulos[i];
    }

    for (int i = 0; i < quantidade; i++)
    {
        M_linha = M / modulos[i];

        inverso = inverso_modular(
            aritmetica_modular(M_linha, modulos[i]),
            modulos[i]);

        soma = soma + restos[i] * M_linha * inverso;
    }

    resultado = aritmetica_modular(soma, M);

    return resultado;
}