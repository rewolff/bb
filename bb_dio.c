
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bb_lib.h"

int interval = 100000;

void usage (void)
{
  fprintf (stderr, "Usage error \n");
  exit (1);
}


int main (int argc,char **argv)
{ 
  int i, old, new; 
  char *var;
  struct bb_var *p;

  if (argc != 2) usage (); 
  
  var = argv[1];

  bb_init ();
  if (bb_get_type (var) != BB_INT) usage (); 

  p = bb_get_handle (var);

  old = ~bb_get_int (p); // Always different!
  while (1) {
    new = bb_get_int (p);
    if (old != new) {
      for (i =0;i<0x10; i++) {
	if ((old ^ new) & (1 << i)) {
	  printf ("%c%x\n", (new & (1 << i))?'h':'l', i);
	  fflush (stdout); 
	}
	usleep (10000);
      }
      old = new;
    }
    usleep (interval);
  }
}
