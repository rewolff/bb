

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include "bb_lib.h"

#define BB_LOGFILE_MAGIC (*((int *)"BBLF"))


struct logfile_header {
  int magic;
  int datastart;
  int hdrversion;
  int dt;
  int numsamples;
  int curpos;
  char pad[0x200 - 0x18];
};


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
  

  bb_init ();

  varname = argv[1];

  // vh = bb_get_handle (varname); 
  // vptr = bb_get_ptr (varname);

  type = bb_get_type (varname);
  tsize = bb_typesize (type);
  
  sprintf (logfname, "%s_log_%s", bb_get_base (), varname);

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
