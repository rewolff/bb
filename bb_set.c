
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
  char *var, *val;
  int t; 
  float f;

  bb_init ();

  var = argv[1];
  if (!var)  usage (); 

  val = argv[2];
  if (!val) usage (); 
 
  vartype = bb_get_type (var);
  p = bb_get_handle (var);

  sscanf (val, "%d", &t);
  sscanf (val, "%f", &f);
  switch (vartype) {
  case BB_BYTE:  
  case BB_SHORT: 
  case BB_INT:   
  case BB_BIT:    bb_write_int   (p, t);break;
  case BB_FLOAT:  bb_write_float (p, f);break;
  }
  exit (0);
}
