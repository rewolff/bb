
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "bb_lib.h"

#define MAXVARS 0x80


int main (int argc, char **argv)
{
  struct bb_var *p; 
  FILE *cf, *board;
  int n;
  struct bb_var *vars[MAXVARS];
  int rhosts[MAXVARS];
  int rvarnrs[MAXVARS];
  int v;
  int i;
  int oldvals[MAXVARS];
  char buf[0x400];
  char varname [0x40];
  int rhost, rvarnr;
  

  bb_init ();
  n = 0;
  board = fopen (argv[1], "w");
  if (!board) {
    perror (argv[1]);
    exit (1);
  }
  cf = fopen (argv[2], "r");
  if (!cf) {
    perror (argv[2]);
    exit (1);
  }
  while (fgets (buf, 1024, cf) ){

    if (buf [0] == '#') continue;

    sscanf (buf, "%s %x:%x", varname, &rhost, &rvarnr);
    p = bb_get_handle (varname);
    if (p) {
      vars[n] = p;
      rhosts[n] = rhost;
      rvarnrs[n] = rvarnr;
      n++;
    } else {
      printf ("Can't find varname %s\n", varname);
      exit (1);
    }
  }
  
  for (i=0;i<n;i++) {
    fprintf (board, "ro %x %x\n", rhosts[i], rvarnrs[i]);
    usleep (300000);
  }

  while (1) {
    for (i=0;i<n;i++) {
      v = bb_get_int (vars[i]);
      if (oldvals[i] != v) {
	fprintf (board, "r%c %x %x\n", v?'s':'c', rhosts[i], rvarnrs[i]);
	oldvals[i] = v;
      }
    }
  }

  exit (0);
}
