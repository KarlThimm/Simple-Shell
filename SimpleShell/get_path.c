#include "get_path.h"

struct pathelement *get_path()
{
  char *path;
  char *p;
  struct pathelement *temp, *pathlist = NULL;

  p = getenv("PATH");
  path = malloc((strlen(p)+1)*sizeof(char)); 
  strncpy(path, p, strlen(p));
  path[strlen(p)] = '\0';
  p = strtok(path, ":"); 
  do   
  {     
    if(!pathlist) //Beginning of list 
    {
      temp = calloc(1, sizeof(struct pathelement));
      pathlist = temp;
    }
    else  
    {
      temp->next = calloc(1, sizeof(struct pathelement));// Adding more elements to path list
      temp = temp->next;
    }
    temp->element = p; 
    temp->next = NULL;
  }
  while ( p = strtok(NULL, ":") );
    return pathlist;
}


void printpath( struct pathelement* curr){ //print out pathing (list)
  while(curr){
    printf("%s ", curr->element);
    curr = curr->next;

  }
}

void deletepath( struct pathelement** head){ //Delete path so no overlapping
  
    struct pathelement *curr = *head; //current set to head
    struct pathelement *temp;
    if(curr!=NULL){
      free(curr->element);
    }
    while(curr){    
      temp = curr;
      curr = curr->next;
      free(temp);
    }
}
