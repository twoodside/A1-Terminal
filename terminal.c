#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(char** argc, int argv) {
  char **command=NULL;
  char *word=NULL;
  char c = '\0';
  int wordCounter = 10;
  int characterCounter = 10;
  int stopCommand=0;
  int stopWord=0;
  int i;
  int status;

  
  
  
  while (command == NULL || strcmp(command[0], "exit") != 0) {
    int maxCommandSize = wordCounter;
    command = calloc(maxCommandSize,sizeof(char*));
    wordCounter=0;
    word=NULL;
    
    while ( word==NULL || !stopCommand ){
      int maxSize = characterCounter;
      word = calloc(maxSize, sizeof(char));
      characterCounter = 0;
      c = '\0';

      while (!stopWord) {
        status = read(0, &c, 1);
        
        if (characterCounter==maxSize){
          maxSize*=2;
          word = realloc(word,maxSize*sizeof(char));
        }
        
        word[characterCounter++] = c;
        
        if (c == ' ' || c == '\t' || c == '\n' || c == EOF){
          stopWord=1;
        }
      }
      stopWord=0;
      
      if (word[characterCounter-1]=='\n'){
        stopCommand=2;
      }
      
      word[characterCounter-1] = '\0';
      
      if ( strcmp(word,";")==0 || strcmp(word,"&&")==0 || strcmp(word,"||")==0 ){
        stopCommand=1;
      }
      
      command[wordCounter++]=word;
      
      //printf("word: %s\n",word);
    }
    stopCommand=0;
    
    printf("command: ");
    for (i=0;i<wordCounter;i++){
      printf("%s ",command[i]);
    }
    printf("\n");
  }
  
  while (c!='\n'){
    read(0,&c,1);
  }

  printf("FINISHED\n");
  return 0;
}
