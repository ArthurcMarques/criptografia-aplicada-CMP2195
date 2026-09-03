#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

long long mdc(long long a, long long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

long long* euclides_estendido(long long a, long long b)
{
    static long long r[3];
    long long x0 = 1, x1 = 0, y0 = 0, y1 = 1;
    while (b != 0) {
        long long q = a / b;
        long long t;
        t = a - q * b;   a = b;   b = t;
        t = x0 - q * x1; x0 = x1; x1 = t;
        t = y0 - q * y1; y0 = y1; y1 = t;
    }
    r[0] = a;  //mdc
    r[1] = x0; //fator 1
    r[2] = y0; //fator 2
    return r;
}

int inverso_modular(int numero, int modulo){
    
    if(mdc(numero,modulo) != 1){
        printf("O núemero e o módulo não são primos entre si\n");
        printf("Portanto, numero não tem inverso modular\n");
    }
    else{
        
    }
    return 0;
}

#endif /* BIBLIOTECA_H */
