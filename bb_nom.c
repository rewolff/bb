

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


void nom_bit (void *p)
{
  int val;

  while (scanf ("%d", &val) > 0) {
    bb_writebit (p, val);
  }
}


void nom_byte (volatile unsigned char *p)
{
  int val;

  while (scanf ("%d", &val) > 0) {
    *p = val;
  }
}

void nom_short (volatile unsigned short *p)
{
  int val;

  while (scanf ("%d", &val) > 0) {
    *p = val;
  }
}

void nom_int (volatile unsigned int *p)
{
  while (scanf ("%d", p) > 0) {
    // Nothing!
  }
}

void nom_float (volatile float *p)
{
  while (scanf ("%f", p) > 0) {
    // Nothing!
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
  case BB_BYTE: nom_byte  (p);break;
  case BB_SHORT:nom_short (p);break;
  case BB_INT:  nom_int   (p);break;
  case BB_FLOAT:nom_float (p);break;
  case BB_BIT:  nom_bit   (p);break;
  }

  exit (0);
}
