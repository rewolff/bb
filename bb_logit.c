

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "bb_lib.h"

int interval = 100000; // in microseconds. 10x per second.

char *logurl;

void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}


void logit (char *s)
{
  char buf[0x200];
  FILE *fp;

  sprintf (buf,"lynx -source %s%s", logurl, s);
  fp = popen (buf,"r");
  while (fgets (buf, 0x200, fp)) {
     // debug: print it 
     printf ("got data: %s", buf);
  }
  pclose (fp);
}


void logit_i ( struct bb_var *v)
{
  char buf[0x40];
  sprintf(buf, "%s=%d",bb_get_name (v),  bb_get_int (v));
  logit (buf);
}


void logit_f ( struct bb_var *v)
{
  char buf[0x40];

  sprintf(buf, "%s=%f",bb_get_name (v),  bb_get_float (v));
  logit (buf);
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
	if (oldvals[i].i != bb_get_int (vars[i])) {
	  logit_i (vars[i]);
	  oldvals[i].i = bb_get_int (vars[i]);
	}
	break;
      case BB_FLOAT:
	if (oldvals[i].f != bb_get_float (vars[i])) {
	  logit_f (vars[i]);
	  oldvals[i].f = bb_get_float (vars[i]);
	}
	break;
      }
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

  logurl = argv[1];

  for (i=2;i<argc;i++) {
    var = argv[i];
    vartypes[i-2] = bb_get_type (var);
    vars[i-2] = bb_get_handle (var);
    if (vars[i-2] == NULL) {
      printf("Variable '%s' not found... Exiting.\n", var);
      exit(-1);
    }
  }
  nvars = i-2;

  mon_varlist (nvars, vartypes, vars);

  exit (0);
}
