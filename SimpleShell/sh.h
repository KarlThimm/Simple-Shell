#include "get_path.h"
#include <glob.h>

#define PROMPTMAX 64
#define MAXARGS   16
#define MAXLINE   128

int pid;
char *which(char *command, struct pathelement *pathlist);
char *where(char *command, struct pathelement *pathlist);
void list(char *dir);
void printenv(char **envp);
int CommandSetup(int argc, char **argv, char **envp);
int getWildcardIndex(char wildcard, char **args); 
void cleanup();
void executeCommand(char *command, char** args, int status);
void executeGlob(int cardIndex, char *commandpath, struct pathelement * pathlist, char **args, glob_t globbuf, int status);
void printcommand(char * command);


