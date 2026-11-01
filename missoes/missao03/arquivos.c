/* Experimento de arquivos AES-128-GCM usando OpenSSL EVP.
   O nucleo AES didatico permanece em aes.c; GCM usa implementacao auditada. */
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define CHUNK 4096
#define IVLEN 12
#define TAGLEN 16
static const unsigned char MAGIC[8]={'S','D','A','E','S','1','2','8'};
static int processar(const char *modo,const char *origem,const char *destino,const char *hex){
    unsigned char key[16],iv[IVLEN],tag[TAGLEN],in[CHUNK],out[CHUNK+16];
    FILE *fi=NULL,*fo=NULL; EVP_CIPHER_CTX *ctx=NULL; int ok=0,n,m,encrypt=strcmp(modo,"cifrar")==0;
    if(strlen(hex)!=32){fprintf(stderr,"Chave deve ter 32 digitos hexadecimais.\n");return 0;}
    for(int i=0;i<16;i++){unsigned int x; char a=hex[i*2],b=hex[i*2+1];
        if(!((a>='0'&&a<='9')||(a>='a'&&a<='f')||(a>='A'&&a<='F')) || !((b>='0'&&b<='9')||(b>='a'&&b<='f')||(b>='A'&&b<='F'))){fprintf(stderr,"Chave hexadecimal invalida.\n");return 0;}
        if (sscanf(hex + 2 * i, "%2x", &x) != 1)
        {
            return 0;
        }

        key[i] = (unsigned char)x;
    }
    if(strcmp(origem,destino)==0){fprintf(stderr,"Entrada e saida nao podem ser iguais.\n");goto fim;}
    fi=fopen(origem,"rb");if(!fi){perror("Abrir entrada");goto fim;}
    if(encrypt){if(RAND_bytes(iv,IVLEN)!=1)goto fim;}
    else {unsigned char magic[8];if(fread(magic,1,8,fi)!=8||memcmp(magic,MAGIC,8)||fread(iv,1,IVLEN,fi)!=IVLEN){fprintf(stderr,"Cabecalho invalido.\n");goto fim;}}
    fo=fopen(destino,"wb");if(!fo){perror("Abrir saida");goto fim;}
    ctx=EVP_CIPHER_CTX_new();if(!ctx)goto fim;
    if(encrypt){
        if(!EVP_EncryptInit_ex(ctx,EVP_aes_128_gcm(),NULL,NULL,NULL)||!EVP_CIPHER_CTX_ctrl(ctx,EVP_CTRL_GCM_SET_IVLEN,IVLEN,NULL)||!EVP_EncryptInit_ex(ctx,NULL,NULL,key,iv))goto fim;
        if(fwrite(MAGIC,1,8,fo)!=8||fwrite(iv,1,IVLEN,fo)!=IVLEN)goto fim;
        while((n=(int)fread(in,1,CHUNK,fi))>0){if(!EVP_EncryptUpdate(ctx,out,&m,in,n)||fwrite(out,1,m,fo)!=(size_t)m)goto fim;}
        if(ferror(fi)||!EVP_EncryptFinal_ex(ctx,out,&m)||fwrite(out,1,m,fo)!=(size_t)m||!EVP_CIPHER_CTX_ctrl(ctx,EVP_CTRL_GCM_GET_TAG,TAGLEN,tag)||fwrite(tag,1,TAGLEN,fo)!=TAGLEN)goto fim;
    }else{
        /* Retem os 16 bytes finais como tag, inclusive ao processar em blocos. */
        unsigned char pending[CHUNK+TAGLEN];size_t have=0,got;
        if(!EVP_DecryptInit_ex(ctx,EVP_aes_128_gcm(),NULL,NULL,NULL)||!EVP_CIPHER_CTX_ctrl(ctx,EVP_CTRL_GCM_SET_IVLEN,IVLEN,NULL)||!EVP_DecryptInit_ex(ctx,NULL,NULL,key,iv))goto fim;
        while((got=fread(pending+have,1,CHUNK,fi))>0){have+=got;if(have>TAGLEN){size_t process=have-TAGLEN;if(!EVP_DecryptUpdate(ctx,out,&m,pending,(int)process)||fwrite(out,1,m,fo)!=(size_t)m)goto fim;memmove(pending,pending+process,TAGLEN);have=TAGLEN;}}
        if(ferror(fi)||have!=TAGLEN){fprintf(stderr,"Arquivo truncado.\n");goto fim;}
        memcpy(tag,pending,TAGLEN);
        if(!EVP_CIPHER_CTX_ctrl(ctx,EVP_CTRL_GCM_SET_TAG,TAGLEN,tag)||EVP_DecryptFinal_ex(ctx,out,&m)!=1){fprintf(stderr,"Falha de autenticacao: chave incorreta ou arquivo adulterado.\n");goto fim;}
        if(fwrite(out,1,m,fo)!=(size_t)m)goto fim;
    }
    ok=1;
fim:
    if(ctx)EVP_CIPHER_CTX_free(ctx);
    if(fi)fclose(fi);
    if(fo){if(fclose(fo)!=0)ok=0;if(!ok)remove(destino);}
    OPENSSL_cleanse(key,sizeof key);
    if(ok)printf("Operacao concluida: %s\n",destino);
    return ok;
}
int main(int argc,char **argv){
    if(argc!=5||(strcmp(argv[1],"cifrar")&&strcmp(argv[1],"decifrar"))){fprintf(stderr,"Uso: %s cifrar|decifrar entrada saida chave_hex_32_digitos\n",argv[0]);return 1;}
    return processar(argv[1],argv[2],argv[3],argv[4])?0:1;
}
