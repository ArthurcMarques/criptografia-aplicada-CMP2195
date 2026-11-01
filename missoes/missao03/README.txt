AES-128 em C — implementacao didatica

Compilar: gcc -std=c11 -Wall -Wextra aes.c main.c -o aes
Teste: gcc -std=c11 -Wall -Wextra aes.c test_vectors.c -o teste

API:
 aes128_expand_key(key[16], round_keys[176]);
 aes128_encrypt_block(input[16], output[16], round_keys[176]);
 aes128_decrypt_block(input[16], output[16], round_keys[176]);

Os bytes de entrada seguem ordem sequencial; internamente o State usa colunas.
AES-128: bloco de 16 bytes, chave de 16 bytes, 10 rodadas.
NAO e cifragem segura de arquivos: esta API opera um unico bloco, sem modo autenticado, nonce, ou verificacao de integridade. Para arquivos, usar AES-GCM em biblioteca criptografica auditada e gestao segura de chaves.
