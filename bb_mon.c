
// test rew

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


void mon_bit (volatile unsigned char *p)
{
  int old = -1;

  while (1) {
    if (old != bb_getbitval(p)) {
      printf ("%d\n", *p);
      fflush (stdout); 
      old = bb_getbitval(p);
    }
    usleep (interval);
  }
}

void mon_byte (volatile unsigned char *p)
{
  int old = -1;

  while (1) {
    if (old != *p) {
      printf ("%d\n", *p);
      fflush (stdout); 
      old = *p;
    }
    usleep (interval);
  }
}


void mon_short (volatile unsigned short *p)
{
  int old = -1;

  while (1) {
    if (old != *p) {
      printf ("%d\n", *p);
      fflush (stdout); 
      old = *p;
    }
    usleep (interval);
  }
}


void mon_int (volatile unsigned int *p)
{
  unsigned int old = -1;

  while (1) {
    if (old != *p) {
      printf ("%d\n", *p);
      fflush (stdout); 
      old = *p;
    }
    usleep (interval);
  }
}


void mon_float (volatile float *p)
{
  float old = -1;

  while (1) {
    if (old != *p) {
      printf ("%f\n", *p);
      fflush (stdout); 
      old = *p;
    }
    usleep (interval);
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
  case BB_BYTE: mon_byte (p);break;
  case BB_SHORT:mon_short (p);break;
  case BB_INT:  mon_int (p);break;
  case BB_FLOAT:mon_float (p);break;
  }

  exit (0);
}
