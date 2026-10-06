# Missao 02 - cifras classicas

## Organizacao atual

| Arquivo em codigos/ | Responsabilidade |
|---|---|
| auxiliares.h | Memoria dinamica, leitura, validacao, estrutura texto, normalizacao e exibicao. |
| matematica.h | Funcoes matematicas e operacoes de matrizes de Hill. |
| cifras.h | Algoritmos de cifragem e decifragem de todas as cifras. |
| menu_cifras.h | Solicita textos e chaves, executa as operacoes e mostra resultados. |
| main.c | Apenas a funcao main, com o menu principal. |

Dependencias, sem inclusao circular:

```text
main.c -> menu_cifras.h -> cifras.h -> matematica.h -> auxiliares.h
```

As implementacoes ficam nos cabecalhos para manter o formato didatico.
Compile somente main.c: os cabecalhos nao foram separados em declaracoes
e implementacoes para uso em varias unidades de compilacao.

Os arquivos antigos das cifras foram consolidados nessa estrutura.
O programa separado cifraHill.c foi substituido pela opcao 8 do menu.
O primeiro trabalho original foi preservado em
referencias/Primeiro_Trabalho_Cripto.h, fora do programa atual.

## Compilar e executar

Na pasta missoes/missao02:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic codigos/main.c -o cifras.exe
./cifras.exe
```

Opcoes: 1 - LFSR, 2 - autochave, 3 - transposicao, 4 - Cesar,
5 - Vigenere, 6 - afim, 7 - substituicao, 8 - Hill, 0 - sair.
O menu reaparece depois de cada operacao.

## Interfaces e limites

Cesar recebe a chave como parametro, sem ler o teclado dentro do algoritmo:

```c
texto entrada = {0}, saida = {0};
/* Preencher entrada antes de cifrar. */
criptCesar(&entrada, &saida, 3);
liberarTexto(&entrada);
liberarTexto(&saida);
```

Inicialize estruturas texto com {0} e ponteiros de saida de Hill e
substituicao com NULL. Libere as saidas ao terminar.

As mensagens usam memoria dinamica. A chave da transposicao continua
limitada a 100 posicoes, sem limitar o tamanho da mensagem. O LFSR usa
semente de quatro bits. A substituicao usa 26 letras sem distinguir caixa.
Hill usa vetor-linha vezes matriz e preserva o preenchimento X. Sua
inversao e especifica para Z26, com determinante reduzido modulo 26;
a expansao por cofatores pode ser lenta para matrizes grandes.

## Testes e historico

[EXEMPLOS_TESTES.txt](EXEMPLOS_TESTES.txt) contem 23 casos com resultados
esperados. Todos passaram apos a reorganizacao, incluindo execucao
consecutiva no menu. Tambem foram verificados chave negativa, minusculas
e uma mensagem de 15.000 letras.

[ALTERACOES.md](ALTERACOES.md) registra
a comparacao completa entre os codigos originais de referencia e a versao
atual, incluindo correcoes, interfaces, reorganizacao e leitura de arquivos.

## Ler mensagens de arquivos

Depois de escolher a cifra e a operacao, na etapa da mensagem escolha:

- 1: digitar pelo teclado;
- 2: ler todo o conteudo de um arquivo de texto;
- 0: cancelar e voltar ao menu principal.

Informe o caminho completo ou relativo a pasta de onde o programa foi
executado. Caminhos com espacos sao aceitos, com ou sem aspas externas.
As chaves continuam sendo digitadas pelo teclado e a saida aparece no terminal.
O arquivo de entrada nao e modificado.

Use texto ASCII ou UTF-8 (com ou sem BOM). Arquivos com bytes nulos,
como muitos arquivos UTF-16, sao rejeitados. Erros de abertura/leitura
permitem escolher a origem novamente. A leitura e dinamica, limitada
pela memoria disponivel, e inclui todas as linhas do arquivo.

As regras de cada cifra continuam valendo: as cifras que normalizam em
A-Z descartam os caracteres fora desse alfabeto; afim preserva os demais
caracteres. Nao ha conversao de letras acentuadas para letras sem acento.
LFSR espera digitos hexadecimais e ignora espacos e quebras de linha.
Na decifragem de Hill, espacos e quebras de linha podem separar os blocos;
o numero de letras ainda precisa ser multiplo da ordem da matriz.

Exemplo LFSR: salve `DF0E01` em `mensagem.txt`. Digite, em ordem:

```text
1
1
2
mensagem.txt
0111
```

Resultado esperado: A794F0. Digite 0 no menu principal para sair.
Os exemplos em EXEMPLOS_TESTES.txt ja incluem a selecao de teclado.

## Geracao de chaves aleatorias - 06/10/2026

Ao cifrar, o programa oferece 1 - Informar, 2 - Gerar aleatoria e
0 - Cancelar. Na decifragem continua solicitando a chave usada originalmente.
Toda chave gerada e exibida no terminal; guarde-a para decifrar depois.
Ela nao e salva automaticamente em arquivo.

- Cesar e autochave: inteiro de 0 a 25.
- LFSR: semente de quatro bits entre 0001 e 1111.
- Vigenere: o usuario informa a quantidade de letras; o gerador escolhe A-Z.
- Transposicao: o usuario informa o bloco; o gerador embaralha as posicoes.
- Afim: A e escolhido entre os coprimos com 26 e B entre 0 e 25.
- Substituicao: permutacao das 26 letras, sem repeticoes.
- Hill: o usuario informa a ordem; uma diagonal com elementos invertiveis
  e transformada por operacoes de linha que preservam a inversa em Z26.

O gerador usa rand, inicializado uma vez com time, para uso didatico.
Nao e um gerador criptograficamente seguro e nao garante chaves distintas
entre execucoes. As operacoes de Hill nao amostram uniformemente todas as matrizes.

As rotinas gerais de sorteio e permutacao estao em auxiliares.h; os
cofatores de afim e as matrizes invertiveis sao gerados em matematica.h.
menu_cifras.h oferece a escolha e exibe as chaves. As formulas das cifras
nao foram modificadas. Os exemplos de teste incluem a escolha de chave manual.

Verificacao: cifragem e decifragem com a chave exibida nas oito opcoes,
os 23 exemplos manuais e 400 matrizes Hill de ordens 1 a 4, conferindo
que o produto pela inversa e a identidade modulo 26.
