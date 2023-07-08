#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct pathelement *get_path();

struct pathelement
{
  char *element;  //elements of list being added  
  struct pathelement *next; //For linked list points to node
};

void printpath( struct pathelement* curr); //Print pathing
void deletepath( struct pathelement** head); //Delete pathing (so itdoesnt stack)

