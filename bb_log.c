

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "bb_lib.h"
#include "bb_logs.h"


void fatal (char *s)
{
  fprintf (stderr, "%s\n", s);
  exit (0);
}


int main (int argc, char **argv)
{
  int numsamples = 0;
  char *varname; 
  int tsize;
  struct logfile_header *logptr;
  void *vptr;
  void *dataptr;
  enum bb_types type;
  int dt; 
  int logf;
  int lognum;
  
  bb_init ();

  varname = argv[1];

  if (argc > 2) {
    lognum = atoi (argv[2]);
  } else {
    lognum = 0;
  }
  printf ("btst1\n");

  logptr = bb_openlog (varname, lognum, &logf);
  if (!logptr) {
    exit(1);
  }
  printf ("btst2 %p\n", logptr);

  type = bb_get_type (varname);
  tsize = bb_typesize (type);
  vptr = bb_get_ptr (varname);
  printf ("btst3 vptr = %p (%s)\n", vptr, varname);

  dataptr = (void *)(logptr + 1);
  printf ("btst4\n");
  dt = logptr->dt;
  printf ("btst5\n");
  numsamples = logptr->numsamples;
  printf ("btst6\n");

#define CHAR_LOG_DATA ((char *)dataptr)
#define CHAR_VAR_PTR  ((char *)vptr) 

#define FLOAT_LOG_DATA ((float *)dataptr)
#define FLOAT_VAR_PTR  ((float *)vptr) 

  printf ("logptr: %p, dataptr: %p, vptr: %p\n", 
  	  logptr, dataptr, vptr);
  if (numsamples) {
    // cyclic logging. Use the memory mapped file... 
    while (1) {
      switch (type) {
      case BB_BYTE:  CHAR_LOG_DATA[logptr->curpos] = *CHAR_VAR_PTR; break;
      case BB_FLOAT: FLOAT_LOG_DATA[logptr->curpos] = *FLOAT_VAR_PTR; break;
#if 0
      case BB_SHORT: 
      case BB_INT:   
      case BB_BIT:   return 1;
#endif
      }
      if (logptr->curpos >= numsamples-1) 
	logptr->curpos = 0;
      else 
	logptr->curpos += 1;
      usleep (dt);
    }
  } else {
    
    lseek (logf, 0, SEEK_END);
    while (1) {
      if (write (logf, vptr, tsize) < 0) {
        fprintf (stderr, "error writing to log\n");
        exit (1);
      }
      usleep (dt);
    }
  }
}
