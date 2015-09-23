#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  char* str;
  int length;
} String;

String readWord();
String* readCommand();

int main(char** argc, int argv) {
  String *command=NULL;
  
  int stopCommand=0;
  int stopWord=0;
  int i;
  int status;
  
  while (command == NULL || strcmp(command[0].str, "exit") != 0) {
    command = readCommand();
    
    printf("command: ");
    for (i=0;command[i].str!=NULL;i++){
      printf("%s ",command[i].str);
    }
    printf("\n");
  }
  
  printf("FINISHED\n");
  return 0;
}

String* readCommand(){
  String *command;
  String word=(String){NULL,0};
  int maxCommandSize = 8;
  int wordCounter=0;
  command = calloc(maxCommandSize,sizeof(String));
  int stopCommand=0;
  
  while ( word.str==NULL || !stopCommand  ){
    word = readWord();
    
    
    
    if (word.str[word.length-1]=='\n'){
      stopCommand=2;
    }
    
    word.str[word.length-1] = '\0';
    
    if ( strcmp(word.str,";")==0 || strcmp(word.str,"&&")==0 || strcmp(word.str,"||")==0 ){
      stopCommand=1;
    }
    
    if (wordCounter==maxCommandSize){
      maxCommandSize*=2;
      command=realloc(command,maxCommandSize*sizeof(String));
    }
    
    command[wordCounter++]=word;
    
    //printf("word: %s\n",word.str);
  }
  return command;
}

String readWord(){
  int maxSize = 8;
  String word;
  word.str = calloc(maxSize, sizeof(char));
  int characterCounter=0;
  char c = '\0';
  int stopWord=0;

  while (!stopWord) {
    read(0, &c, 1);
    
    if (characterCounter==maxSize){
      maxSize*=2;
      word.str = realloc(word.str,maxSize*sizeof(char));
    }
    
    word.str[characterCounter++] = c;
    
    if (c == ' ' || c == '\t' || c == '\n' || c == EOF){
      stopWord=1;
    }
  }
  
  word.length=characterCounter;
  return word;
}