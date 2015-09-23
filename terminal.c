#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct{
  char* str;
  int length;
} String;

String readWord();
String* readCommand();
int searchForCommand(char *str);

int main(char** argc, int argv) {
  String *command=NULL;
  
  int stopCommand=0;
  int stopWord=0;
  int i;
  int status;
  
  while (command == NULL || strcmp(command[0].str, "exit") != 0) {
	pid_t process;
	printf(">");
	fflush(stdout);
    command = readCommand();
    
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
	
	
	
	/*
    printf("command: ");
    for (i=0;command[i].str!=NULL;i++){
      printf("%s ",command[i].str);
    }
    printf("\n");
	*/
  }
  
  printf("FINISHED\n");
  return 0;
}

int searchForCommand(char* location){
	return access(location,F_OK);
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