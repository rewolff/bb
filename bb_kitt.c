

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (int argc, char **argv)
{
  int i, n, t, delay; 


  n = 8;
  delay = 100000;

  while (1) {
    t = 1; 
    for (i=0;i<n;i++) {
      printf ("%d\n", t);
      fflush (stdout);
      t = t << 1;
      usleep (delay);
    }
    for (i=0;i<n;i++) {
      t = t >>  1;
      printf ("%d\n", t);
      fflush (stdout);
      usleep (delay);
    }
  }
  exit (0);
}
