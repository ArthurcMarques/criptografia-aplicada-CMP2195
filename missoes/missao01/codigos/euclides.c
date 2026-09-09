#include <stdio.h>

long long euclides(long long a, long long b)
{
    if (a < 0) {
        a = -a;
    }
    if (b < 0) {
        b = -b;
    }

    while (b != 0) {
        long long resto = a % b;
        a = b;
        b = resto;
    }

    return a;
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

    printf("MDC(%lld, %lld) = %lld\n", a, b, euclides(a, b));

    return 0;
}
