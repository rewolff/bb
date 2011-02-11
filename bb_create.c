
#include <stdio.h>
#include <stdlib.h>

#include "bb_lib.h"

void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}


int main (int argc, char **argv)
{
  int vartype; 
  char *var;
  char *vartypes;
  struct bb_var     *p;

  bb_init ();

  var = argv[1];
  if (!var) usage (); 
 
  vartypes = argv[2];
  if (!vartypes) usage (); 

  vartype = bb_typestring_to_enum (vartypes);

  p = bb_get_handle (var);
  if (p)
  {
    printf("Variable %s already exists. Will not create again.\n", var);
    exit(-1);
  }


  bb_create_var (var, vartype);
  exit (0);
}

