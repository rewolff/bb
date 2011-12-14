

#include <stdlib.h>
#include <stdio.h>


#include "bb_lib.h"


void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}


int main (int argc, char **argv)
{
  int vartype; 
  struct bb_var *p;
  char *var;

  bb_init ();

  var = argv[1];
  if (!var) usage (); 
 
  p = bb_get_handle (var);
  if (!p) {
     fprintf (stderr, "Cant find var %s\n", var);
     exit (1);
  }
  vartype = bb_get_type (var);

  switch (vartype) {
  case BB_BYTE: 
  case BB_SHORT:
  case BB_INT:  
  case BB_BIT:   printf ("%d\n", bb_get_int (p));break;
  case BB_FLOAT: printf ("%f\n", bb_get_float (p));break;
  }
  exit (0);
}
