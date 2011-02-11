
#include "bb_lib.h"
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}


int main (int argc, char **argv)
{
  char  *varname; 
  int flags; 
  char buf[0x100];
  int max, n;
  int idletime;
  struct bb_var *v;
  

  varname = argv[1];
  if (!varname)
    usage (); 

  bb_init (); 
  v = bb_get_handle (varname); 
  if (!v) 
    usage ();

  // XX Figure out max depending on the type. 
  max = 255; // for now don't increment beyond a byte. 
  
  flags = fcntl (0, F_GETFL, NULL);
  fcntl (0, F_SETFL, flags | O_NONBLOCK);

  idletime = 0; 

  while (1) {
    n = read (0, buf, 0x100);
    if (n == 0) break;

    if (errno == EWOULDBLOCK) errno = EAGAIN; // simplify following code. 

    if (n < 0) {
      if (errno == EAGAIN) {
	if (idletime < max)
	  idletime++;
      } else {
	perror ("read");
	// Can we continue??? What will happen next time?
      }
    } else {
      // n > 0 
      buf[n] = 0;
      if (strstr (buf, "bytes from")) {
	// That seems to be a ping response!
	idletime = 0; 
      } else {
	// possibly a host unreachable or no route to host. 
	if (idletime < max)
	  idletime++;
      }
    }

    bb_write_int (v, idletime);
    sleep (1);
  }
  exit (0);
}
