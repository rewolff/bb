

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
  struct bb_var *vh;
  int tsize;
  struct logfile_header *logptr;
  void *vptr;
  void *dataptr;
  enum bb_types type;
  int dt; 
  char logfname[0x400];
  int logf;
  long long fsize;
  struct stat statb;
  

  bb_init ();

  varname = argv[1];

  vh = bb_get_handle (varname); 
  vptr = bb_get_ptr (varname);

  type = bb_get_type (varname);
  tsize = bb_typesize (type);
  
  sprintf (logfname, "%s_log_%s", bb_get_base (), varname);

  logf = open (logfname, O_RDWR);
  if (logf < 0) {
    fprintf (stderr, "No logfile for %s\n", varname);
    exit (1);
  }
  // The logfile already exist!
  fstat (logf, &statb);
  fsize = statb.st_size;
  logptr = mmap (NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, logf, 0);
  if ((long) logptr == -1) {
    perror ("mmap");
    exit (1);
  }
  dataptr = (void *)(logptr + 1);
  if (logptr->magic != BB_LOGFILE_MAGIC) fatal ("no magic");
  if (logptr->hdrversion != BB_LOG_HDRVERSION) fatal ("incompatible header");
  dt = logptr->dt;
  numsamples = logptr->numsamples;


#define CHAR_LOG_DATA ((char *)dataptr)
#define CHAR_VAR_PTR  ((char *)vptr) 

#define FLOAT_LOG_DATA ((float *)dataptr)
#define FLOAT_VAR_PTR  ((float *)vptr) 

  //printf ("logptr: %p, dataptr: %p, vptr: %p\n", 
  //	  logptr, dataptr, vptr);
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
      logptr->curpos += 1;
      if (logptr->curpos >= numsamples) 
	logptr->curpos = 0;
      sleep (dt);
    }
  } else {
    lseek (logf, 0, SEEK_END);
    while (1) {
      if (write (logf, vptr, tsize) < 0) {
        fprintf (stderr, "error writing to log\n");
        exit (1);
      }
      sleep (dt);
    }
  }
}
