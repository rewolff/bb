

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>


#include "bb_lib.h"

int interval = 100000; // in microseconds. 10x per second.


void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}


void mon_int (struct bb_var *p)
{
  int new;
  int old;
  
  old = bb_get_int (p) + 1; // Always different!
  while (1) {
    new = bb_get_int (p);
    if (old != new) {
      printf ("%d\n", new);
      fflush (stdout); 
      old = new;
    }
    usleep (interval);
  }
}


void mon_float (struct bb_var *p)
{
  float new;
  float old;
  
  old = NAN;
//  bb_get_float (p) * 2; // almost always different!
  while (1) {
    new = bb_get_float (p);
    if (old != new) {
      printf ("%f\n", new);
      fflush (stdout); 
      old = new;
    }
    usleep (interval);
  }
}



int main (int argc, char **argv)
{
  int vartype; 
  struct bb_var *p;
  char *var;

  bb_init ();

  var = argv[1];
  if (!var) usage (); 
 
  vartype = bb_get_type (var);
  p = bb_get_handle (var);
  if (p == NULL)
  {
    printf("Variable '%s' not found... Exiting.\n", var);
    exit(-1);
  }

  switch (vartype) {
  case BB_BIT: 
  case BB_BYTE: 
  case BB_SHORT:
  case BB_INT:  mon_int (p);break;
  case BB_FLOAT:mon_float (p);break;
  }

  exit (0);
}
