

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>



#include "bb_lib.h"
#include "bb_logs.h"

void fatal (char *s)
{
  fprintf (stderr, "%s\n", s);
  exit (0);
}


int main (int argc, char **argv)
{
  int numsamples;
  char *varname; 
  float dt; 

  bb_init ();
  if (argc <= 2) {
     fprintf (stderr, "usage: bb_createlog var dt [numsampes]\n");
     exit (1);
  }

  varname = argv[1];
  dt = atof (argv[2]);
  if (argc > 3) {
    numsamples = atoi (argv[3]);
  } else
    numsamples = 0;

  bb_createlog (varname, dt, numsamples);
  exit (0);
}
