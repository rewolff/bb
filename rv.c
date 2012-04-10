
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "bb_lib.h"

struct var {
  char *name;
  struct bb_var *bbv;
  int oldval;
  int raddr, onum;  
  struct var *next;
};

struct var *vars;



void readconfig (char *fname)
{
  FILE *fp;
  char buf[1024];
  char varname [0x40], lr[0x40];
  struct var *v;
  int p1, p2;

  fp = fopen (fname, "r");
  while (fgets (buf, 1024, fp)) {
    sscanf (buf, "%s %s %x %x", lr, varname, &p1, &p2);

    // printf ("%s has type %d.\n", varname, bb_get_type (varname));
    switch (bb_get_type (varname)) {
    case BB_BYTE: 
    case BB_SHORT:
    case BB_INT:  
    case BB_BIT:   
      // Thats ok!
      break;
    default:
      // float / unknown type. 
      fprintf (stderr, "float type for %s not allowed. \n", varname);
      //exit (1);
      break;
    }

    v = malloc (sizeof (struct var));
    v -> next = vars;
    v -> name = strdup (varname);
    v -> bbv = bb_get_handle (varname);
    // printf ("%s: got handle: %p\n", varname, v->bbv);
    v -> oldval = -1;
    if (strcmp (lr, "local") == 0) {
      v->raddr = -1;
      v->onum = p1;
    } else {
      v->raddr = p1;
      v->onum = p2;
    }
    vars = v;
  }
}

void monitor_variables (void)
{
  struct var *v;
  int nv;

  while (1) {
    for (v = vars; v!= NULL;v=v->next) {
      nv = bb_get_int (v->bbv);
      fprintf (stderr, "%s: %d->%d.\n", v->name, v->oldval, nv);
      if (v->oldval != nv) {
	if (v->raddr == -1) { // local
	  printf ("%c %x\n", 
		  nv?'s':'c', v->onum);
	} else {
	  printf ("r%c %x %x\n", 
		  nv?'s':'c', v->raddr, v->onum);
	}
	v->oldval = nv;
	usleep (100000);
      }
    }
    usleep (1000000);
  }
}



int main (int argc, char **argv)
{
  bb_init ();

  readconfig (argv[1]);
  monitor_variables ();

  exit (0);
}
