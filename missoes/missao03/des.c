#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int DES_BlockSize = 64;
const int DES_KeySize = 56;
const int DES_C0Size = 28;
const int DES_D0Size = 28;

const int pc1[56] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

// Tabela PC-2 (48 posições) - Comprime os 56 bits para gerar a subchave de 48 bits[cite: 3, 13]
const int pc2[48] = 
{
    14, 17, 11, 24,  1,  5,  3, 28,
    15,  6, 21, 10, 23, 19, 12,  4,
    26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};
// Permutação Inicial (IP)
int ip[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// Permutação Inicial Inversa (IP^-1)
int ip_inversa[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};
// Função de Expansão (E)
int expansao_e[] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};
// Permutação P
int permutacao_p[] = {
    16,  7, 20, 21,
    29, 12, 28, 17,
     1, 15, 23, 26,
     5, 18, 31, 10,
     2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};// Caixas S (S1 a S8)
int s_boxes[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};


// Protótipo da Função F necessário para chamar na Encript
void funcaoF(char a[33], char j[49]);

//Calculo de chave
void Des_CalcularChave(char kEntrada[65], char subchaves[16][49])
{
    // Permutação PC1 - Lê os 64 bits originais e seleciona 56
    char kEntradaCopy[65];
    strcpy(kEntradaCopy, kEntrada);

    for(int i = 0; i < DES_KeySize; i++)
    {
        kEntrada[i] = kEntradaCopy[pc1[i] - 1];
    }
    kEntrada[56] = '\0'; // Fecha a chave de 56 bits
    
    char c0[29];
    memcpy(c0, &kEntrada[0], 28);
    c0[28] = '\0';
    
    char d0[29];
    memcpy(d0, &kEntrada[28], 28);
    d0[28] = '\0';

    // 16 Rodadas para gerar subchaves
    for(int j = 0; j < 16; j++)
    {
        char aux;
        if(j == 0 || j == 1 || j == 8 || j == 15)
        {
            // Shift de 1 bit
            aux = c0[0];
            for(int i = 0; i < 27; i++) { c0[i] = c0[i + 1]; }
            c0[27] = aux;
            
            aux = d0[0];
            for(int i = 0; i < 27; i++) { d0[i] = d0[i + 1]; }
            d0[27] = aux;
        }
        else
        {
            // Shift de 2 bits
            char aux1 = c0[0], aux2 = c0[1];
            for(int i = 0; i < 26; i++) { c0[i] = c0[i + 2]; }
            c0[26] = aux1; c0[27] = aux2;
            
            aux1 = d0[0]; aux2 = d0[1];
            for(int i = 0; i < 26; i++) { d0[i] = d0[i + 2]; }
            d0[26] = aux1; d0[27] = aux2;
        }

        // Reconstrói a chave de 56 bits
        memcpy(kEntrada, c0, 28);
        memcpy(&kEntrada[28], d0, 28);
        kEntrada[56] = '\0';

        // Permutação PC2 para gerar a subchave da rodada
        for(int i = 0; i < 48; i++)
        {
            subchaves[j][i] = kEntrada[pc2[i] - 1];
        }
        subchaves[j][48] = '\0';
    }
}

// A função F agora é void e modifica o array 'a' diretamente
void funcaoF(char a[33], char j[49])
{
    char expanded[49];

    // Expansão E e XOR imediato com a subchave 'j'
    for(int i = 0; i < 48; i++)
    {
        // Lê do array 'a' original, subtrai '0' para virar número e faz XOR com a chave
        int bitA = a[expansao_e[i] - 1] - '0';
        int bitKey = j[i] - '0';
        expanded[i] = (bitA ^ bitKey) + '0'; 
    }
    expanded[48] = '\0';

    char resultado_sboxes[33];

    // Aplicação das 8 Caixas S
    for(int i = 0; i < 8; i++)
    {
        char linha[3];
        char coluna[5]; // 4 bits + \0

        // Linha: bits das pontas (0 e 5)
        linha[0] = expanded[i * 6 + 0];
        linha[1] = expanded[i * 6 + 5];
        linha[2] = '\0';
        
        // Coluna: os 4 bits do meio
        memcpy(coluna, &expanded[i * 6 + 1], 4);
        coluna[4] = '\0';

        // Converte as strings binárias para inteiro decimal
        int num_linha = strtol(linha, NULL, 2);
        int num_coluna = strtol(coluna, NULL, 2);

        // Busca o valor na matriz S correspondente (0 a 15)
        int numeroInt = s_boxes[i][num_linha][num_coluna];

        // Converte o decimal resultante de volta para string binária de 4 bits
        for(int bit = 3; bit >= 0; bit--)
        {
            resultado_sboxes[i * 4 + (3 - bit)] = ((numeroInt >> bit) & 1) + '0';
        }
    }
    resultado_sboxes[32] = '\0';

    // Permutação P final devolvendo o valor para o array 'a'
    for(int i = 0; i < 32; i++)
    {
        a[i] = resultado_sboxes[permutacao_p[i] - 1];
    }
    a[32] = '\0';
}

// Calculo 1
void Des_Encript(char textoOriginal[65], char kSubchaves[16][49])
{
    char textoOriginalCopy[65];
    char L[33]; // Tamanho 33 para os 32 bits + '\0'
    char R[33]; // Tamanho 33 para os 32 bits + '\0'

    // Permutação IP
    strcpy(textoOriginalCopy, textoOriginal);
    for(int i = 0; i < 64; i++)
    {
        textoOriginal[i] = textoOriginalCopy[ip[i] - 1];
    }

    // Separa L0 e R0
    memcpy(L, textoOriginal, 32);
    L[32] = '\0';
    memcpy(R, &textoOriginal[32], 32);
    R[32] = '\0';
    
    // 16 Rodadas da Rede de Feistel
    for(int j = 0; j < 16; j++)
    {
        char R_antigo[33];
        strcpy(R_antigo, R); // 1. Guarda o R atual (pois o R será modificado e precisaremos dele para ser o novo L)

        // 2. Passa o R atual pela função F e pela subchave da rodada
        // (A funcaoF modifica o próprio 'R')
        funcaoF(R, kSubchaves[j]); 
        
        // 3. Faz o XOR: Novo R = L_antigo XOR resultado_da_funcao_F (que está em R)
        for(int i = 0; i < 32; i++)
        {
            R[i] = ((L[i] - '0') ^ (R[i] - '0')) + '0';
        }
        
        // 4. Novo L = R_antigo
        strcpy(L, R_antigo);
    } 

    //Junção R + L e Permutação Inversa (IP-1)
    char blocoFinal[65];
    
    // O DES inverte as metades no final (R depois L)
    memcpy(blocoFinal, R, 32);
    memcpy(&blocoFinal[32], L, 32);
    blocoFinal[64] = '\0';

    // Aplica IP-1 usando o bloco final combinado e joga no textoOriginal
    for(int i = 0; i < 64; i++)
    {
        textoOriginal[i] = blocoFinal[ip_inversa[i] - 1];
    }
    textoOriginal[64] = '\0';
}

// Calculo 1
void Des_Decript(char textoOriginal[65], char kSubchaves[16][49])
{
    char textoOriginalCopy[65];
    char L[33]; // Tamanho 33 para os 32 bits + '\0'
    char R[33]; // Tamanho 33 para os 32 bits + '\0'

    // Permutação IP
    strcpy(textoOriginalCopy, textoOriginal);
    for(int i = 0; i < 64; i++)
    {
        textoOriginal[i] = textoOriginalCopy[ip[i] - 1];
    }

    // Separa L0 e R0
    memcpy(L, textoOriginal, 32);
    L[32] = '\0';
    memcpy(R, &textoOriginal[32], 32);
    R[32] = '\0';
    
    // 16 Rodadas da Rede de Feistel
    for(int j = 15; j >= 0; j--)
    {
        char R_antigo[33];
        strcpy(R_antigo, R); // 1. Guarda o R atual (pois o R será modificado e precisaremos dele para ser o novo L)

        // 2. Passa o R atual pela função F e pela subchave da rodada
        // (A funcaoF modifica o próprio 'R')
        funcaoF(R, kSubchaves[j]); 
        
        // 3. Faz o XOR: Novo R = L_antigo XOR resultado_da_funcao_F (que está em R)
        for(int i = 0; i < 32; i++)
        {
            R[i] = ((L[i] - '0') ^ (R[i] - '0')) + '0';
        }
        
        // 4. Novo L = R_antigo
        strcpy(L, R_antigo);
    } 

    //Junção R + L e Permutação Inversa (IP-1)
    char blocoFinal[65];
    
    // O DES inverte as metades no final (R depois L)
    memcpy(blocoFinal, R, 32);
    memcpy(&blocoFinal[32], L, 32);
    blocoFinal[64] = '\0';

    // Aplica IP-1 usando o bloco final combinado e joga no textoOriginal
    for(int i = 0; i < 64; i++)
    {
        textoOriginal[i] = blocoFinal[ip_inversa[i] - 1];
    }
    textoOriginal[64] = '\0';
}

//Calculo 2

int main()
{
    char subChaves[16][49];
    char chaveDeSeguranca[65] = "0001001100110100010101110111100110011011101111001101111111110001";
    Des_CalcularChave(chaveDeSeguranca, subChaves);

    char texto[65] = "1010101010101010101010101010101010101010101010101010101010101010";
    
    Des_CalcularChave(chaveDeSeguranca, subChaves);

    Des_Encript(texto, subChaves);
    printf("Texto Encriptado: %s\n", texto);

    Des_Decript(texto, subChaves);
    printf("Texto Decriptado: %s\n", texto);

    return 0;
}