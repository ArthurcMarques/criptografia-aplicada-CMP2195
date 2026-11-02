"""Menu simples: DES, RC5 e AES implementados em cifras.py."""
from pathlib import Path
from cifras import cifrar_texto, decifrar_texto, cifrar_arquivo, decifrar_arquivo


def selecionar():
    print("\n1 - DES (16 caracteres hexadecimais)")
    print("2 - RC5 (32 caracteres hexadecimais)")
    print("3 - AES-128 (32 caracteres hexadecimais)")
    opcao = input("Algoritmo: ").strip()
    if opcao not in ("1", "2", "3"):
        raise ValueError("Algoritmo invalido.")
    return {"1": "DES", "2": "RC5", "3": "AES"}[opcao]


def chave():
    return input("Chave em hexadecimal: ").strip()


def caminho(mensagem):
    return input(mensagem).strip().strip('"')


def mostrar_arquivo(caminho_arquivo, titulo, hexadecimal=False):
    dados = Path(caminho_arquivo).read_bytes()
    print(f"\n--- {titulo} ({len(dados)} bytes) ---")
    if not dados:
        print("(arquivo vazio)")
        return
    if not hexadecimal:
        try:
            texto = dados.decode("utf-8")
            if all(c.isprintable() or c in "\n\r\t" for c in texto):
                print(texto[:500])
                if len(texto) > 500:
                    print("... (previa dos primeiros 500 caracteres)")
                return
        except UnicodeDecodeError:
            pass
    print("Conteudo em hexadecimal:")
    print(dados[:128].hex(" "))
    if len(dados) > 128:
        print("... (previa dos primeiros 128 bytes)")


def main():
    print("Projeto didatico: nao use para proteger informacoes reais.")
    while True:
        print("\n=== MISSAO 3: DES, RC5 E AES ===")
        print("1 - Cifrar texto")
        print("2 - Decifrar texto")
        print("3 - Cifrar arquivo")
        print("4 - Decifrar arquivo")
        print("5 - Experimento AES com arquivo")
        print("0 - Sair")
        opcao = input("Escolha: ").strip()
        if opcao == "0":
            return
        if opcao not in ("1", "2", "3", "4", "5"):
            print("Opcao invalida.")
            continue
        try:
            if opcao == "5":
                entrada = Path(caminho("Arquivo original: "))
                segredo = chave()
                cifrado = entrada.with_name(entrada.name + ".aes")
                recuperado = entrada.with_name(entrada.name + ".recuperado")
                cifrar_arquivo(str(entrada), str(cifrado), segredo, "AES")
                decifrar_arquivo(str(cifrado), str(recuperado), segredo)
                mostrar_arquivo(entrada, "Arquivo original")
                mostrar_arquivo(cifrado, "Arquivo cifrado (inclui cabecalho)", hexadecimal=True)
                mostrar_arquivo(recuperado, "Arquivo recuperado")
                print("Cifrado:", cifrado)
                print("Recuperado:", recuperado)
                print("Arquivos identicos:", entrada.read_bytes() == recuperado.read_bytes())
                continue
            algoritmo = selecionar() if opcao in ("1", "3") else None
            if opcao == "1":
                texto = input("Texto original: ")
                print("Texto cifrado (Base64):", cifrar_texto(texto, chave(), algoritmo))
            elif opcao == "2":
                texto = input("Texto cifrado (Base64): ")
                print("Original:", decifrar_texto(texto, chave()))
            else:
                entrada = caminho("Arquivo de entrada: ")
                padrao = entrada + ".m3" if opcao == "3" else entrada + ".recuperado"
                saida = caminho(f"Arquivo de saida [{padrao}]: ") or padrao
                if opcao == "3":
                    cifrar_arquivo(entrada, saida, chave(), algoritmo)
                    mostrar_arquivo(entrada, "Arquivo original")
                    mostrar_arquivo(saida, "Arquivo cifrado (inclui cabecalho)", hexadecimal=True)
                else:
                    decifrar_arquivo(entrada, saida, chave())
                    mostrar_arquivo(entrada, "Arquivo cifrado (inclui cabecalho)", hexadecimal=True)
                    mostrar_arquivo(saida, "Arquivo recuperado")
                print("Arquivo salvo em:", saida)
        except (ValueError, OSError, UnicodeError) as erro:
            print("Erro:", erro)


if __name__ == "__main__":
    main()
