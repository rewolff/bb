

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "bb_lib.h"


void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}

void nom_int (struct bb_var *p)
{
  int t;

  while (scanf ("%d", &t) > 0) {
    bb_write_int (p, t);
  }
}

void nom_float (struct bb_var *p)
{
  float t;

  while (scanf ("%f", &t) > 0) {
    bb_write_float (p, t);
  }
}



int main (int argc, char **argv)
{
  int vartype; 
  char *varn;
//  void *p;
  struct bb_var *var;

  bb_init ();

  varn = argv[1];
  if (!varn) usage (); 
 
  var = bb_get_handle (varn);
  vartype = bb_get_type (varn);
 // p = bb_get_ptr (var);

  switch (vartype) {
  case BB_BIT: 
  case BB_BYTE:
  case BB_SHORT:
  case BB_INT:  nom_int   (var);break;
  case BB_FLOAT:nom_float (var);break;

  }

  exit (0);
}
