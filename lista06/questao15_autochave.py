"""Questão 15: cifra de fluxo autochave baseada no texto plano anterior.
A=0, ..., Z=25. Espaços e pontuação são removidos; acentos não são aceitos.
Informe a operação, a mensagem e a semente pelo terminal.
"""

ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"


def numeros(texto):
    texto = texto.upper()
    if any(c.isalpha() and c not in ABC for c in texto):
        raise ValueError("Use somente letras A-Z, sem acentos")
    return [ABC.index(c) for c in texto if c in ABC]


def letras(valores):
    return "".join(ABC[x % 26] for x in valores)


def autochave_cifrar(texto, semente):
    if not 0 <= semente < 26:
        raise ValueError("A semente deve estar entre 0 e 25")
    plano = numeros(texto)
    chave = [semente] + plano[:-1]
    return letras([(x+k) % 26 for x, k in zip(plano, chave)])


def autochave_decifrar(texto, semente):
    if not 0 <= semente < 26:
        raise ValueError("A semente deve estar entre 0 e 25")
    plano = []
    anterior = semente
    for y in numeros(texto):
        x = (y-anterior) % 26
        plano.append(x)
        anterior = x  # A próxima posição usa o texto plano recuperado.
    return letras(plano)


def gerar_chave(texto, semente):
    if not 0 <= semente < 26:
        raise ValueError("A semente deve estar entre 0 e 25")
    plano = numeros(texto)
    if not plano:
        return []
    return [semente] + plano[:-1]


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
    print("CIFRA DE FLUXO AUTOCHAVE - Questão 15")
    operacao = ler_operacao()
    if operacao == 0:
        return
    mensagem = ler_mensagem()
    semente = ler_inteiro("Digite a semente (0 a 25): ", 0, 25)
    if operacao == 1:
        chave = gerar_chave(mensagem, semente)
        resultado = autochave_cifrar(mensagem, semente)
    else:
        resultado = autochave_decifrar(mensagem, semente)
        # Na decifração, a chave é reconstruída a partir do plano recuperado.
        chave = gerar_chave(resultado, semente)
    print("Chave numérica:", chave)
    print("Chave em letras:", letras(chave))
    print("Cifrada:" if operacao == 1 else "Decifrada:", resultado)


if __name__ == "__main__":
    try:
        main()
    except (EOFError, KeyboardInterrupt):
        print("\nPrograma encerrado.")
