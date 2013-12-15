

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>


#include "bb_lib.h"



void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}

int main (int argc, char **argv)
{
  //  int vartype; 
  struct bb_var *p1, *p2;
  char *var1, *var2;
  float dz, last, out;

  bb_init ();

  var1 = argv[1];
  if (!var1) usage (); 
 
  var2 = argv[2];
  if (!var2) usage (); 
 
  p1 = bb_get_handle (var1);
  p2 = bb_get_handle (var2);
  if (!p1 || !p2) {
    fprintf (stderr, "Cant find var %s or %s\n", var1, var2);
    exit (1);
  }
  dz = atof (argv[3]);
  out = last = bb_get_float (p1);
  bb_write_float (p2, out); 

  //printf ("dz =%f\n", dz);

  while (1) {
    //printf ("%f %f %f %f\n",  bb_get_float (p1), out, last, fabs (out - bb_get_float (p1)));
    if (fabs (out - bb_get_float (p1)) > dz) {
      out = last;
      bb_write_float (p2, out); 
    }
    last = bb_get_float (p1);
    usleep (100000);
  }
  exit (0);
}
