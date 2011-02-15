

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "bb_lib.h"



void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}



void nom_varlist (int nvars, int *vartypes, struct bb_var *vars[])
{
  int i, n;
  int ti;
  float tf;
  char *buf, *p; 

  buf = malloc (nvars * 16); 
  printf ("Buf=%p\n", buf);
  while (fgets (buf, nvars * 16, stdin) ) {
    p = buf; 
    for (i=0;i<nvars;i++) {
      switch (vartypes[i]) {
      case BB_BYTE: 
      case BB_SHORT:
      case BB_INT:  
      case BB_BIT: 
	sscanf (p, "%d%n", &ti, &n);
	p += n;
	bb_write_int (vars[i], ti);
	break;
      case BB_FLOAT: 
	sscanf (p, "%f%n", &tf, &n);
	p += n;
	bb_write_float (vars[i], tf);
	break;
      }
    }
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

  nom_varlist (nvars, vartypes, vars);

  exit (0);
}
