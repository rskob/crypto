#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define RULE1 "qwertyuiopasdfghjklzxcvbnm"
#define RULE2 "mnbvcxzlkjhgfdsapoiuytrewq"
#define RULE3 "zabcdefghijklmnopqrstuvwxy"

#define FREQUENCY1 1
#define FREQUENCY2 26
#define FREQUENCY3 676

#define KEY_SIZE 3
#define TEXT_SIZE 1000
#define ALPHABET_LENGTH 26

char alphabet[ALPHABET_LENGTH + 1] = "abcdefghijklmnopqrstuvwxyz"; 

typedef struct {
    char* rule;
    int frequency;
    char start_position;
} Rotor;


void print_Rotor(Rotor* Rotor)
{
    for(int i = 0; i < ALPHABET_LENGTH; i++){
        printf("%c: %c\n", alphabet[i], Rotor->rule[i]);
    }
}

char* validate(char* string, int length)
{
    for(int i = 0; i < length; i++){
        if(!isalpha(string[i])){
            return NULL;
        };
        string[i] = tolower(string[i]);
    }
    return string;
}

int main()
{
    Rotor Rotor1 = {.rule=RULE1, .frequency=FREQUENCY1};
    Rotor Rotor2 = {.rule=RULE2, .frequency=FREQUENCY2};
    Rotor Rotor3 = {.rule=RULE3, .frequency=FREQUENCY3};

    char* text = malloc(TEXT_SIZE + 1);
    fgets(text, TEXT_SIZE, stdin);
    
    char* text_newline = strchr(text, '\n');
    if(text_newline != NULL){
        *text_newline = '\0';
    } else {
        char c;
        while((c = getchar()) != EOF);
    }

    int text_length = strlen(text);
    if((text = validate(text, text_length)) == NULL){
        puts("Text myst contain only letters (not even spaces)");
        return 1;
    }

    char* key = malloc(KEY_SIZE + 2);
    fgets(key, KEY_SIZE + 2, stdin);
    char* key_newline = strchr(key, '\n');
    if(key_newline != NULL){
        *key_newline = '\0';
    }
    else{
        puts("Too long");
        return 1;
    }

    if(strlen(key) != KEY_SIZE){
        perror("Too short");
        return 1;
    }

    if((key = validate(key, KEY_SIZE)) == NULL){
        perror("invalid key");
        return 1;
    }

    Rotor1.start_position = key[0];
    Rotor2.start_position = key[1];
    Rotor3.start_position = key[2];

    for(int i = 0; i < text_length; i++){
        char letter1 = Rotor1.rule[((text[i] - 'a') + (Rotor1.start_position - 'a')) % ALPHABET_LENGTH];
        Rotor1.start_position = alphabet[((Rotor1.start_position - 'a') + ((i + 1) % Rotor1.frequency == 0)) % ALPHABET_LENGTH];

        char letter2 = Rotor2.rule[((letter1 - 'a') + (Rotor2.start_position - 'a')) % ALPHABET_LENGTH];
        Rotor2.start_position = alphabet[((Rotor2.start_position - 'a') + ((i + 1) % Rotor2.frequency == 0)) % ALPHABET_LENGTH];

        char letter3 = Rotor3.rule[((letter2 - 'a') + (Rotor3.start_position - 'a')) % ALPHABET_LENGTH];
        Rotor3.start_position = alphabet[((Rotor3.start_position - 'a') + ((i + 1) % Rotor3.frequency == 0)) % ALPHABET_LENGTH];    

        printf("%c", letter3);
    }

    return 0;
}
