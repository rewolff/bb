
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char **argv)
{
  int xorval, t;

  xorval = atoi (argv[1]); 
  while (scanf ("%d", &t) > 0) {
    printf ("%d\n", t ^ xorval); 
    fflush (stdout); 
  }
  exit (0);
}
