# Cifras de transposicao e de fluxo

A biblioteca `codigos/cifras.h` contém as implementações das seis funções:

- `encriptar_transposicao(entrada, saida, tamanho, chave, bloco)`
- `decriptar_transposicao(entrada, saida, tamanho, chave, bloco)`
- `encriptar_fluxo(entrada, saida, tamanho, chave)`
- `decriptar_fluxo(entrada, saida, tamanho, chave)`
- `encriptar_fluxo_lfsr(entrada, saida, tamanho, semente)`
- `decriptar_fluxo_lfsr(entrada, saida, tamanho, semente)`

Inclua `#include "cifras.h"` para usar. As funções são `static`,
permitindo incluir o cabeçalho em vários arquivos C sem duplicar símbolos
na ligação. Não é necessário compilar um arquivo de implementação separado.

Na transposicao e na autochave, os textos são vetores de inteiros em Z26 (A=0, ..., Z=25), com `tamanho`
elementos. O chamador fornece os vetores de saída com capacidade suficiente
e valida as entradas. A função auxiliar `normalizar` converte letras ASCII
para esses valores, ignorando espaços, números e pontuação; seu vetor de
destino deve ter capacidade para todas as letras do texto. Use texto sem acentos.

## Transposição

A chave é uma permutação de índices de 0 a `bloco - 1`. Cada posição da
chave indica a origem da letra na saída. Por exemplo, a chave interna
`{2, 0, 1}` transforma `ABCDEF` em `CABFDE`. A mesma chave decifra.
Os vetores de entrada e saída devem ser distintos e não se sobrepor;
`bloco` deve ser positivo e `tamanho` deve ser múltiplo dele.

O programa `main.c` (opcao 3) recebe posições de 1 até o tamanho do bloco
e converte para índices internos. Pede novamente chaves inválidas e
preenche blocos incompletos com X. A decifragem mantém esses X.

## Cifras de fluxo do PDF

O programa `main.c` oferece as duas variantes concretas apresentadas nas
paginas 18 a 22 de `Criptografia_classica.pdf`. Sincrono e nao sincrono
sao classificacoes dos geradores, e nao algoritmos adicionais.

### 1 - Binaria com LFSR (Z2)

Corresponde ao exemplo das paginas 20-21. O registrador possui quatro bits
`[k1, k2, k3, k4]`. Em cada passo:

1. Emite `k1` como bit do fluxo antes do deslocamento.
2. Calcula a realimentacao `k1 XOR k2`.
3. Atualiza o estado para `[k2, k3, k4, k1 XOR k2]`.
4. Combina o bit da mensagem com o bit emitido usando XOR.

A decifragem usa o mesmo XOR e reinicia com a mesma semente. Este fluxo e
sincrono: depende apenas da semente. Para as sementes nao nulas, seu
periodo e 15 bits. A semente `0000` e rejeitada porque gera apenas zeros.

Funcoes da biblioteca:

```c
encriptar_fluxo_lfsr(entrada, saida, tamanho, semente);
decriptar_fluxo_lfsr(entrada, saida, tamanho, semente);
```

Entrada e saida sao vetores de `int` contendo bits 0 ou 1; `tamanho` e a
quantidade de bits. A semente e um inteiro de 1 a 15 representando os
quatro bits (por exemplo, `0111` corresponde a 7). O chamador valida os
parametros e reserva a saida. Cada chamada reinicia o registrador.

No programa, a palavra e hexadecimal, sem prefixo `0x`, e a semente e
uma string de quatro bits. Os bits sao processados do mais significativo
ao menos significativo de cada digito, da esquerda para a direita.
Zeros iniciais sao preservados; chaves invalidas permitem nova tentativa.

Teste solicitado (escolha variante 1, operacao 1):

```text
Palavra:          DF0E01
Semente:         0111
Fluxo de chave:  011110001001101011110001 = 789AF1
Cifrado:         DF0E01 XOR 789AF1 = A794F0
Decifrado:       A794F0 XOR 789AF1 = DF0E01
```

### 2 - Autochave (Z26)

Corresponde a pagina 22. A chave inicial K e um inteiro de 0 a 25.
O fluxo e nao sincrono: depende da chave e das letras do texto original.

```text
z[0] = K
z[i] = m[i-1], para i >= 1
c[i] = (m[i] + z[i]) mod 26
m[i] = (c[i] - z[i] + 26) mod 26
```

As funcoes `encriptar_fluxo` e `decriptar_fluxo` recebem vetores de valores
entre 0 e 25 e a chave inicial inteira. Na decifragem, cada letra
recuperada fornece o proximo valor da sequencia. Nao ha preenchimento.
As funcoes de ambas as variantes aceitam entrada e saida no mesmo vetor,
mas nao sobreposicao parcial.

Exemplo: K=8, texto `MODULO`, fluxo `8 12 14 3 20 11`, cifrado `UARXFZ`.
Decifrar com K=8 recupera `MODULO`. No menu, escolha variante 2, depois
cifrar ou decifrar, informe o texto e a chave. Chaves invalidas sao
solicitadas novamente.

## Compilar e executar

Na pasta `missoes/missao02`:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic codigos/main.c -o cifras.exe
./cifras.exe
```

O codigo-fonte fica em apenas dois arquivos: `codigos/cifras.h`, com as
funcoes das cifras e auxiliares de entrada e validacao, e `codigos/main.c`,
contendo apenas a funcao `main` com o menu principal.
Escolha 1 para LFSR, 2 para autochave ou 3 para transposicao; depois
selecione cifrar ou decifrar.

O LFSR usa um vetor de quatro bits para mostrar o deslocamento passo a passo.
O menu usa `switch` e a opcao 0 encerra o programa. Apos cada operacao,
o menu reaparece. As funcoes nao usam `inline`.
