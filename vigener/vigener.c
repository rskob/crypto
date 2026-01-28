#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define TEXT_LENGTH 101
#define KEY_LENGTH 11


int main()
{
    char default_alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    
    char text[TEXT_LENGTH] = "";
    char key[KEY_LENGTH] = "";
    
    printf("Enter text: ");
    fgets(text, sizeof text, stdin);
    printf("Enter key: ");
    fgets(key, sizeof key, stdin);

    text[strcspn(text, "\n")] = '\0';
    key[strcspn(key, "\n")] = '\0';

    if(strlen(text) == 0 || strlen(key) == 0) exit(-1);

    printf("Original text: %s\n", text);

    //lower case
    for(int i = 0; text[i]; i++) text[i] = tolower(text[i]);
    for(int i = 0; key[i]; i++) key[i] = tolower(key[i]);
    
    //removing all spaces
    int text_length = strlen(text);
    for(int i = 0; i < text_length; i++){
        if(text[i] == ' '){
            for(int j = i; j < text_length - 1; j++){
                text[j] = text[j + 1];
            }
            i--;
            text_length--;
            text[text_length] = '\0';
        }
    }

    //creating vigener square
    char all_alphabets[26][27];
    strcpy(all_alphabets[0], default_alphabet);
    for(int i = 1; i < 26; i++){
        char new_alphabet[27] = "";
        for(int j = 0; j < 26; j++){
            new_alphabet[j] = default_alphabet[(j + i) % 26];
        }
        new_alphabet[26] = '\0';
        strcpy(all_alphabets[i], new_alphabet);
    }

    //encoding
    unsigned char key_length = strlen(key);
    char* encoded = malloc(text_length + 1);
    for(int i = 0; i < text_length; i++){
        if(!isalpha(text[i])) continue;

        char letter = key[i % key_length];
        encoded[i] = all_alphabets[letter - 'a'][text[i] - 'a'];
    }
    encoded[text_length] = '\0';
    printf("Encoded text: %s", encoded);
    
    //decoding
    char* decoded = malloc(text_length + 1);
    for(int i = 0; i < text_length; i++){
        char letter = key[i % key_length];
        decoded[i] = default_alphabet[(encoded[i] - all_alphabets[letter - 'a'][0] + 26) % 26];
    }
    decoded[text_length] = '\0';
    printf("\nDecoded text: %s", decoded);

    free(encoded);
    free(decoded);

    return 0;
}