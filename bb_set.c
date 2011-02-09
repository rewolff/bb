
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
  void *p;
  char *var, *val;
  int t; 
  float f;

  bb_init ();

  var = argv[1];
  if (!var)  usage (); 

  val = argv[2];
  if (!val) usage (); 
 
  vartype = bb_get_type (var);
  p = bb_get_ptr (var);

  sscanf (val, "%d", &t);
  sscanf (val, "%f", &f);
  switch (vartype) {
  case BB_BYTE:   * (unsigned char  *)p = t;break;
  case BB_SHORT:  * (unsigned short *)p = t;break;
  case BB_INT:    * (unsigned int   *)p = t;break;
  case BB_FLOAT:  * (float          *)p = f;break;
  case BB_BIT:    bb_writebit (p, t);break;
  }
  exit (0);
}
