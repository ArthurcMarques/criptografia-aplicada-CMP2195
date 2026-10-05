# Entradas dinâmicas — 05/10/2026

## Objetivo e alcance

As mensagens não têm mais os limites fixos de 100, 500, 1000 ou 4096
posições. A leitura pelo teclado cresce até Enter, conforme a memória
disponível. Esta etapa não adiciona abertura de arquivos de texto nem
reúne os programas em um único menu.

`main.c` continua contendo apenas a função `main`, com transposição,
autochave e LFSR. Hill continua como programa separado. César/Vigenère
e afim/substituição continuam em seus cabeçalhos separados.

## Leitura e memória compartilhadas

Novo arquivo: `codigos/entrada_dinamica.h`.

- `ler_linha_dinamica()` lê até Enter ou EOF, sem truncar mensagens.
- A capacidade começa em 128 bytes e dobra quando necessário. Esse valor
  inicial não é um limite.
- Retorna uma string alocada, inclusive para uma linha vazia. Retorna
  `NULL` quando encontra EOF antes de qualquer caractere.
- Uma última linha sem Enter também é processada.
- Quem recebe a string deve chamar `free` quando terminar.
- `memoria_dinamica()` usa `realloc` e verifica falhas e multiplicações
  de tamanho antes de alocar. Em falta de memória, tamanho impossível ou
  erro de leitura, o programa informa o erro e encerra com código de falha.
  Não há tentativa de continuar a operação após esses erros fatais.
- Nos caminhos normais e nas entradas inválidas recuperáveis, os vetores
  temporários são liberados. Os tamanhos de mensagem e índices usam `size_t`.

```c
char *mensagem = ler_linha_dinamica();
if (mensagem != NULL) {
    /* Usar mensagem. */
    free(mensagem);
}
```

## César e Vigenère — `cesar_vigenere.h`

A estrutura mudou de vetor fixo para:

```c
typedef struct {
    size_t TamVet;
    int *texto;
} texto;
```

Inicialize **todas** as estruturas com `{0}`. Não copie uma estrutura por
atribuição para compartilhar seu ponteiro: cada estrutura possui sua memória.
Os valores armazenados representam letras em Z26, de 0 a 25.

- `lerTexto()` lê uma linha, converte letras ASCII para Z26 e substitui o
  conteúdo anterior. Ignora caracteres fora de A-Z/a-z. Retorna `false` em EOF.
- `liberarTexto()` libera o vetor e zera o ponteiro e o tamanho.
- As quatro funções das cifras alocam/redimensionam a saída e agora retornam
  `bool`. Verifique esse retorno.
- César continua pedindo sua chave pelo teclado. A chave é normalizada em
  módulo 26 e entradas numéricas inválidas permitem nova tentativa.
- Vigenère usa `i % Chave->TamVet`, sem criar um vetor auxiliar do tamanho
  da mensagem. Chave vazia é rejeitada. A saída não pode ser a própria chave.
- A entrada e a saída podem ser a mesma estrutura, exceto a restrição da chave.

```c
texto entrada = {0}, chave = {0}, cifrado = {0}, recuperado = {0};
if (lerTexto(&entrada) && lerTexto(&chave)) {
    if (criptVigenere(&entrada, &chave, &cifrado)) {
        decriptVigenere(&cifrado, &chave, &recuperado);
    }
}
liberarTexto(&entrada);
liberarTexto(&chave);
liberarTexto(&cifrado);
liberarTexto(&recuperado);
```

## Transposição, autochave e LFSR — `cifras.h`

- Removido `MAX_TEXTO`. Mensagens e resultados são alocados conforme a entrada.
- As funções de cifragem recebem `size_t tamanho`.
- `normalizar()` devolve `size_t`; o chamador reserva espaço para seus valores.
- O menu e as chaves numéricas também usam a leitura dinâmica. A antiga
  `ler_linha(char *)` foi substituída por `ler_linha_dinamica()`.
- Transposição reserva espaço adicional para o último bloco e mantém o
  preenchimento X. A chave ainda tem limite de 100 posições: esse limite é
  do bloco, não da mensagem. A permutação deve ser digitada em uma linha.
- Autochave mantém sua chave inicial de 0 a 25.
- LFSR aloca quatro inteiros por dígito hexadecimal. O registrador e a
  semente continuam com quatro bits, conforme o algoritmo. Zeros iniciais
  do texto hexadecimal são preservados.
- Chaves inválidas continuam permitindo nova tentativa. O programa volta
  ao menu após a operação; 0 encerra.

## Afim e substituição — `Segundo_Trabalho_Cripto.h`

`lerTextoSegundoTrabalho()` retorna uma linha dinâmica. O chamador libera
essa linha com `free`.

### Afim

As declarações `char vetor[500]` em parâmetros já eram ponteiros em C,
mas sugeriam um limite que não era verificado. Agora usam `char *` e os
laços usam `size_t`. `encript()` e `decript()` continuam alterando a string
recebida, sem alocar outra saída.

Ambas validam a chave A, normalizam as chaves e retornam 0 para chave sem
inverso. Letras são convertidas para maiúsculas; espaços, números e
pontuação são preservados. Essa preservação é uma correção em relação
ao código anterior, que aplicava a fórmula também a caracteres não alfabéticos.

### Substituição

As fun??es agora se chamam `encriptSubstituicao` e `decriptSubstituicao`,
mas agora percorrem todo `tamanhoEntrada`, sem blocos ou preenchimento.
As saídas são recebidas como `char **`, começam em `NULL` e são alocadas
pela função com espaço para o terminador `\0`.

```c
const char *alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *chave = "BCDEFGHIJKLMNOPQRSTUVWXYZA";
char *entrada = lerTextoSegundoTrabalho();
char *saida = NULL;
if (entrada != NULL) {
    encriptSubstituicao(entrada, alfabeto, chave, strlen(entrada), &saida);
}
free(entrada);
free(saida);
```

A saída deve ter memória própria e ser distinta da entrada e dos alfabetos.
Nas chamadas seguintes, a função libera a saída anterior apenas ao obter
um novo resultado válido. Os alfabetos são strings de 26 letras distintas, sem distinguir mai?sculas de min?sculas,
com terminador. Letras fora do alfabeto e outros caracteres são preservados.

Foi necessário corrigir os acessos fora dos vetores do código original:
laços fixos de 52 posições, saída pequena, uso do vetor errado e preenchimento
além da capacidade. A chave aleatória agora é um embaralhamento sem repetições,
pois letras repetidas impediam a decifragem. Continua sendo uma demonstração
com `rand`, não um gerador criptográfico. A chave é exibida.

## Hill — `cifraHill.c`

- Mensagem, texto preparado, cifrado, decifrado e vetores de cada bloco
  são alocados dinamicamente; removidos os arrays de 1000 posições e VLAs.
- A matriz continua dinâmica, agora com verificação de alocação.
- `prepararTexto(entrada, ordem)` retorna uma nova string que deve ser liberada.
- `cifraHill(texto, chave, ordem, &resultado)` e
  `decifraHill(texto, chave, ordem, &resultado)` retornam 1 em sucesso e
  recebem `char **resultado`, inicialmente `NULL` ou com memória própria.
- O resultado antigo só é liberado quando a operação obtém sucesso.
- A decifragem rejeita texto não alfabético ou tamanho incompatível com o bloco.
- O programa pede cada elemento da matriz em uma linha, entre 0 e 25.
- O determinante passa a ser calculado **módulo 26**, e não como determinante
  inteiro exato, para evitar crescimento dos valores intermediários.
  As rotinas de inversão desse arquivo são destinadas a Z26.
- A expansão por cofatores continua custosa para ordens grandes. Entrada
  dinâmica não torna o cálculo de matrizes grandes eficiente.
- A multiplicação por vetor-linha e o preenchimento X foram mantidos.

```c
char *cifrado = NULL;
char *decifrado = NULL;
if (cifraHill(mensagem, chave, ordem, &cifrado)) {
    decifraHill(cifrado, chave, ordem, &decifrado);
}
free(cifrado);
free(decifrado);
```

## Compilação e verificações

Na pasta `missoes/missao02`:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror codigos/main.c -o cifras.exe
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror codigos/cifraHill.c -o hill.exe
```

Os dois programas têm `main` próprios e são compilados separadamente.
Não foi feita a união dos cabeçalhos: a duplicação de funções matemáticas
entre `Segundo_Trabalho_Cripto.h` e `matematica.h` ainda deve ser resolvida
antes de incluí-los juntos no mesmo programa.

Verificações executadas:

- Compilação estrita dos programas e de pequenos programas para os outros cabeçalhos.
- César e Vigenère: ida e volta de 12.000 letras e chave vazia rejeitada.
- Afim e substituição: ida e volta de 12.000 caracteres, saída reutilizada,
  chave inválida e mensagem vazia na substituição.
- Transposição: 12.000 letras e nova tentativa de chave.
- Autochave: 15.000 letras e exemplo MODULO com K=8.
- LFSR: 6.000 dígitos hexadecimais comparados a uma sequência de referência,
  incluindo decifragem e exemplo DF0E01/0111.
- Hill: 12.000 letras e mensagem de tamanho ímpar com preenchimento X.
- Retorno ao menu, EOF sem Enter e mensagem vazia.

Os testes de tamanho não equivalem a uma medição de vazamentos de memória
nem a uma simulação de esgotamento de memória.

`Primeiro_Trabalho_Cripto.h` e `matematica.h` não foram alterados nesta etapa.
