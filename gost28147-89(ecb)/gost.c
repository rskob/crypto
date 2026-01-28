#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define IS_LITTLE_ENDIAN *((char*)&(uint32_t){1}) == 1 
#define KEY_PARTS 8
#define BYTES_IN_PART 4

#define KEY_FILE "key.txt"
#define SOURCE_FILE "text.txt"
#define DESTINATION_FILE "result.txt"
#define ROUNDS 32

const uint8_t Sbox[8][16] = {
    // RFC 4357
    {9, 6, 3, 2, 8, 1, 7, 4, 15, 12, 0, 5, 10, 11, 13, 14},
    {3, 14, 9, 8, 15, 0, 5, 2, 6, 11, 4, 1, 7, 10, 13, 12},
    {14, 6, 2, 3, 13, 8, 15, 5, 0, 7, 9, 11, 4, 1, 10, 12},
    {7, 10, 12, 13, 1, 3, 9, 0, 2, 14, 15, 8, 5, 6, 11, 4},
    {11, 5, 1, 9, 8, 15, 14, 4, 2, 3, 12, 7, 6, 10, 13, 0},
    {3, 13, 12, 1, 0, 7, 5, 9, 4, 8, 15, 6, 2, 11, 14, 10},
    {1, 2, 9, 0, 6, 8, 12, 4, 5, 1, 3, 7, 14, 10, 13, 11},
    {11, 2, 5, 14, 0, 6, 8, 12, 4, 5, 1, 3, 7, 9, 13, 15}
};


uint32_t* get_key_parts(FILE* fp)
{
    uint32_t* K = malloc(sizeof(uint32_t) * KEY_PARTS);
    for(int i = 0; i < KEY_PARTS; i++){
        uint8_t kx[BYTES_IN_PART];
        for(int j = 0; j < BYTES_IN_PART; j++){
            fscanf(fp, "%2hhx", &kx[IS_LITTLE_ENDIAN ? BYTES_IN_PART - j - 1 : j]);
        }
        K[i] = *(uint32_t*)&kx;
    }

    return K;
}

void box_substitute(uint32_t* sublock)
{
    for(int i = 0; i < 8; i++){
        uint32_t mask = (0xF0000000 >> i * 4);

        uint8_t block_4bits = (*sublock & mask) >> ((7 - i) * 4);
        *sublock = (*sublock & ~mask) | (Sbox[i][block_4bits] << ((7 - i) * 4));
    }
}


uint64_t read_block(FILE* fp)
{
    uint8_t bytes[8];
    uint8_t read = fread(bytes, 1, 8, fp);

    if(read == 0) return 0;
    if(read < 8){
        // must add some padding (PKCS7 in this case)
        uint8_t padding_length = 8 - read;
        for(int i = read; i < 8; i++){
            bytes[i] = padding_length;
        }
    }

    uint64_t block = 0;
    for(int i = 0; i < 8; i++){
        block |= ((uint64_t)bytes[i] << (i * 8));
    }

    return block;
}

uint64_t encrypt_block(uint64_t block, uint32_t* key_parts)
{
    uint32_t b_left = (uint32_t)(block >> 32);
    uint32_t b_right = (uint32_t)block;

    for(int i = 0; i < ROUNDS; i++){
        // 1) Get key_part
        uint32_t b_left_cpy = b_left;
        int index = !((i & 24) / 24) * (i % 8) + ((i & 24) / 24) * (7 - i % 8);
        uint32_t kx = key_parts[index];
        
        // 2) Addition modulo 2^32
        b_left = (uint32_t)((uint64_t)b_left + kx);

        // 3) Substitution box
        box_substitute(&b_left);
        
        // 4) Shift
        b_left = (b_left << 11) | (b_left >> 21);

        // 5) XOR
        b_right ^= b_left;

        // 6) Exchange of values
        b_left = b_right;
        b_right = b_left_cpy;
    }
    block = (uint64_t)(((uint64_t)b_left << 32) | b_right);
    return block;
}

int main()
{
    FILE* key_fp = fopen(KEY_FILE, "r");
    if(key_fp == NULL){
        perror(KEY_FILE);
        return 1;
    }

    uint32_t* key_parts = get_key_parts(key_fp);
    fclose(key_fp);

    FILE* source_fp = fopen(SOURCE_FILE, "rb");
    if(source_fp == NULL){
        perror(SOURCE_FILE);
        return 1;
    }

    FILE* destination_fp = fopen(DESTINATION_FILE, "wb");
    if(destination_fp == NULL){
        perror(DESTINATION_FILE);
        return 1;
    }

    for(;;){
        uint64_t block = read_block(source_fp);
        if(block == 0) break;

        block = encrypt_block(block, key_parts);

        fprintf(destination_fp, "%016llx", block);
        printf("%016llx", block);
    }

    free(key_parts);
    fclose(source_fp);
    fclose(destination_fp);

    return 0;
}