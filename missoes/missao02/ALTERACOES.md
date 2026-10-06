# Registro geral de alteracoes do projeto

Atualizado em 05/10/2026. Este documento compara os codigos originais de
referencia com a versao atual. Abrange todas as etapas: criacao das cifras,
correcoes, mudancas de comportamento, interfaces, memoria, menus,
reorganizacao, leitura de arquivos e testes. A entrada dinamica e apenas
uma dessas alteracoes.

## Visao geral de todas as alteracoes

| Area | Alteracao realizada | Detalhamento |
|---|---|---|
| Transposicao | Implementacao em C/Z26, separacao de cifrar e decifrar, validacao da permutacao e preenchimento X. | Secao 9 |
| Fluxo | Troca do fluxo aditivo manual por autochave conforme o PDF e inclusao do LFSR binario. | Secao 9 |
| LFSR | Semente de quatro bits, mensagem hexadecimal, XOR, exibicao do fluxo e simplificacao do registrador para vetor. | Secao 9 |
| Cesar | Correcao do conflito TamVet, chave normalizada, retorno bool e chave recebida como parametro. | Secao 5 |
| Vigenere | Repeticao direta da chave, remocao do vetor auxiliar, rejeicao de chave vazia e retorno bool. | Secao 5 |
| Afim | Validacao do inverso, normalizacao das chaves, centralizacao matematica e tratamento de nao letras. | Secao 6 |
| Substituicao | Correcao dos vetores e chamadas, chave reversivel, novas assinaturas e troca de 52 para 26 letras. | Secao 7 |
| Normalizacao | Conversao para maiusculas e descarte dos caracteres fora do alfabeto nas cifras indicadas; documentada a excecao da afim. | Secao 11 |
| Hill | Include corrigido, matrizes validadas no menu, determinante modular, simplificacao da inversao e novas assinaturas. | Secao 8 |
| Matematica | Remocao de duplicacoes e revisao de primalidade, exponenciacao, inverso, phi e teorema chines. | Secao 3 |
| Memoria | Substituicao de arrays fixos/VLAs, size_t, malloc/realloc verificados e liberacao das saidas. | Secoes 4 a 9 |
| Funcoes auxiliares | Extracao de leitura numerica, normalizacao, exibicao, leitura de matriz, semente e permutacao. | Secao 4 |
| Menus | Todas as cifras reunidas, operacoes separadas, repeticao de chaves invalidas e retorno ao menu. | Secao 10 |
| main | Apenas a funcao main e um unico programa principal; remocao dos executaveis independentes do fluxo atual. | Secao 2 |
| Organizacao | Cinco arquivos por responsabilidade, sem inclusoes circulares; referencia original preservada. | Secoes 1 e 2 |
| Estilo | Retirada de inline, uso de switch, formulas explicitas e separacao entre algoritmos e interacao. | Secoes 2, 5 e 9 |
| Arquivos de texto | Escolha da origem, leitura completa, caminhos, BOM, cancelamento e tratamento de erros. | Secao 10 |
| Documentacao | README com compilacao/estrutura, roteiro EXEMPLOS_TESTES.txt e este registro geral. | Secoes 2 e 13 |
| Validacao | Exemplos conhecidos, ida e volta, mensagens longas, chaves invalidas, EOF e arquivos. | Secao 13 |
| Alteracoes desfeitas | Registro das solucoes intermediarias que nao fazem parte da versao final. | Secao 12 |

As secoes abaixo detalham o que foi preservado, corrigido, substituido ou
removido. Mudancas de comportamento estao indicadas explicitamente; nao
sao apresentadas como simples adaptacoes de memoria.

## 1. Referencias e destino dos arquivos

A comparacao considera os arquivos originalmente enviados e o codigo de
Cesar/Vigenere fornecido na conversa, alem da primeira implementacao de
transposicao e fluxo criada durante o trabalho.

| Origem | Destino atual | Alteracao estrutural |
|---|---|---|
| Primeiro_Trabalho_Cripto.h | matematica.h | Funcoes matematicas revisadas e centralizadas. O original permanece em referencias/Primeiro_Trabalho_Cripto.h. |
| Segundo_Trabalho_Cripto.h | cifras.h, matematica.h, auxiliares.h e menu_cifras.h | Afim e substituicao separadas da leitura, dos calculos auxiliares e das demonstracoes. |
| cifraHill.c | cifras.h, matematica.h, auxiliares.h e menu_cifras.h | Removido o main independente; Hill passou para a opcao 8 do programa principal. |
| Codigo original de Cesar/Vigenere, depois cesar_vigenere.h | cifras.h, auxiliares.h e menu_cifras.h | Estrutura dinamica, algoritmos separados da leitura e integracao ao menu. |
| transposicao.c e fluxo.c | cifras.h e menu_cifras.h | Funcoes e menus reunidos no programa principal. |
| entrada_dinamica.h | auxiliares.h | Leitura, alocacao e validacao compartilhadas. |
| hill.h | matematica.h e cifras.h | Operacoes de matriz separadas dos algoritmos de Hill. |

Os nomes antigos acima sao referencias historicas, nao arquivos que devem
ser compilados atualmente. Apenas o primeiro trabalho original foi preservado
na pasta referencias; os demais foram consolidados. As descricoes de seus
originais se baseiam no conteudo fornecido durante o trabalho.

## 2. Estrutura atual e compilacao

| Arquivo em codigos/ | Conteudo |
|---|---|
| auxiliares.h | Alocacao, leitura pelo teclado e por arquivo, validacao numerica, normalizacao, estrutura texto, exibicao e leitura de matriz. |
| matematica.h | Euclides, MDC, inverso e aritmetica modular, primalidade, exponenciacao, phi, teorema chines e matrizes de Hill. |
| cifras.h | Cifragem/decifragem, validacao dos alfabetos de substituicao e preparacao do texto de Hill. |
| menu_cifras.h | Coleta das chaves, escolha das operacoes, execucao de cada cifra e apresentacao dos resultados. Mantem tambem as demonstracoes afim() e substituicaoTamanhoIndeterminado(). |
| main.c | Somente a funcao main, com o menu principal. |

Dependencias principais, sem inclusoes circulares:

```text
main.c -> menu_cifras.h -> cifras.h -> matematica.h -> auxiliares.h
```

Os algoritmos nao solicitam mais dados pelo teclado: recebem seus parametros.
A interacao ficou no menu e nas funcoes auxiliares. As implementacoes ainda
ficam nos cabecalhos; algumas funcoes sao static e outras possuem ligacao
externa. Portanto, esta organizacao foi feita para compilar um unico .c,
nao como biblioteca pronta para varias unidades de compilacao.
O inline usado em uma versao intermediaria foi retirado.

Na pasta missoes/missao02:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror codigos/main.c -o cifras.exe
./cifras.exe
```

Nao compilar o antigo cifraHill.c ou os arquivos de referencia junto com main.c.

## 3. Matematica: original e implementacao atual

| Funcao | O que mudou em relacao ao primeiro trabalho |
|---|---|
| euclides | Algoritmo preservado, centralizado em matematica.h. |
| euclides_estendido | Algoritmo preservado. Continua retornando vetor static com MDC, x e y, sobrescrito a cada chamada. |
| mdc | Simplificada para retornar o resultado de euclides. Eliminada a segunda implementacao existente no segundo trabalho. |
| aritmetica_modular | Mantida a regra de resto nao negativo e retorno -1 para modulo invalido; simplificada a escrita. |
| inverso_modular | Valida modulo >= 2, normaliza o numero, verifica coprimalidade e normaliza o coeficiente de Euclides estendido. Retorna -1 sem imprimir mensagens. A busca por tentativa do segundo trabalho foi substituida pela implementacao compartilhada. |
| eh_primo | Rejeita todos os valores menores que 2, inclusive negativos; testa divisores apenas ate a raiz quadrada, sem calcular a raiz. |
| exp_modular | Reescrita por quadraturas sucessivas, sem vetor binario alocado. Eliminado o vazamento desse vetor. Valida expoente/modulo, trata expoente zero e usa long long nos produtos. |
| phi_euler | Reescrita por fatoracao com inteiros, sem malloc, realloc ou pow. Remove dependencia de ponto flutuante e a variavel fator nao utilizada. Retorna -1 para n < 1. |
| teorema_chines_resto | Valida quantidade, modulos > 1, coprimalidade dois a dois e produto dentro de INT_MAX. Usa long long e reducoes modulares nos termos. Retorna -1 para entradas invalidas. |
| eh_coprimo_26 | Movida do segundo trabalho para matematica.h e normaliza a chave antes de testar o MDC. |

As reescritas acima foram feitas na versao atual, nao no arquivo original
preservado em referencias. As funcoes inteiras nao oferecem precisao arbitraria:
os argumentos e calculos precisam caber nos tipos usados. Em particular,
nao usar LLONG_MIN nas rotinas que negam argumentos negativos, nem valores
que causem estouro nos coeficientes de Euclides estendido. mdc retorna int.

## 4. Memoria dinamica e leitura compartilhada

Antes havia mensagens com vetores de 100, 500, 1000 ou 4096 posicoes.
Agora os vetores de mensagens e resultados sao alocados conforme a entrada.
Os indices e comprimentos dessas mensagens usam size_t. Isso remove o limite
fixo de mensagem, mas nao o limite da memoria disponivel.

- memoria_dinamica(anterior, quantidade, tamanho): usa realloc, verifica a
  multiplicacao dos tamanhos e testa o ponteiro retornado. Em falha fatal,
  imprime o erro e encerra com EXIT_FAILURE. Quantidade zero reserva uma
  posicao para evitar depender de realloc com tamanho zero.
- ler_linha_dinamica(): capacidade inicial de 128 bytes, dobrada quando
  necessario. Le ate Enter ou EOF. Uma ultima linha sem Enter e aceita.
  Linha vazia e uma string alocada; EOF sem caracteres retorna NULL.
- numero_valido(): valida a linha inteira com strtol, limites e caracteres
  restantes; nao aceita, por exemplo, 3abc como numero 3.
- ler_numero(): repete entradas invalidas e devolve o numero por ponteiro;
  retorna 0 em EOF, permitindo distinguir EOF de numeros negativos.
- pedir_numero(): atalho para intervalos nao negativos; retorna -1 em EOF.
- escolher_operacao(): menu compartilhado de cifrar/decifrar.
- normalizar(): transforma letras ASCII em valores 0 a 25 e ignora o resto.
- imprimir_letras(): converte o vetor de Z26 para letras e exibe uma linha.
- ler_matriz_hill(): centraliza a leitura dos elementos da matriz.

Foram extraidas tambem ler_semente() e ler_permutacao(), no menu, para
separar validacao de chaves da execucao dos algoritmos.

Nos caminhos normais, cancelamentos e erros recuperaveis, os vetores
adquiridos sao liberados. Em falha fatal de alocacao, o processo encerra;
nao ha uma rotina geral para recuperar a operacao depois de faltar memoria.

## 5. Cesar e Vigenere

### Estrutura e erro do original

O original declarava #define TamVet 100 e, ao mesmo tempo, int TamVet na
estrutura. O pre-processador transformava int TamVet em int 100 e
Arquivo->TamVet em Arquivo->100. Primeiro a constante foi renomeada para
CAPACIDADE, com comentario explicativo; depois o vetor fixo foi removido:

```c
typedef struct {
    size_t TamVet;
    int *texto;
} texto;
```

Inicialize estruturas com {0}. Cada estrutura deve possuir sua memoria:
nao copie o ponteiro para outra estrutura que tambem sera liberada.
liberarTexto() libera o vetor e zera tamanho e ponteiro.
lerTexto() continua lendo letras pelo teclado, sendo usado para a chave
Vigenere. ler_mensagem_letras() permite escolher teclado ou arquivo para
a mensagem. Ambas normalizam em A-Z/Z26.

### Cesar

- criptCesar e decriptCesar passaram de void para bool.
- A chave saiu do scanf interno e passou a ser parametro. O menu faz a leitura.
- A chave e reduzida a 0..25, inclusive para valores negativos ou maiores que 25.
- A saida e alocada/redimensionada conforme a quantidade de letras.
- Entrada e saida podem ser a mesma estrutura.

```c
texto entrada = {0}, saida = {0};
if (lerTexto(&entrada)) {
    criptCesar(&entrada, &saida, 3);
}
liberarTexto(&entrada);
liberarTexto(&saida);
```

### Vigenere

- criptVigenere e decriptVigenere passaram de void para bool.
- Removido vetorAdaptado, que era um VLA do tamanho da mensagem.
- A repeticao da chave usa diretamente i % Chave->TamVet.
- Chave vazia e rejeitada. A estrutura de saida nao pode ser a estrutura da chave.
- A saida e dinamica; as formulas de soma/subtracao modulo 26 foram preservadas.

## 6. Afim

- Os parametros char vetor[500] foram escritos como char *. Em parametros
  de C, a declaracao antiga ja equivalia a ponteiro; o tamanho 500 nao
  protegia a entrada. A leitura agora realmente aloca conforme a mensagem.
- Os lacos usam size_t e percorrem a string sem recalcular strlen a cada passo.
- encript() e decript() continuam alterando a entrada no proprio vetor e
  retornando int, com 0 para chave sem inverso.
- A e B sao normalizados; a decifragem verifica o inverso antes de operar.
- O menu pede outro A quando ele nao e coprimo com 26; as chaves do menu
  ficam entre 0 e 25, embora as funcoes normalizem outros valores int.
- para_maiusculo() ficou em auxiliares.h.
- A demonstracao afim(), que cifra e decifra em sequencia, permanece em
  menu_cifras.h; o menu principal usa as operacoes separadamente.

Mudanca de comportamento: o original aplicava a formula a todos os
caracteres da string. Agora somente A-Z sao cifrados apos converter para
maiusculas; os demais bytes sao preservados. Portanto, na implementacao
atual afim ainda preserva espacos, numeros, pontuacao e quebras de linha.
Esse comportamento difere das cifras que filtram o texto para A-Z.

## 7. Substituicao

### Correcoes em relacao ao original

- Os lacos deixaram de processar sempre 52 posicoes e passaram a percorrer
  o comprimento real, evitando ler alem de mensagens curtas.
- A saida fixa de 52 posicoes foi substituida por alocacao dinamica com
  terminador, evitando escrita/leitura fora do vetor em mensagens longas.
- Removido o preenchimento ate multiplo de 52, que podia ultrapassar a entrada.
- Corrigidas as chamadas que usavam vetorValor em vez da mensagem e a
  decifragem que nao recebia corretamente o texto cifrado.
- A chave da demonstracao passou de sorteio com reposicao, que podia repetir
  letras e impedir a inversao, para embaralhamento sem repeticoes.
- A chave aleatoria da demonstracao e exibida. Usa rand apenas para fins
  didaticos. O menu normal solicita a chave ao usuario, permitindo reutiliza-la.

### Alfabeto e caracteres

O original usava 52 letras, distinguindo A-Z de a-z. A versao atual usa
exatamente 26 letras, sem distinguir maiusculas de minusculas, conforme
solicitado. Texto, alfabeto e chave sao convertidos para maiusculas.
validarAlfabetos() exige 26 letras A-Z distintas; A e a contam como repeticao.

Durante uma etapa intermediaria, caracteres fora do alfabeto eram copiados.
Esse comportamento foi desfeito: agora espacos, numeros, pontuacao e letras
acentuadas sao ignorados. Nao existe transliteracao: a letra acentuada e
removida, nao convertida para sua versao sem acento. A decifragem nao recupera
a formatacao descartada.

### Interface atual

- encriptAte52Posicoes -> encriptSubstituicao.
- decriptAte52Posicoes -> decriptSubstituicao.
- tamanhoEntrada passou de int para size_t e deve ser igual a strlen(entrada).
- A saida passou de char[] para char **; inicialize o ponteiro com NULL.
- A funcao aloca a nova saida e libera a anterior apenas em caso de sucesso.
- O chamador libera o resultado. Entrada, saida e alfabetos devem ter
  armazenamento distinto conforme o contrato das funcoes.

```c
const char *alfabeto = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *chave = "BCDEFGHIJKLMNOPQRSTUVWXYZA";
char *saida = NULL;
encriptSubstituicao("AbZz! 123", alfabeto, chave, strlen("AbZz! 123"), &saida);
/* saida: BCAA */
free(saida);
```

## 8. Hill

### Integracao e memoria

- O include inexistente ../../biblioteca.h foi inicialmente substituido
  por matematica.h. Na estrutura final, cifras.h inclui matematica.h;
  as rotinas de matriz foram movidas para matematica.h.
- Removidos texto[1000], cifrado[1000], decifrado[1000] e textoPreparado[1000].
- Vetores temporarios de bloco deixaram de ser VLAs e passaram para o heap.
- A matriz ja era dinamica; agora as alocacoes sao verificadas pelo auxiliar comum.
- O main exclusivo de cifraHill.c deixou de existir; Hill usa o menu principal.
- Cada elemento da matriz e digitado em uma linha, no intervalo 0..25.
- O menu verifica se a matriz tem inversa em Z26 e pede outra matriz se nao tiver.

### Calculos e contratos

- Mantida a convencao vetor-linha vezes matriz; nao foi trocada por vetor-coluna.
- Mantidos normalizacao em A-Z e preenchimento X na cifragem.
- determinanteHill agora devolve o determinante reduzido modulo 26, nao o
  determinante inteiro exato. A reducao durante a recursao evita crescimento
  dos produtos para matrizes com elementos entre 0 e 25.
- multiplicarVetorMatriz reduz modulo a cada soma.
- calcularInversaHill rejeita ordem <= 0 e modulo diferente de 26.
- Removida a matriz adjunta temporaria: a inversao acessa cofatores[j][i]
  diretamente. calcularTransposta continua disponivel, mas nao e usada nessa inversao.
- As rotinas matematicas assumem os limites de entrada documentados; o menu
  valida os valores. Chamadas diretas devem respeitar esses contratos.
- A expansao por cofatores continua recursiva e cara para matrizes grandes.
  Nao ha limite fixo de ordem 3 na versao atual.

### Mudancas de assinatura

| Original | Atual |
|---|---|
| void prepararTexto(char *entrada, char *saida, int ordem) | char *prepararTexto(const char *entrada, int ordem) |
| void cifraHill(char *texto, int **chave, int ordem, char *resultado) | int cifraHill(const char *texto, int **chave, int ordem, char **resultado) |
| int decifraHill(char *texto, int **chave, int ordem, char *resultado) | int decifraHill(const char *texto, int **chave, int ordem, char **resultado) |

prepararTexto retorna uma string nova; quem chama deve libera-la. As duas
operacoes recebem o endereco do ponteiro de saida, inicialmente NULL ou
com memoria propria, e retornam 1 em sucesso. A saida anterior so e
substituida em sucesso. A decifragem rejeita letras invalidas e comprimento
que nao seja multiplo da ordem. No menu, espacos e quebras de linha sao
removidos antes de decifrar para aceitar arquivos formatados em linhas;
a funcao decifraHill, chamada diretamente, continua exigindo somente letras.
Os X permanecem no texto decifrado.

## 9. Transposicao e cifras de fluxo

Essas implementacoes foram criadas durante o trabalho, nao vieram dos tres
arquivos de referencia adicionados posteriormente.

### Transposicao

- A antiga funcao transpor foi separada em encriptar_transposicao e
  decriptar_transposicao, preservando a mesma permutacao e sua inversao.
- Chaves invalidas deixaram de encerrar o programa: ler_permutacao pede nova tentativa.
- A chave externa usa posicoes 1..bloco, convertidas para indices internos 0..bloco-1.
- O texto deixou o limite de 4096 bytes; entrada e saida usam memoria dinamica.
- Reservado espaco para completar o ultimo bloco com X. Decifragem exige
  blocos completos e nao remove X automaticamente.
- Mantido MAX_CHAVE=100 para a chave/bloco; isso nao limita a mensagem.

### Autochave

A primeira versao de fluxo apenas somava uma sequencia de chave fornecida
inteira. Foi substituida pela autochave do PDF: z[0]=K e z[i]=m[i-1].
A decifragem usa a letra original ja recuperada para produzir o proximo z.
K fica entre 0 e 25. Texto e resultados passaram a usar memoria dinamica.

### LFSR

Foi acrescentada a variante binaria do PDF, separada da autochave:
registrador [k1,k2,k3,k4], saida k1 e novo bit k1 XOR k2. A atualizacao e
[k2,k3,k4,k1 XOR k2]. Cifragem e decifragem usam XOR com o mesmo fluxo.

A primeira implementacao compactava o estado em um inteiro com deslocamentos
de bits. Foi simplificada para um vetor de quatro bits e deslocamento
explicito. A mensagem e hexadecimal, processada do bit mais significativo
para o menos significativo. Zeros iniciais sao preservados. A semente tem
quatro bits e nao pode ser 0000. O texto pode ser dinamico, mas o registrador
continua de tamanho quatro por definicao do algoritmo.

Exemplo mantido: DF0E01, semente 0111, fluxo 789AF1, resultado A794F0.

## 10. Menu e leitura de arquivos

O menu unificado oferece LFSR, autochave, transposicao, Cesar, Vigenere,
afim, substituicao e Hill, com cifragem e decifragem separadas.
Depois de uma operacao, retorna ao menu; 0 encerra. main.c contem apenas main.

Na etapa da mensagem, ler_mensagem oferece:

- 1: teclado;
- 2: arquivo de texto;
- 0: cancelar a operacao e voltar ao menu.

As chaves continuam no teclado. O arquivo nao e modificado e a saida e
exibida no terminal; nao foi implementada gravacao do resultado em arquivo.

ler_arquivo_texto abre em rb e le todas as linhas ate EOF, aumentando o
vetor conforme necessario. Fecha o arquivo apos sucesso ou erro recuperavel.
Aceita texto ASCII/UTF-8, remove BOM UTF-8 inicial e rejeita bytes nulos,
como os encontrados em muitos arquivos UTF-16. Nao e um validador completo
de codificacao UTF-8 nem um conversor de codificacoes.

Caminhos podem conter espacos e aspas externas opcionais. Caminhos relativos
sao resolvidos a partir da pasta de execucao, nao obrigatoriamente a pasta
do codigo-fonte. Se o executavel for iniciado em codigos/output, teste.txt
sera procurado nessa pasta; para outro local, informe o caminho correspondente.
Abertura/leitura com erro permite escolher a origem novamente ou cancelar.
Arquivo vazio pode ser lido; cada cifra aplica suas validacoes ao conteudo.

remover_espacos elimina espacos, tabulacoes e quebras de linha dos dados
hexadecimais do LFSR e do texto cifrado de Hill antes da decifragem.

## 11. Tratamento atual dos caracteres

| Cifra | Regra atual |
|---|---|
| Cesar, Vigenere, transposicao e autochave | Letras ASCII convertidas para maiusculas; outros caracteres descartados. |
| Substituicao | Texto e chave sem distincao de caixa; alfabeto de 26 letras; caracteres fora de A-Z descartados. |
| Hill - cifrar | Somente letras ASCII em maiusculas, com X para completar blocos. |
| Hill - decifrar no menu | Remove espacos/quebras; exige letras e blocos completos. Nao remove X. |
| Afim | Converte letras para maiusculas, cifra A-Z e preserva os demais caracteres. |
| LFSR | Digitos hexadecimais; ignora espacos/quebras; outros caracteres tornam a entrada invalida. |

Nao ha transliteracao de acentos ou recuperacao de formatacao descartada.
A regra da afim acima e uma diferenca ainda existente no codigo, nao deve
ser confundida com a normalizacao das outras cifras.

## 12. Etapas intermediarias que nao representam o estado atual

- A tentativa inicial de unir tudo foi desfeita quando foi solicitado
  manter apenas matematica.h e a troca do include de Hill naquele momento.
  A integracao definitiva ocorreu posteriormente, a pedido do usuario.
- A restricao temporaria de Hill a matrizes 1..3 foi desfeita; a matriz atual e dinamica.
- A correcao inicial ampla de Cesar/Vigenere foi desfeita; o conflito TamVet
  foi depois corrigido isoladamente e, posteriormente, veio a adaptacao dinamica.
- A preservacao de caracteres externos na substituicao foi retirada.
- O alfabeto de 52 letras foi substituido por 26 letras.
- O uso de static inline foi retirado; nao e necessario para as formulas.
- Os programas separados e os cabecalhos por cifra deram lugar aos cinco
  arquivos atuais. Os nomes antigos nos exemplos historicos nao sao a API atual.

## 13. Verificacoes realizadas e alcance

Durante as etapas anteriores foram executados:

- Compilacao com C11, Wall, Wextra, Wpedantic e Werror.
- Os 23 casos de EXEMPLOS_TESTES.txt, tambem em sequencia no mesmo processo.
- Os mesmos 23 casos usando arquivos UTF-8 com BOM e caminhos com espacos.
- Mensagens entre 6.000 e 20.000 caracteres, maiores que os limites originais.
- Chaves invalidas, repetidas, negativas, EOF, linha sem Enter e cancelamento.
- Arquivos inexistentes, vazios, com bytes nulos e com varias linhas.
- Todas as 15 sementes nao nulas do LFSR e todas as 26 chaves iniciais de autochave.
- Exemplos de Hill, preenchimento e inversao; em uma etapa intermediaria,
  foram verificadas todas as matrizes 2x2 em Z26 daquela implementacao.

Esses registros se referem aos testes feitos nas respectivas etapas, nao
significam que todas as versoes receberam todos os testes novamente.
A atualizacao deste documento nao altera os algoritmos e nao executa uma
nova bateria. Os testes nao equivalem a auditoria de seguranca, medicao de
vazamentos ou simulacao de falta de memoria. EXEMPLOS_TESTES.txt foi atualizado
para incluir a escolha de teclado; os resultados esperados das cifras permanecem.
