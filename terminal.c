#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct{
  char *str;
  int length;
} String;

typedef struct{
  String *c;
  int length;
} Array;

extern char **environ;

String readWord();
Array readCommand();
int isCommand(char *str);
int processMgmt(Array command);
void printCommand(Array command);
Array clearInput(Array command);

int main(char** argv, int argc) {
  Array command=(Array){NULL,0};
  
  while (1) {
    if ( command.c==NULL || !isBooleanOperator(command.c[command.length-1].str) ){
      printf(">");
      fflush(stdout);
    }
    command = readCommand();
    
    /*printf("Read %d words: ",command.length);
    printCommand(command);*/
    
    if (strcmp(command.c[0].str, "exit") == 0){
      break;
    }
    
    int status = processMgmt(command);	
    
    if ( isBooleanOperator(command.c[command.length-1].str) ){
      switch (command.c[command.length-1].str[0]){
        case '&':
          if (status!=0){
            command=clearInput(command);
          }
          break;
        case '|':
          if (status==0){
            command=clearInput(command);
          }
          break;
        case ';':
          //Don't clear input
          break;
      }
    }
    
  }
  
  printf("Good bye.\n");
  return 0;
}

Array clearInput(Array command){
  char c='\0';
  while (c!='\n'){
    read(0,&c,1);
  }
  
  command.c=NULL;
  command.length=0;
  return command;
}

void printCommand(Array command){
  int i;
  
  for (i=0;i<command.length;i++){
    printf("%s ",command.c[i].str);
  }
  printf("\n");
}

int isBooleanOperator(char *str){
  if (
    strcmp(str,"&&")==0 ||
    strcmp(str,"||")==0 ||
    strcmp(str,";")==0
  ){
    return 1;
  }
  return 0;
}

int processMgmt(Array command){
  pid_t process = fork();
  if (process<0){
    return (int)process;
  }
  else if (process==0){
    if (command.c[0].str[0]!='/'){
      //Relative path
      char *pathList = getenv("PATH");
      char *path = strtok(pathList,":");
      char *temp;
      do{
        temp = malloc(sizeof(char)*(command.c[0].length+strlen(path)));
        temp = strcpy(temp,path);
        strcat(temp,"/");
        strcat(temp,command.c[0].str);
      } while (!isCommand(temp) && (path=strtok(NULL,":"))!=NULL );
      
      if (path!=NULL){
        command.c[0].str = temp;
      }
    }
    
    int argc=command.length+1;
    if (isBooleanOperator(command.c[command.length-1].str)){
      argc--;
    }
    
    {
      char *args[argc];
      int i;
      
      for (i=0;i<argc-1;i++){
        args[i]=command.c[i].str;
      }
      args[i]=0;
      
      int status = execve(command.c[0].str,args,environ);
    }
    
    printf("%s: command not found\n",command.c[0].str);
    exit(-1);
  }
  else if (process>0){
    int status;
    waitpid(process,&status,0);
    
    return status;
  }
  
  return -1;
}

/*
void processManagement(){
  process = fork();
    
    if (process<0){
      continue;
    }
    else if (process==0){
      //This is the child
      char c='\0';
      printf("this is the child\n");
      
      if (command[0].str[0]=='/'){
        int commandFound = searchForCommand(command[0].str);
        printf( "%d\n",commandFound );
      }
      else{
        char *newCommand = malloc(sizeof(char)*(strlen(command[0].str)+16)); //Something about this strcat fails
        strcpy(newCommand,"/usr/local/bin/");
        newCommand = strcat(newCommand,command[0].str);
        
        printf("%s\n",newCommand);
        
        int commandFound = searchForCommand(newCommand);
        printf( "%d\n",commandFound );
      }
      
      printf("c:");
      
      fflush(stdout);
      
      while (c!='\n'){
        read(0, &c, 1);
      }
      
      if (strcmp(command[0].str,"exit")==0){
        printf("exiting with 0 status\n");
        fflush(stdout);
        exit(0);
      }
      else{
        printf("exiting with 1 status\n");
        fflush(stdout);
        exit(1);
      }
    }
    else{
      //Parent
      int status;
      waitpid(process,&status,0); //This isn't waiting for the child process to exit?!
      printf(">status: %d\n",status);
    }
}
*/


int isCommand(char* location){
  return access(location,X_OK)==0;
}

Array readCommand(){
  int wordCounter=0;
  int stopCommand=0;
  int maxCommandSize=8;
  String word=(String){NULL,0};
  Array command=(Array){NULL,0};
  command.c = calloc(maxCommandSize,sizeof(String));
  
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
      command.c=realloc(command.c,maxCommandSize*sizeof(String));
    }
    
    command.c[wordCounter++]=word;
  }
  command.length=wordCounter;
  
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