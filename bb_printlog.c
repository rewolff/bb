

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
  int tsize, p, i;
  struct logfile_header *logptr;
  void *dataptr;
  enum bb_types type;
  char logfname[0x400];
  int logf;
  long long fsize;
  struct stat statb;
  int lognr;
  
  bb_init ();

  if (argc <= 1) {
    fprintf(stderr, "usage: bb_printlog varname\n");
    exit (1);
  }
  varname = argv[1];
 
  if (argc > 2) {
    lognr = atoi (argv[2]);
  } else {
    lognr = 0;
  }

  type = bb_get_type (varname);
  tsize = bb_typesize (type);
  
  sprintf (logfname, "%s_logs/%s_log%d", bb_get_base (), varname, lognr);

  logf = open (logfname, O_RDONLY);
  if (logf < 0) {
      perror (logfname);
      exit (1);
  } 
  
  // The logfile already exist!
  fstat (logf, &statb);
  fsize = statb.st_size;
  logptr = mmap (NULL, fsize, PROT_READ, MAP_SHARED, logf, 0);
  if ((long) logptr == -1) {
    perror ("mmap");
    exit (1);
  }
  dataptr = (void *)(logptr + 1);
  if (logptr->magic != BB_LOGFILE_MAGIC) fatal ("no magic");
  if (logptr->hdrversion != 1) fatal ("incompatible header");

  numsamples = logptr->numsamples;

#define CHAR_LOG_DATA ((char *)dataptr)
#define CHAR_VAR_PTR  ((char *)vptr) 

#define FLOAT_LOG_DATA ((float *)dataptr)
#define FLOAT_VAR_PTR  ((float *)vptr) 

  //printf ("logptr: %p, dataptr: %p, vptr: %p\n", 
  //	  logptr, dataptr, vptr);
  if (numsamples) {
    // cyclic logging. start at curpos. 
    p = logptr->curpos;
  } else {
    p = 0;
    numsamples = (fsize - sizeof (struct logfile_header)) / tsize;
  }
    
  for (i=0;i<numsamples;i++) {
    switch (type) {
    case BB_BYTE:  printf ("%d\n", CHAR_LOG_DATA[p]);break;
    case BB_FLOAT: printf ("%f\n", FLOAT_LOG_DATA[p]);break;
#if 0
    case BB_SHORT: 
    case BB_INT:   
    case BB_BIT:   return 1;
#endif
    }
    
    p++;
    if (p >= numsamples) p = 0;
  }
  exit (0);
}
