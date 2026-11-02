"""Missao 3: DES, RC5 e AES implementados manualmente em Python.
AES-128, DES e RC5-32/12/16: cifragem/decifragem de blocos.
Textos e arquivos sao divididos em blocos independentes, com zeros no ultimo bloco.
Projeto educacional; nao recomendado para dados sensiveis.
"""
import base64
from pathlib import Path

# Utilitarios para o DES (permutacoes e XOR em sequencias de bits).
def perm(bits, table):
    return ''.join(bits[i - 1] for i in table)


def xor(a, b):
    return ''.join(str(int(x) ^ int(y)) for x, y in zip(a, b))


def rot(s, n):
    return s[n:] + s[:n]


IP=[58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7]
FP=[40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25]
PC1=[57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4]
PC2=[14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32]
SHIFTS=[1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1]
E=[32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1]
P=[16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25]
SBOX=[
[[14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7],[0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8],[4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0],[15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13]],
[[15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10],[3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5],[0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15],[13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9]],
[[10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8],[13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1],[13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7],[1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12]],
[[7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15],[13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9],[10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4],[3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14]],
[[2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9],[14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6],[4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14],[11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3]],
[[12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11],[10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8],[9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6],[4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13]],
[[4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1],[13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6],[1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2],[6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12]],
[[13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7],[1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2],[7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8],[2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11]]]

def tobits(b): return ''.join(f'{x:08b}' for x in b)
def frombits(b): return bytes(int(b[i:i+8],2) for i in range(0,len(b),8))
def hx(b): return f'{int(b,2):0{len(b)//4}X}'
def des_keys(k):
    if len(k) != 8:
        raise ValueError("A chave DES precisa ter exatamente 8 bytes.")
    b=perm(tobits(k),PC1); c,d=b[:28],b[28:]; out=[]
    for sh in SHIFTS:
        c,d=rot(c,sh),rot(d,sh)
        out.append(perm(c+d,PC2))
    return out

def des(block,key,decrypt=False,verbose=False):
    if len(block) != 8:
        raise ValueError("O bloco DES precisa ter exatamente 8 bytes.")
    ks=des_keys(key)
    if decrypt: ks=ks[::-1]
    b=perm(tobits(block),IP); l,r=b[:32],b[32:]
    if verbose: print('M_HEX',block.hex().upper(),'K_HEX',key.hex().upper());print('IP',hx(b),'L0',hx(l),'R0',hx(r));print('K1',hx(ks[0]))
    for i,k in enumerate(ks):
        expanded=perm(r,E); x=xor(expanded,k)
        pieces=[x[j*6:(j+1)*6] for j in range(8)]
        subs=''.join(f'{SBOX[j][int(t[0]+t[5],2)][int(t[1:5],2)]:04b}' for j,t in enumerate(pieces))
        fout=perm(subs,P)
        l,r=r,xor(l,fout)
        if verbose and i==0: print('E(R0)',hx(expanded),'XOR K1',hx(x),'Sboxes',hx(subs),'P',hx(fout),'L1',hx(l),'R1',hx(r),'L1R1',hx(l+r))
    return frombits(perm(r+l,FP))


MASCARA = 0xFFFFFFFF


def _esquerda(x, n):
    n &= 31
    return ((x << n) | (x >> (32 - n))) & MASCARA


def _direita(x, n):
    n &= 31
    return ((x >> n) | (x << (32 - n))) & MASCARA


class RC5:
    def __init__(self, chave: bytes, rodadas=12):
        if not chave or len(chave) > 255:
            raise ValueError("A chave deve ter entre 1 e 255 bytes.")
        if type(rodadas) is not int or not 0 <= rodadas <= 255:
            raise ValueError("O numero de rodadas deve ser inteiro, entre 0 e 255.")
        self.rodadas = rodadas
        u, c = 4, max(1, (len(chave) + 3) // 4)
        L = [0] * c
        for i in range(len(chave) - 1, -1, -1):
            L[i // u] = ((L[i // u] << 8) + chave[i]) & MASCARA
        t = 2 * (rodadas + 1)
        S = [0] * t
        S[0] = 0xB7E15163
        for i in range(1, t):
            S[i] = (S[i - 1] + 0x9E3779B9) & MASCARA
        a = b = i = j = 0
        for _ in range(3 * max(t, c)):
            a = S[i] = _esquerda((S[i] + a + b) & MASCARA, 3)
            b = L[j] = _esquerda((L[j] + a + b) & MASCARA, a + b)
            i = (i + 1) % t
            j = (j + 1) % c
        self.S = S

    def cifrar_bloco(self, bloco: bytes) -> bytes:
        if len(bloco) != 8:
            raise ValueError("Bloco precisa ter 8 bytes.")
        a = int.from_bytes(bloco[:4], "little")
        b = int.from_bytes(bloco[4:], "little")
        a = (a + self.S[0]) & MASCARA
        b = (b + self.S[1]) & MASCARA
        for i in range(1, self.rodadas + 1):
            a = (_esquerda(a ^ b, b) + self.S[2 * i]) & MASCARA
            b = (_esquerda(b ^ a, a) + self.S[2 * i + 1]) & MASCARA
        return a.to_bytes(4, "little") + b.to_bytes(4, "little")

    def decifrar_bloco(self, bloco: bytes) -> bytes:
        if len(bloco) != 8:
            raise ValueError("Bloco precisa ter 8 bytes.")
        a = int.from_bytes(bloco[:4], "little")
        b = int.from_bytes(bloco[4:], "little")
        for i in range(self.rodadas, 0, -1):
            b = _direita((b - self.S[2 * i + 1]) & MASCARA, a) ^ a
            a = _direita((a - self.S[2 * i]) & MASCARA, b) ^ b
        a = (a - self.S[0]) & MASCARA
        b = (b - self.S[1]) & MASCARA
        return a.to_bytes(4, "little") + b.to_bytes(4, "little")




# AES-128 (FIPS 197): as funcoes de campo finito, S-box, rodadas e
# expansao de chaves sao implementadas aqui, sem bibliotecas de cifra.
def _gf_mul(a, b):
    resultado = 0
    for _ in range(8):
        if b & 1:
            resultado ^= a
        a = ((a << 1) ^ (0x1B if a & 0x80 else 0)) & 0xFF
        b >>= 1
    return resultado


def _sbox_valor(x):
    # Inverso em GF(2^8), seguido da transformacao afim AES.
    inv = 1
    if x:
        # x^254 e o inverso multiplicativo de x no campo.
        base, expoente = x, 254
        while expoente:
            if expoente & 1:
                inv = _gf_mul(inv, base)
            base = _gf_mul(base, base)
            expoente >>= 1
    else:
        inv = 0
    y = inv
    for deslocamento in (1, 2, 3, 4):
        y ^= ((inv << deslocamento) | (inv >> (8 - deslocamento))) & 255
    return y ^ 0x63


AES_SBOX = tuple(_sbox_valor(i) for i in range(256))
AES_INV_SBOX = [0] * 256
for _idx, _valor in enumerate(AES_SBOX):
    AES_INV_SBOX[_valor] = _idx


def _aes_shiftrows(state, inverso=False):
    # Estado armazenado por colunas: indice = 4*coluna + linha.
    original = state[:]
    for linha in range(4):
        for coluna in range(4):
            origem = (coluna - linha if inverso else coluna + linha) % 4
            state[4 * coluna + linha] = original[4 * origem + linha]


def _aes_mixcolumns(state, inverso=False):
    matriz = ((14, 11, 13, 9), (9, 14, 11, 13),
              (13, 9, 14, 11), (11, 13, 9, 14)) if inverso else (
              (2, 3, 1, 1), (1, 2, 3, 1),
              (1, 1, 2, 3), (3, 1, 1, 2))
    for coluna in range(4):
        valores = state[coluna * 4:coluna * 4 + 4]
        for linha in range(4):
            state[coluna * 4 + linha] = (
                _gf_mul(matriz[linha][0], valores[0]) ^
                _gf_mul(matriz[linha][1], valores[1]) ^
                _gf_mul(matriz[linha][2], valores[2]) ^
                _gf_mul(matriz[linha][3], valores[3]))


class AES128:
    """AES-128 completo para blocos de exatamente 16 bytes."""
    def __init__(self, chave: bytes):
        if len(chave) != 16:
            raise ValueError('AES-128 exige uma chave de exatamente 16 bytes.')
        # 44 palavras de 4 bytes = 11 chaves de rodada.
        palavras = [list(chave[i:i+4]) for i in range(0, 16, 4)]
        rcon = 1
        while len(palavras) < 44:
            i = len(palavras)
            temp = palavras[-1][:]
            if i % 4 == 0:
                temp = temp[1:] + temp[:1]
                temp = [AES_SBOX[x] for x in temp]
                temp[0] ^= rcon
                rcon = _gf_mul(rcon, 2)
            palavras.append([x ^ y for x, y in zip(palavras[i-4], temp)])
        self.subchaves = [sum(palavras[4*r:4*r+4], []) for r in range(11)]

    def _addroundkey(self, state, rodada):
        for i, k in enumerate(self.subchaves[rodada]):
            state[i] ^= k

    def cifrar_bloco(self, bloco: bytes) -> bytes:
        if len(bloco) != 16:
            raise ValueError('Bloco AES precisa ter 16 bytes.')
        state = list(bloco)
        self._addroundkey(state, 0)
        for rodada in range(1, 10):
            state[:] = [AES_SBOX[x] for x in state]
            _aes_shiftrows(state)
            _aes_mixcolumns(state)
            self._addroundkey(state, rodada)
        state[:] = [AES_SBOX[x] for x in state]
        _aes_shiftrows(state)
        self._addroundkey(state, 10)
        return bytes(state)

    def decifrar_bloco(self, bloco: bytes) -> bytes:
        if len(bloco) != 16:
            raise ValueError('Bloco AES precisa ter 16 bytes.')
        state = list(bloco)
        self._addroundkey(state, 10)
        for rodada in range(9, 0, -1):
            _aes_shiftrows(state, inverso=True)
            state[:] = [AES_INV_SBOX[x] for x in state]
            self._addroundkey(state, rodada)
            _aes_mixcolumns(state, inverso=True)
        _aes_shiftrows(state, inverso=True)
        state[:] = [AES_INV_SBOX[x] for x in state]
        self._addroundkey(state, 0)
        return bytes(state)



# Formato didatico: identificador (4), algoritmo (1), tamanho original (8), blocos cifrados.
# Cada bloco e cifrado separadamente. Isso revela padroes; NAO e seguro para dados reais.
ALGORITMOS = {"DES": (1, 8, 8), "RC5": (2, 8, 16), "AES": (3, 16, 16)}
MAGICA = b"M3D1"
LIMITE_ARQUIVO = 8 * 1024 * 1024


def _objeto_cifra(algoritmo, chave):
    if algoritmo == "RC5":
        return RC5(chave)
    if algoritmo == "AES":
        return AES128(chave)
    if algoritmo == "DES":
        class CifraDES:
            def cifrar_bloco(self, bloco):
                return des(bloco, chave)
            def decifrar_bloco(self, bloco):
                return des(bloco, chave, decrypt=True)
        return CifraDES()
    raise ValueError("Algoritmo desconhecido.")


def _validar_chave(chave_hex, algoritmo):
    comprimento = ALGORITMOS[algoritmo][2]
    try:
        chave = bytes.fromhex(chave_hex)
    except ValueError as erro:
        raise ValueError("Chave precisa ser hexadecimal.") from erro
    if len(chave) != comprimento:
        raise ValueError(f"Chave de {algoritmo} precisa ter {comprimento * 2} digitos hexadecimais.")
    return chave


def cifrar(dados: bytes, chave_hex: str, algoritmo="AES") -> bytes:
    algoritmo = algoritmo.upper()
    if algoritmo not in ALGORITMOS:
        raise ValueError("Escolha DES, RC5 ou AES.")
    ident, tamanho, _ = ALGORITMOS[algoritmo]
    cifra = _objeto_cifra(algoritmo, _validar_chave(chave_hex, algoritmo))
    falta = (-len(dados)) % tamanho
    dados = dados + bytes(falta)  # completa ultimo bloco com zeros, se necessario
    resultado = b"".join(cifra.cifrar_bloco(dados[i:i+tamanho])
                         for i in range(0, len(dados), tamanho))
    return MAGICA + bytes([ident]) + (len(dados) - falta).to_bytes(8, "big") + resultado


def decifrar(dados: bytes, chave_hex: str) -> bytes:
    if not dados.startswith(MAGICA) or len(dados) < 13:
        raise ValueError("Arquivo nao pertence a esta versao didatica.")
    ident = dados[4]
    algoritmo = next((nome for nome, valores in ALGORITMOS.items() if valores[0] == ident), None)
    if algoritmo is None:
        raise ValueError("Identificador de algoritmo invalido.")
    _, tamanho, _ = ALGORITMOS[algoritmo]
    original = int.from_bytes(dados[5:13], "big")
    blocos = dados[13:]
    if len(blocos) % tamanho or len(blocos) != ((original + tamanho - 1) // tamanho) * tamanho:
        raise ValueError("Tamanho do arquivo cifrado invalido.")
    cifra = _objeto_cifra(algoritmo, _validar_chave(chave_hex, algoritmo))
    resultado = b"".join(cifra.decifrar_bloco(blocos[i:i+tamanho])
                         for i in range(0, len(blocos), tamanho))
    return resultado[:original]


def cifrar_texto(texto: str, chave_hex: str, algoritmo="AES") -> str:
    return base64.b64encode(cifrar(texto.encode("utf-8"), chave_hex, algoritmo)).decode("ascii")


def decifrar_texto(texto_base64: str, chave_hex: str) -> str:
    try:
        dados = base64.b64decode(texto_base64, validate=True)
    except ValueError as erro:
        raise ValueError("Base64 invalido.") from erro
    return decifrar(dados, chave_hex).decode("utf-8")


def cifrar_arquivo(entrada: str, saida: str, chave_hex: str, algoritmo="AES"):
    origem, destino = Path(entrada), Path(saida)
    if origem.resolve() == destino.resolve() or destino.exists():
        raise ValueError("O arquivo de saida nao pode sobrescrever um existente.")
    if origem.stat().st_size > LIMITE_ARQUIVO:
        raise ValueError("Limite demonstrativo: 8 MiB.")
    destino.write_bytes(cifrar(origem.read_bytes(), chave_hex, algoritmo))


def decifrar_arquivo(entrada: str, saida: str, chave_hex: str):
    origem, destino = Path(entrada), Path(saida)
    if origem.resolve() == destino.resolve() or destino.exists():
        raise ValueError("O arquivo de saida nao pode sobrescrever um existente.")
    if origem.stat().st_size > LIMITE_ARQUIVO + 64:
        raise ValueError("Limite demonstrativo: 8 MiB.")
    destino.write_bytes(decifrar(origem.read_bytes(), chave_hex))
