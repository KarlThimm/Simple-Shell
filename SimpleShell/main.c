#include "sh.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int signal); 


int main( int argc, char **argv, char **envp )
{
  signal(SIGINT, sig_handler);
  signal(SIGTSTP, sig_handler);
  signal(SIGTERM, sig_handler);
  signal(EOF, sig_handler);
  return CommandSetup(argc, argv, envp);
}

void sig_handler(int signal)
{
  printf("\nIGNORING SIGNAL\n");
}
