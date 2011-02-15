

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


void mon_varlist (int nvars, int *vartypes, struct bb_var *vars[])
{
  int i;
  union intfloat {
    int i;
    float f; 
  };
  union intfloat *oldvals;

  oldvals   = malloc (sizeof (union intfloat) * nvars);

  while (1) {
    for (i=0;i<nvars;i++) {
      switch (vartypes[i]) {
      case BB_BYTE: 
      case BB_SHORT:
      case BB_INT:  
      case BB_BIT: 
	if (oldvals[i].i != bb_get_int (vars[i])) i = nvars + 1;
	break;
      case BB_FLOAT:
	if (oldvals[i].f != bb_get_float (vars[i])) i = nvars + 1;
	break;
      }
    }
    // if all are the same, we'll come here with i == nvars. 
    if (i > nvars) {
      // Some changed! print them all!
      for (i=0;i<nvars;i++) {
	switch (vartypes[i]) {
	case BB_BYTE: 
	case BB_SHORT:
	case BB_INT:  
	case BB_BIT:   
	  oldvals[i].i =  bb_get_int (vars[i]);
	  printf ("%d ", oldvals[i].i);
	  break;
	case BB_FLOAT: 
	  oldvals[i].f =  bb_get_float (vars[i]);
	  printf ("%f ", oldvals[i].f);
	  break;
	}
      }
      printf ("\n");
      fflush (stdout);
    }
    usleep (interval); 
  }
}


int main (int argc, char **argv)
{
  struct bb_var *vars[100];
  int vartypes[100];
  char *var;
  int i, nvars;

  bb_init ();

  for (i=1;i<argc;i++) {
    var = argv[i];
    vartypes[i-1] = bb_get_type (var);
    vars[i-1] = bb_get_handle (var);
    if (vars[i-1] == NULL) {
      printf("Variable '%s' not found... Exiting.\n", var);
      exit(-1);
    }
  }
  nvars = i-1;

  mon_varlist (nvars, vartypes, vars);

  exit (0);
}
