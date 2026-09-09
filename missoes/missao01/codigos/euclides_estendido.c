#include <stdio.h>

long long euclides_estendido(long long a, long long b, long long *x, long long *y)
{
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

    *x = x_atual;
    *y = y_atual;
    return a;
}

int main(void)
{
    long long a;
    long long b;
    long long x;
    long long y;

    printf("Digite dois numeros inteiros: ");

    if (scanf("%lld %lld", &a, &b) != 2) {
        printf("Entrada invalida.\n");
        return 1;
    }

    long long mdc = euclides_estendido(a, b, &x, &y);

    printf("MDC(%lld, %lld) = %lld\n", a, b, mdc);
    printf("x = %lld, y = %lld\n", x, y);
    printf("(%lld) * (%lld) + (%lld) * (%lld) = %lld\n", a, x, b, y, mdc);

    return 0;
}
