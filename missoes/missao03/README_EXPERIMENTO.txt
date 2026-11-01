MISSÃO 3 - AES-128 em C + experimento com arquivos

1) Núcleo AES (aes.c/aes.h): implementa SubBytes, ShiftRows, MixColumns, AddRoundKey, expansão de chave, 
cifragem e decifragem de blocos AES-128. Não depende de OpenSSL.
   gcc -std=c11 -Wall -Wextra aes.c main.c -o aes_demo
   gcc -std=c11 -Wall -Wextra aes.c test_vectors.c -o aes_teste
   ./aes_teste

2) Experimento de arquivos (arquivos.c): usa OpenSSL EVP AES-128-GCM para processar arquivos arbitrários, 
com IV aleatório de 12 bytes e tag de autenticação de 16 bytes.
   Linux (OpenSSL dev instalado): gcc -std=c11 -Wall -Wextra arquivos.c -o arquivos -lcrypto
   Windows MSYS2 UCRT64: pacman -S mingw-w64-ucrt-x86_64-openssl
   gcc -std=c11 -Wall -Wextra arquivos.c -o arquivos.exe -lcrypto

   ./arquivos cifrar original.txt protegido.aes 00112233445566778899aabbccddeeff
   ./arquivos decifrar protegido.aes recuperado.txt 00112233445566778899aabbccddeeff
   cmp original.txt recuperado.txt   (Windows: fc /b original.txt recuperado.txt)

O arquivo cifrado armazena: identificador de 8 bytes + IV de 12 bytes + ciphertext + tag de 16 bytes.

ATENCAO: a senha hexadecimal de exemplo e publica e serve APENAS para demonstracao. Para uso real, gerar 
chaves aleatorias secretas e armazena-las em cofre seguro. Passar chaves na linha de comando pode 
expo-las no historico e na lista de processos. Nunca reutilizar o mesmo IV com a mesma chave (aqui e 
aleatorio, com risco residual de colisao). O programa de exemplo escreve a saida descriptografada antes 
de verificar a tag; em caso de erro a apaga, mas isso NAO oferece garantia de liberacao atomica de 
plaintext: use diretório temporario protegido e publicacao atomica para sistemas reais. Evitar caminhos 
de saida que ja contenham dados importantes, pois sao sobrescritos. O arquivo de entrada deve permanecer 
imutavel durante o processamento.

OBSERVACAO IMPORTANTE: arquivos.c usa o AES-GCM da OpenSSL, e NAO chama aes.c. Essa escolha preserva a implementacao didatica manual e usa uma biblioteca de seguranca confiavel para o modo autenticado. Explicar isso ao grupo e na apresentacao. A especificacao da missao exige experimento AES com arquivos, mas nao especifica GCM.
