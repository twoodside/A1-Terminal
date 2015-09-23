#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(char** argc, int argv) {
    char *word;
    char c = '\0';
    int characterCounter = 10;
    int status;

    while (word == NULL || strcmp(word, "exit") != 0) {

        int maxSize = characterCounter;
        word = calloc(maxSize, sizeof(char));
        characterCounter = 0;
        c = '\0';

        while (!(c == ' ' || c == '\t' || c == '\n' || c == EOF)) {
            status = read(0, &c, 1);
            
            if (characterCounter==maxSize){
                maxSize*=2;
                word = realloc(word,maxSize*sizeof(char));
            }
            
            word[characterCounter++] = c;
        }
        
        word[characterCounter-1] = '\0';
        
        printf("%.*s\n",maxSize, word);
    }
    
    while (c!='\n'){
      read(0,&c,1);
    }

    printf("FINISHED\n");
    return 0;
}
