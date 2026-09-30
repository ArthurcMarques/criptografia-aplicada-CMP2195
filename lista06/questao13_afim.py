"""Questão 13: cifra afim. Execute com Python 3.8 ou superior.
A=0, ..., Z=25. Espaços e pontuação são removidos; acentos não são aceitos.
Informe a operação, a mensagem e os valores de a e b pelo terminal.
"""

from math import gcd

ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"


def numeros(texto):
    texto = texto.upper()
    if any(c.isalpha() and c not in ABC for c in texto):
        raise ValueError("Use somente letras A-Z, sem acentos")
    return [ABC.index(c) for c in texto if c in ABC]


def letras(valores):
    return "".join(ABC[x % 26] for x in valores)


def afim_cifrar(texto, a, b):
    if gcd(a, 26) != 1:
        raise ValueError("a deve ser coprimo com 26")
    return letras([(a*x + b) % 26 for x in numeros(texto)])


def afim_decifrar(texto, a, b):
    if gcd(a, 26) != 1:
        raise ValueError("a deve ser coprimo com 26")
    inverso = pow(a, -1, 26)
    return letras([inverso*(y-b) % 26 for y in numeros(texto)])


def ler_inteiro(pergunta, minimo=None, maximo=None):
    while True:
        try:
            valor = int(input(pergunta))
            if minimo is not None and valor < minimo:
                raise ValueError()
            if maximo is not None and valor > maximo:
                raise ValueError()
            return valor
        except ValueError:
            print("Entrada inválida. Digite um inteiro dentro do intervalo informado.")


def ler_operacao():
    print("1 - Cifrar")
    print("2 - Decifrar")
    print("0 - Sair")
    return ler_inteiro("Escolha: ", 0, 2)


def ler_mensagem():
    while True:
        texto = input("Digite a mensagem (A-Z, sem acentos): ")
        try:
            valores = numeros(texto)
            if not valores:
                raise ValueError("Digite pelo menos uma letra A-Z")
            return letras(valores)
        except ValueError as erro:
            print("Erro:", erro)


def main():
    print("CIFRA AFIM - Questão 13")
    operacao = ler_operacao()
    if operacao == 0:
        return
    mensagem = ler_mensagem()
    while True:
        a = ler_inteiro("Digite a (inteiro coprimo com 26): ")
        if gcd(a, 26) == 1:
            break
        print("Chave inválida: a deve ser coprimo com 26.")
    b = ler_inteiro("Digite b (inteiro): ")
    print("Chave módulo 26:", (a % 26, b % 26))
    print("Inverso de a módulo 26:", pow(a, -1, 26))
    if operacao == 1:
        print("Cifrada:", afim_cifrar(mensagem, a, b))
    else:
        print("Decifrada:", afim_decifrar(mensagem, a, b))


if __name__ == "__main__":
    try:
        main()
    except (EOFError, KeyboardInterrupt):
        print("\nPrograma encerrado.")
