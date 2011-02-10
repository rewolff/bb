
#include "bb_lib.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char **argv)
{
  char  *varname; 
  int flags; 
  char buf[0x100];
  int max, n;
  int idletime;
  struct bb_var *v;
  

  varname = argv[1];
  bb_init (); 
  v = bb_get_handle (varname); 

  // XX Figure out max depending on the type. 
  max = 255; // for now don't increment beyond a byte. 
  
  flags = fcntl (0, F_GETFL, NULL);
  fcntl (0, F_SETFL, flags | O_NONBLOCK);

  idletime = 0; 

  while (1) {
    n = read (0, buf, 0x100);
    if (n == 0) break;

    if (errno == EAGAIN) errno = EWOULDBLOCK; // simplify following code. 

    if ((n < 0) && 
	(errno == EWOULDBLOCK)) {
      if (idletime < max)
	idletime++;
    } else {
      idletime = 0; 
    }

    bb_write_int (v, idletime);
    sleep (1);
  }
  exit (0);
}
