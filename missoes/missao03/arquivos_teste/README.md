# Arquivos para testar a missao 3

Execute na raiz do projeto:

```powershell
python missoes/missao03/main.py
```

## Teste rapido com AES

1. Escolha a opcao 5 (Experimento AES com arquivo).
2. Informe: missoes/missao03/arquivos_teste/acentos.txt
3. Use a chave: 000102030405060708090A0B0C0D0E0F
4. Confira a mensagem: Arquivos identicos: True

O programa cria acentos.txt.aes e acentos.txt.recuperado ao lado do original.
Para repetir o experimento, escolha outro arquivo ou remova os dois arquivos
gerados anteriormente. O programa recusa sobrescrever arquivos existentes.

## Teste separado com DES, RC5 ou AES

1. Escolha a opcao 3 (Cifrar arquivo) e selecione o algoritmo.
2. Informe o caminho de um dos arquivos desta pasta.
3. Aceite o caminho de saida sugerido pressionando Enter.
4. Informe uma das chaves de teste abaixo.
5. Escolha a opcao 4 (Decifrar arquivo).
6. Informe o caminho do arquivo .m3 gerado e aceite a saida sugerida.
7. Use a mesma chave da cifragem.

Chaves apenas para os testes:

- DES: 133457799BBCDFF1
- RC5: 000102030405060708090A0B0C0D0E0F
- AES: 000102030405060708090A0B0C0D0E0F

## Casos incluidos

- simples.txt: texto curto.
- acentos.txt: texto UTF-8 com acentos e simbolos.
- varios_blocos.txt: texto com 40 linhas, ocupando varios blocos.
- vazio.txt: arquivo sem conteudo.
- bloco_8_bytes.txt: exatamente um bloco DES/RC5.
- bloco_16_bytes.txt: exatamente um bloco AES.
- todos_bytes.bin: os 256 valores possiveis de um byte.
- zeros_no_final.bin: verifica se os zeros finais originais sao preservados.

Os arquivos .bin devem ser comparados como bytes, pois nao sao textos.
Todos os oito arquivos foram testados com os tres algoritmos e recuperados
sem diferencas. Esta pasta contem apenas os originais para uso no menu.
