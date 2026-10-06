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
