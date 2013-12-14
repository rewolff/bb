
#include <stdio.h>
#include <stdlib.h>

#include "bb_lib.h"

static char *progname;

void usage (void)
{
  fprintf (stderr, "usage: %s <varname> <type>.\n", progname);
  exit (1);
}


int main (int argc, char **argv)
{
  int vartype; 
  char *var;
  char *vartypes;
  struct bb_var     *p;

  progname = argv[0];
  bb_init ();

  var = argv[1];
  if (!var) usage (); 
 
  vartypes = argv[2];
  if (!vartypes) usage (); 

  vartype = bb_typestring_to_enum (vartypes);
  if (vartype == BB_INVALID) {
     fprintf (stderr, "Invalid typstring. \n");
     exit (0);
  }

  p = bb_get_handle (var);
  if (p) {
    printf("Variable %s already exists. Will not create again.\n", var);
    exit(-1);
  }

  bb_create_var (var, vartype);
  exit (0);
}

