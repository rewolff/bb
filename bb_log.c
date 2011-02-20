

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
  dt = atoi (argv[2]);
  if (argc > 3) {
    numsamples = atoi (argv[3]);
  }

  vh = bb_get_handle (varname); 
  vptr = bb_get_ptr (varname);

  type = bb_get_type (varname);
  tsize = bb_typesize (type);
  
  sprintf (logfname, "%s_log_%s", bb_get_base (), varname);

  logf = open (logfname, O_RDWR);
  if (logf < 0) {
    // Lets try to initialize it. 
    logf = open (logfname, O_RDWR | O_CREAT, 0666);
    fsize = sizeof(struct logfile_header) + numsamples * tsize;
    ftruncate (logf, fsize);
    //fstat (logf, &statb);
    logptr = mmap (NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, logf, 0);
    dataptr = (void *)(logptr + 1);
    logptr->magic = BB_LOGFILE_MAGIC;
    logptr->datastart = sizeof (struct logfile_header);
    logptr->hdrversion = 1;
    logptr->dt = dt;
    logptr->numsamples = numsamples;
    logptr->curpos = 0;
  } else {
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
    if (logptr->hdrversion != 1) fatal ("incompatible header");
    if (logptr->dt != dt) fatal ("incompatible interval");
    if (logptr->numsamples != numsamples) fatal ("incompatible numsamples");
  }

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
      write (logf, vptr, tsize);
      sleep (dt);
    }
  }



}
