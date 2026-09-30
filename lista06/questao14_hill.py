"""Questão 14: cifra de Hill com blocos em vetores coluna.
Execute com Python 3.8 ou superior; não requer bibliotecas externas.
A=0, ..., Z=25. Espaços e pontuação são removidos; acentos não são aceitos.
Blocos incompletos recebem X; a decifração mantém esse preenchimento.
Informe a operação, a mensagem e as linhas da matriz pelo terminal (ordem 1 a 4).
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


def menor(matriz, i, j):
    # Apaga a linha i e a coluna j; os índices do Python começam em zero.
    return [linha[:j] + linha[j+1:]
            for indice, linha in enumerate(matriz) if indice != i]


def determinante(matriz):
    n = len(matriz)
    if n == 1:
        return matriz[0][0]
    if n == 2:
        return matriz[0][0]*matriz[1][1] - matriz[0][1]*matriz[1][0]
    # Laplace pela primeira linha: elemento vezes seu cofator.
    resultado = 0
    for j in range(n):
        cofator = (-1)**j * determinante(menor(matriz, 0, j))
        resultado += matriz[0][j] * cofator
    return resultado


def inversa_modular(matriz, modulo=26):
    n = len(matriz)
    if n == 0 or any(len(linha) != n for linha in matriz):
        raise ValueError("A matriz deve ser quadrada e não vazia")
    if any(not isinstance(x, int) for linha in matriz for x in linha):
        raise ValueError("A matriz deve conter números inteiros")
    d = determinante(matriz)
    if gcd(d, modulo) != 1:
        raise ValueError("O determinante não possui inverso modular")
    inverso_d = pow(d, -1, modulo)
    if n == 1:
        return [[inverso_d]]

    cofatores = []
    for i in range(n):
        linha = []
        for j in range(n):
            linha.append((-1)**(i+j) * determinante(menor(matriz, i, j)))
        cofatores.append(linha)

    # Transpõe a matriz de cofatores para obter a adjunta.
    adjunta = [[cofatores[j][i] for j in range(n)] for i in range(n)]
    return [[inverso_d*x % modulo for x in linha] for linha in adjunta]


def aplicar_blocos(valores, matriz):
    n = len(matriz)
    if len(valores) % n != 0:
        raise ValueError("O tamanho do texto cifrado deve ser múltiplo da ordem da matriz")
    resultado = []
    for inicio in range(0, len(valores), n):
        bloco = valores[inicio:inicio+n]
        for linha in matriz:
            soma = 0
            for j in range(n):
                soma += linha[j] * bloco[j]
            resultado.append(soma % 26)
    return resultado


def hill_cifrar(texto, matriz):
    inversa_modular(matriz)  # Verifica se a chave admite decifração.
    valores = numeros(texto)
    while len(valores) % len(matriz) != 0:
        valores.append(23)  # X = 23
    return letras(aplicar_blocos(valores, matriz))


def hill_decifrar(texto, matriz):
    inversa = inversa_modular(matriz)
    return letras(aplicar_blocos(numeros(texto), inversa))


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


def ler_matriz():
    # Limite adequado aos exemplos da lista e à expansão recursiva de Laplace.
    n = ler_inteiro("Ordem da matriz (1 a 4): ", 1, 4)
    while True:
        matriz = []
        for i in range(n):
            while True:
                entrada = input(f"Linha {i+1}: digite {n} inteiros separados por espaços: ")
                try:
                    linha = [int(x) for x in entrada.split()]
                    if len(linha) != n:
                        raise ValueError()
                    matriz.append(linha)
                    break
                except ValueError:
                    print(f"Entrada inválida: informe exatamente {n} inteiros.")
        try:
            inversa_modular(matriz)
            return matriz
        except ValueError as erro:
            print("Erro:", erro)
            print("Digite novamente as linhas da matriz.")


def main():
    print("CIFRA DE HILL - Questão 14")
    operacao = ler_operacao()
    if operacao == 0:
        return
    mensagem = ler_mensagem()
    matriz = ler_matriz()
    if operacao == 2:
        while len(mensagem) % len(matriz) != 0:
            print("O texto cifrado deve ter um número de letras múltiplo da ordem da matriz.")
            mensagem = ler_mensagem()
    print("Determinante:", determinante(matriz))
    print("Matriz inversa módulo 26:")
    for linha in inversa_modular(matriz):
        print(linha)
    if operacao == 1:
        quantidade = -len(mensagem) % len(matriz)
        if quantidade:
            print(f"Preenchimento: {quantidade} letra(s) X adicionada(s) ao final.")
        print("Cifrada:", hill_cifrar(mensagem, matriz))
    else:
        print("Decifrada:", hill_decifrar(mensagem, matriz))
        print("Eventuais letras X de preenchimento são mantidas.")


if __name__ == "__main__":
    try:
        main()
    except (EOFError, KeyboardInterrupt):
        print("\nPrograma encerrado.")
