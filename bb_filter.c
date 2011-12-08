

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "bb_lib.h"

int interval = 1000; // ms
int averagelength = 10;


void do_average (struct bb_var *p, struct bb_var *r)
{
  double *values;
  float val;
  int i, n;
  double tot = 0;
  
  values = malloc (sizeof (double) * averagelength);
  i = 0;
  while (1) {
    if (i >= averagelength)
      tot -= values[i%averagelength];
    val = bb_get_float (p);
    values[i%averagelength] = val;
    tot += val;
    i++;
    if (i > averagelength) n = averagelength;
    else                   n = i;
    bb_write_float (r, tot / n);
    usleep (interval * 1000);
  }
}

void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}




static char *short_opts = "i:a:";
static struct option long_opts[] = {
  {"interval",                    1, NULL, 'i'},
  {"averagelength",               1, NULL, 'a'},
};


int main (int argc, char **argv)
{
  struct bb_var *p, *r;
  char *var, *outvar;
  int c; 

  bb_init ();

  if (argc < 2) usage (); 

  while (1) {
    c = getopt_long (argc, argv, short_opts, long_opts, NULL);
    if (c == -1) break;
    switch (c) {
    case 'i':sscanf (optarg, "%d", &interval);break;
    case 'a':sscanf (optarg, "%d", &averagelength);break;
    default:
      fprintf (stderr,"getopt returned unexpected char: 0x%02x\n", c);
      exit (1);
      break;
    }
  }
  if (optind < argc) 
    var = argv[optind++];
  else {
    fprintf (stderr, "no varname\n");
    exit (1);
  }

  if (optind < argc) 
    outvar = argv[optind++];
  else {
    outvar = malloc (strlen (var) + 10);
    sprintf (outvar, "%s_avg", var);
  }
 
  p = bb_get_handle (var);
  if (p == NULL) {
    fprintf (stderr, "Variable '%s' not found... Exiting.\n", var);
    exit(1);
  }

  r = bb_get_handle (outvar);
  if (r == NULL) {
    fprintf (stderr, "Variable '%s' not found.\n", outvar);
    exit (1);
  }

  do_average (p, r);
  exit (0);
}
