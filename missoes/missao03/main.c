#include "aes.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static void print_hex(const char *label,const uint8_t data[16]){
    printf("%s",label);
    for(int i=0;i<16;i++) printf("%02X",data[i]);
    putchar('\n');
}
int main(void){
    const uint8_t mensagem[16]={0xAA,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0xAB,0x01,0x23,0x45,0x67,0x89,0xFD,0xC1};
    const uint8_t chave[16]={0xBA,0xD1,0x23,0x45,0x67,0x89,0xFD,0xC1,0xF1,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
    uint8_t subchaves[176],cifrado[16],recuperado[16];
    aes128_expand_key(chave,subchaves);
    aes128_encrypt_block(mensagem,cifrado,subchaves);
    aes128_decrypt_block(cifrado,recuperado,subchaves);
    print_hex("Mensagem:      ",mensagem);
    print_hex("Chave:         ",chave);
    print_hex("Cifrado:       ",cifrado);
    print_hex("Descriptografado: ",recuperado);
    printf("Teste: %s\n",memcmp(mensagem,recuperado,16)==0?"OK":"FALHOU");
    return memcmp(mensagem,recuperado,16)!=0;
}
