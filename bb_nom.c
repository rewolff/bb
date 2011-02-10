

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "bb_lib.h"

int interval = 100000; // in microseconds. 10x per second.


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
  void *p;
  char *var;

  bb_init ();

  var = argv[1];
  if (!var) usage (); 
 
  vartype = bb_get_type (var);
  p = bb_get_ptr (var);

  switch (vartype) {
  case BB_BIT: 
  case BB_BYTE:
  case BB_SHORT:
  case BB_INT:  nom_int   (p);break;
  case BB_FLOAT:nom_float (p);break;

  }

  exit (0);
}
