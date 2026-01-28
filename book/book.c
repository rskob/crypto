#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>


#define KEY_FILE "key.txt"


#define LETTERS 26
#define DEFAULT_SIZE 4


typedef struct{
    char letter;
    int* indexes;
    int size;
    int amount;
} Letter;


void print_letters(Letter* letters)
{
    for(int i = 0; i < LETTERS; i++){
        Letter letter = letters[i];
        printf("Letter: %c, indexes: ", letter.letter);
        for(int j = 0; j < letter.amount; j++){
            printf("%d ", letter.indexes[j]);
        }
        printf("\n");
    }
}


void set_index(Letter* letter, int index)
{
    if(letter->amount == letter->size){
        int* temp_ptr = realloc(letter->indexes, sizeof(int) * letter->size * 2);
        if(temp_ptr == NULL){
            printf("Cannot add new index for letter %c", letter->letter);
            return;
        }
        letter->indexes = temp_ptr;
        letter->size = letter->size * 2;
    }
    letter->indexes[letter->amount++] = index;
}


int get_random_index(Letter* letter)
{
    return letter->indexes[rand() % letter->amount];
}



char* remember(char* map, int index, char letter, int* size)
{
    if(index >= *size){
        char* temp_ptr = realloc(map, *size * 2);
        if(temp_ptr == NULL){
            perror("can not allocate memory");
            exit(-1);
        }
        *size *= 2;
        map = temp_ptr;
    }
    map[index] = letter;
    return map;
}


int* write_number(int* encoded, int index, int number, int* size){
    if(index >= *size){
        int* temp_ptr = realloc(encoded, sizeof(int) * *size * 2);
        if(temp_ptr == NULL){
            perror("can not allocate memory");
            exit(-1);
        }
        *size *= 2;
        encoded = temp_ptr;
    }
    encoded[index] = number;
    return encoded;
}


int main()
{
    srand(time(NULL));


    FILE* fp = fopen(KEY_FILE, "r");
    if(fp == NULL){
        perror(KEY_FILE);
        return 1;
    }


    Letter letters[26];
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    for(int i = 0; i < 26; i++){
        letters[i] = (Letter){alphabet[i], malloc(sizeof(int) * DEFAULT_SIZE), DEFAULT_SIZE, 0};
    }


    char buff[51];
    int count = 0;
    int elements = 20;
    char* map = malloc(elements);
    while(fscanf(fp, "%50s", buff) == 1){
        char letter = tolower(buff[0]);
        if(!isalpha(letter)) continue;
        set_index(&letters[letter - 'a'], count);
        map = remember(map, count, letter, &elements);
        count++;
    }


    fclose(fp);


    for(int i = 0; i < 26; i++){
        if(letters[i].amount == 0){
            set_index(&letters[i], count);
            map = remember(map, count, letters[i].letter, &elements);
            count++;
        }
    }


    char text[1001];
    fgets(text, 1000, stdin);
    text[strcspn(text, "\n")] = '\0';


    printf("Original text: %s\n", text);


    int write = 0;
    int length = strlen(text);
    for(int read = 0; read < length; read++){
        if(isalpha(text[read])){
            text[write++] = text[read];
        }
    }
    text[write] = '\0';
    length = write;


    //encoding
    int size = length * 2;
    int* encoded = malloc(sizeof(int) * size);


    printf("Encoded text: ");
    for(int i = 0; i < length; i++){
        encoded = write_number(encoded, i, get_random_index(&letters[tolower(text[i]) - 'a']), &size);
        printf(i == length - 1 ? "%d" : "%d,", encoded[i]);
    }
    printf("\n");


    //decoding
    printf("Decoded text: ");
    for(int i = 0; i < length; i++){
        printf("%c", map[encoded[i]]);
    }
   
    free(encoded);
    free(map);


    for(int i = 0; i < LETTERS; i++){
        free(letters[i].indexes);
    }


    return 0;
}