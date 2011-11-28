

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
  enum bb_types type;
  struct logfile_header *logptr;
  int dt; 
  char logfname[0x400];
  int logf;
  long long fsize;

  bb_init ();
  if (argc <= 2) {
     fprintf (stderr, "usage: bb_createlog var dt [numsampes]\n");
     exit (1);
  }
  varname = argv[1];
  dt = atoi (argv[2]);
  if (argc > 3) {
    numsamples = atoi (argv[3]);
  }

  // check existance of varname... 
  vh = bb_get_handle (varname);
  if (!vh) {
     fprintf (stderr, "Variable %s doesn't exist", varname);
     exit (1);
  }
  type = bb_get_type (varname);
  tsize = bb_typesize (type);

  sprintf (logfname, "%s_log_%s", bb_get_base (), varname);

  logf = open (logfname, O_RDWR);
  if (logf < 0) {
    // Lets try to initialize it. 
    logf = open (logfname, O_RDWR | O_CREAT, 0666);
    fsize = sizeof(struct logfile_header) + numsamples * tsize;
    if (ftruncate (logf, fsize) < 0) {
       fprintf (stderr,"error truncating\n");
       exit (1);
    }
    //fstat (logf, &statb);
    logptr = mmap (NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, logf, 0);
    logptr->magic = BB_LOGFILE_MAGIC;
    logptr->datastart = sizeof (struct logfile_header);
    logptr->hdrversion = BB_LOG_HDRVERSION;
    logptr->dt = dt;
    logptr->numsamples = numsamples;
    logptr->curpos = 0;
  } else {
    close (logf);
    fprintf (stderr,"Log already exists");
    exit (1);
  }
  exit (0);
}
