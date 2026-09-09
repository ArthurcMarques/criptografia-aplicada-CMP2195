#include <stdio.h>

long long *euclides_estendido(long long a, long long b)
{
    // O vetor permanece valido apos o retorno e e sobrescrito a cada chamada.
    static long long resultado[3];
    long long x_atual = 1;
    long long x_proximo = 0;
    long long y_atual = 0;
    long long y_proximo = 1;

    if (a < 0) {
        a = -a;
        x_atual = -1;
    }
    if (b < 0) {
        b = -b;
        y_proximo = -1;
    }

    while (b != 0) {
        long long quociente = a / b;
        long long resto = a % b;
        long long novo_x = x_atual - quociente * x_proximo;
        long long novo_y = y_atual - quociente * y_proximo;

        a = b;
        b = resto;
        // Atualiza os coeficientes da identidade de Bezout.
        x_atual = x_proximo;
        x_proximo = novo_x;
        y_atual = y_proximo;
        y_proximo = novo_y;
    }

    resultado[0] = a;       // MDC
    resultado[1] = x_atual; // Coeficiente x
    resultado[2] = y_atual; // Coeficiente y

    return resultado;
}

int main(void)
{
    long long a;
    long long b;

    printf("Digite dois numeros inteiros: ");

    if (scanf("%lld %lld", &a, &b) != 2) {
        printf("Entrada invalida.\n");
        return 1;
    }

    long long *resultado = euclides_estendido(a, b);

    printf("MDC(%lld, %lld) = %lld\n", a, b, resultado[0]);
    printf("x = %lld, y = %lld\n", resultado[1], resultado[2]);
    printf("(%lld) * (%lld) + (%lld) * (%lld) = %lld\n",
           a, resultado[1], b, resultado[2], resultado[0]);

    return 0;
}
