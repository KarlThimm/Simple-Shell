#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include "sh.h"


int CommandSetup(int argc, char **argv, char **envp){ //Begin Shell Setup
  glob_t globbuf;
  extern char **environ; //Declare variables
  char buffer[MAXLINE];
  int i = 1;
  int status = 1;
  int argsct = 1;
  int process = 1;
  char *homedir;
  char promptbuffer[PROMPTMAX];
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command;
  char *arg;
  char *pathingcommand;
  char *p;
  char *pwd;
  char *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  struct pathelement *pathlist;
  if((pwd = getcwd(NULL, PATH_MAX+1))==NULL)
    {
      perror("getcwd");
      exit(2);
    }
  owd = calloc(PATH_MAX+1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';
  pathlist = get_path();

  while(process){ //Begin while loop for prompt
    printf("\n%s%s>>> ", prompt, pwd); //Prints prompt
    if(fgets(buffer, MAXLINE, stdin) != NULL){
		int linelength = strlen(buffer);
		if(buffer[linelength-1] == '\n'){
	  buffer[linelength-1] = 0;
	}
	strcpy(commandline, buffer);
    }
      int i = 0;
      char* typed = strtok(commandline, " "); //delete str
      command = typed;
      memset(args, '\0', MAXARGS*sizeof(char*));
      while(typed){ //while true
			args[i] = typed;
			typed = strtok(NULL, " ");
			i++;
    }
    if(command != NULL){  
	if(strcmp(command,"exit") == 0){
	  printcommand(command);
	  break;
	}
	else if((command[0] == '/')|(command[0] == '.') & (command[1] == '/')|((command[1] == '.') & (command[2] == '/'))){	  
	  if(access(command, X_OK) == -1){
	    printf("\nCannot access %s", command);
	    perror("Error ");
	  }
	  else{
	    printf("\nExecuted path %s\n", command);
	    executeCommand(command, args, status);
	  }
	}
	else if(strcmp(command, "which" ) == 0){ //execute which
	  for( int i = 1; args[i] != NULL; i++){
	    pathingcommand = which(args[i], pathlist);
	    printf("\n%s",pathingcommand);
	    free(pathingcommand);
	  }
	}
	else if(strcmp(command, "where" ) == 0){ //execte where
	  for(int i = 1; args[i] != NULL; i++){
	    pathingcommand = where(args[i], pathlist);
	    free(pathingcommand);
	  }
	}      
	else if(strcmp(command, "cd" ) == 0){ //cd (change directory)
	  printcommand(command);
	  if(args[1] == NULL){
	    strcpy(owd, pwd);
	    strcpy(pwd, homedir);
	    chdir(pwd);
	  }
	  else if(strcmp(args[1], "-") == 0){ //allows for "cd -"" to go home
	    p = pwd;
	    pwd = owd;
	    owd = p;
	    chdir(pwd);
	  }
	  else if(args[1] != NULL && args[2] == NULL){		
	    if(chdir(args[1]) == -1){
	      perror("Error ");
	    }
	    else{
	      memset(owd, '\0', strlen(owd));
	      memcpy(owd, pwd, strlen(pwd));  
	      getcwd(pwd, PATH_MAX+1);
	    }	    
	  }

	  //built-in list
	}else if(strcmp(command, "list") == 0){
	  printcommand(command);
	  if(args[1] == NULL){ //0 holds command
	    list(pwd);
	  }
	  else{
	    int i = 1;
	    while(args[i]){
	      if(access(args[i], X_OK) == -1){
		perror("\nError ");
	      }else{
		printf("\n%s:\n", args[i]);
		list(args[i]);
	      }
	      i++;
	    }
	  }
	}
	else if(strcmp(command, "prompt") == 0){
	  printcommand(command);
	  if(args[1] == NULL){
	    printf("\nEnter a prompt prefix: "); //Prefix puts something before pathing
	    if(fgets(promptbuffer, PROMPTMAX, stdin) != NULL){
	      int linelength = strlen(promptbuffer);
	      if(promptbuffer[linelength-1] == '\n'){
				promptbuffer[linelength-1] = 0; 
	      }
	      strtok(promptbuffer, " ");
	      strcpy(prompt, promptbuffer);
	    }
	  }
	  else{
	    strcpy(prompt, args[1]); 
	  }	
	}
	else if(strcmp(command, "printenv") == 0){ //prints the environment
	  printcommand(command);
	  if(args[1] == NULL){
	    printenv(environ);
	  }
	  else if(args[2] == NULL){
	    printf("\n%s\n", getenv(args[1]));
	  }
	  else{
	    printf("\nprintenv: Too many arguments.");
	  } 
	}
	else if(strcmp(command, "setenv") == 0){ //set Environment command, sets environment of where you currently are
	  printcommand(command);
	  if(args[1] == NULL){
	    printenv(environ);
	  }
	  else if(args[2] == NULL && (strcmp(args[1], "PATH") == 0 || strcmp(args[1], "HOME") == 0)){
	    printf("\nCan't set PATH or HOME to empty\n");
	  }
	  else if(args[2] == NULL){
	    if(setenv(args[1] , "", 1) == -1){
	      perror("Error ");
	    }
	  }
	  else if(args[3] == NULL){
	    if(setenv(args[1], args[2],1) == -1){
	      perror("Error ");
	    }
	    else{
	      if(strcmp(args[1], "PATH")==0){
		deletepath(&pathlist);
		pathlist = NULL; 
	      }
	      if(strcmp(args[1], "HOME") == 0){
		homedir = args[2];
	      }
	    }
	  }
	  else{
	    printf("\nsetenv: Too many arguments.");
	  }
	}
	else if( strcmp(command, "pid") == 0){ //prints process id of where you are (kill -9 and current pid kills terminal)
	  printcommand(command);
	  printf("\nPID: %d", getpid());
	}
	else if(strcmp(command, "pwd") == 0){
	  printcommand(command);
	  printf("\nPWD: %s", pwd);
	}
	else if( strcmp(command, "kill") == 0){ //Kill Command, kills a process that is going on or you can kill "yourself" and ecit the terminal
	  if(args[1] == NULL){
	    printf("\nMust enter an argument for kill");
	  }
	  else if(args[2] == NULL){
	    int temppid = -1;
	    sscanf(args[1], "%d", &temppid);
	    if(temppid != -1){
	      if(kill(temppid, 15) == -1){
		perror("Error ");
	      }
	    }else{
	      printf("\nInvalid pid");
	    }
	  }
	  else if(args[3] == NULL){
	    int temppid = -1;
	    int sig = 0;
	    sscanf(args[2], "%d", &temppid);
	    sscanf(args[1], "%d", &sig);
	    if(temppid != -1 && sig < 0){
	    if(temppid == getpid() && sig == -1){
	      }
	      if( kill(temppid, abs(sig)) == -1){
					perror("Error ");
	      }
	    }
	    else{
	      printf("\nIncorrect arguments for kill command");
	    }
	  }
	}
	else{
	  printf("Executed command: %s", command);
	  int sCard = getWildcardIndex('*', args); //for finding files (like .c files)
	  int qCard = getWildcardIndex('?', args); //wildcard ?
	  if(strcmp(command, "ls") == 0 && sCard != -1){	    
	    executeGlob(sCard, pathingcommand, pathlist, args, globbuf, status);	  
	  }
	  else if(strcmp(command, "ls") == 0 && qCard != -1){
	    executeGlob(qCard, pathingcommand, pathlist, args, globbuf, status);
	  }
	  else{
	    pathingcommand = which(command, pathlist);
	    executeCommand(pathingcommand, args, status);
	    free(pathingcommand);
	  	}   
		}
	}
}
  free(owd);  //Free rest of allocated mem, after valgrind no mem leaks
  free(pwd);
  free(prompt);  
  free(args);
  free(commandline);
  deletepath(&pathlist);
  pathlist = NULL;
  exit(0);
  return 0; 
} //End of CommandSetup Function


//Start Functions Here (to call in CommandSetup)
//Start which function
char *which(char *command, struct pathelement *pathlist){
  char buffer[MAXLINE];
  while(pathlist){
    snprintf(buffer, MAXLINE, "%s/%s", pathlist->element, command);
    if(access(buffer, X_OK) == -1){
      pathlist = pathlist->next;
    }
    else{
      int linelength = strlen(buffer);
      char* vari = calloc(linelength+1, sizeof(char));    
      strncpy(vari, buffer, linelength);
      return vari;
    }
  }
  return NULL;  
}

//Start Where function
char *where(char *command, struct pathelement *pathlist){
  char buffer[MAXLINE];
  int found = 0;
  char* vari;
  while(pathlist){
   snprintf(buffer, MAXLINE, "%s/%s", pathlist->element, command);
    if(access(buffer, X_OK) == -1){
      pathlist = pathlist->next;
    }
    else if (access(buffer, X_OK) != -1 && found == 0){
      found = 1;
      int linelength = strlen(buffer);
      vari = calloc(linelength+1, sizeof(char));    
      strncpy(vari, buffer, linelength);
      printf("\n%s", vari);
      pathlist = pathlist->next;
    }
    else if (access(buffer, X_OK) != -1){
      printf("\n%s", buffer);
      pathlist = pathlist->next;
    }
  }
    return vari;  
}

void list ( char* dir) //List Command
{
  DIR* adir = opendir(dir);
  struct dirent* afile;
  if(adir){
    while((afile = readdir(adir))!= NULL){
      printf("%s\n", afile->d_name); //Prints files in vertical order
    }
  }
  closedir(adir);
}

//Using glob to sort files for the wildcard function (allows for less confusion when files are similar)
void executeGlob(int Index, char *pathingcommand, struct pathelement *pathlist, char **args, glob_t globbuf, int status){
      globbuf.gl_offs = Index;
      glob(args[Index], GLOB_DOOFFS, NULL, &globbuf);
      for(int i = 0; i < Index; i++){
	globbuf.gl_pathv[i] = calloc(sizeof(char), strlen(args[i])+1 );
	strcpy( globbuf.gl_pathv[i], args[i]);	
      }
      pathingcommand = which(globbuf.gl_pathv[0], pathlist);
      executeCommand(pathingcommand, globbuf.gl_pathv, status);
      free(pathingcommand);
      for (int i = 0; i < Index; i++){
	free(globbuf.gl_pathv[i]);
      }
      globfree(&globbuf);
}

void executeCommand(char *pathingcommand, char** args, int status){
  if(pathingcommand==NULL){
      fprintf(stderr, "%s: This Is Not A Command!\n", args[0]); 
      }
      else{
				pid = fork(); //fork to child process
				if(pid == 0){ //Child process
	  			execve(pathingcommand,args,NULL);
	  			exit(EXIT_FAILURE); //Does not return (failed)
				}
			else if(pid < 0){
			}
			else{ //parent to child
	  		do{
	    		waitpid(pid, &status, WUNTRACED); //Waiting for child
	  		}
	  	while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
  }
}

void printenv(char ** envp){ //Print environment set command (can change with setenv)
    int i = 0;
    while(envp[i]!=NULL){ //while not empty
      printf("%s\n",envp[i]);
      i++;
    }
} 
  
void printcommand(char * command){ //Just prints that you are executing a command on prompt
  printf("\nExecuting built in command: %s", command);
}

int getWildcardIndex( char wildcard, char **args){
  int i = 0;
  char *p;
  while(args[i]){
    p = strchr(args[i], wildcard);
    if(p != NULL){
      return i;
    }
    i++;
  }
  return -1;
}
