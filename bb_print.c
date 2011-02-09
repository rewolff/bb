

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
  char *var;

  bb_init ();

  var = argv[1];
  if (!var) usage (); 
 
  vartype = bb_get_type (var);
  p = bb_get_ptr (var);

  switch (vartype) {
  case BB_BYTE:  printf ("%d\n", * (unsigned char  *)p);break;
  case BB_SHORT: printf ("%d\n", * (unsigned short *)p);break;
  case BB_INT:   printf ("%d\n", * (unsigned int   *)p);break;
  case BB_FLOAT: printf ("%f\n", * (float          *)p);break;
  case BB_BIT:      printf ("%d\n", bb_getbitval (p));break;
  }
  exit (0);
}
