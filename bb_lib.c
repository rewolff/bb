
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "bb_lib.h"
#include "bb_logs.h"


char *typestrings [] = {
  "byte", 
  "short", 
  "int", 
  "float", 
  "bit",
};


struct bb_var {
  char *name;
  long offset;
  enum bb_types type;
  struct bb_var *next;
};
 
static struct bb_var *varlist = NULL;
static int firstfree;
static char *bb_base;
static void *bb_baseptr;


enum bb_types bb_typestring_to_enum (char *s)
{
  int i;

  for  (i=0;i<sizeof (typestrings)/sizeof(typestrings[0]); i++) {
    if (strcmp (s, typestrings[i]) == 0) return (enum bb_types) i;
  }
  return BB_INVALID;
}


char * bb_type_enum_to_string (enum bb_types t)
{
   return typestrings[t]; 
}


struct bb_var *bb_find_var (char *s)
{
  struct bb_var *t;

  for (t = varlist; t != NULL ; t = t->next) {
    if (strcmp (t->name, s) == 0) return t;
  }
  return NULL;
}


int bb_typesize (enum bb_types t)
{
  switch (t) {
  case BB_INVALID:return -1;
  case BB_BYTE:  return 1;
  case BB_SHORT: return 2;
  case BB_INT:   return 4;
  case BB_FLOAT: return 4;
  case BB_BIT:   return 1;
  }
  return -1;
}


void bb_init_base (void)
{
  bb_base = getenv ("BB_BASE");
  if (!bb_base) {
     bb_base = malloc (0x400);
     sprintf (bb_base, "%s/.bb", getenv ("HOME"));
  }
}

char *bb_get_base (void)
{
  return bb_base;
}


FILE *bb_open_names (char *mode)
{
  char fname[0x400];
  FILE *fp; 

  sprintf (fname, "%s_names", bb_base);
  
  fp = fopen (fname, mode);
  if (!fp) {
    // Can't open. We need to initialize... 
    sprintf (fname, "%s_shm", bb_base);
    creat (fname, 0666);
    
    sprintf (fname, "%s_names", bb_base);
    creat (fname, 0666);
    fp = fopen (fname, mode);
  }
  return fp;
}


int bb_open_shm (int mode)
{
  char fname[0x400];
  int fd; 

  sprintf (fname, "%s_shm", bb_base);
  
  fd = open (fname, mode);
  if (!fd) {
    // Can't open. We need to initialize... 
    return -1;
  }
  return fd;
}


void bb_init ()
{
  //char *fname; 
  FILE *fp; 
  char buf[0x400];
  struct bb_var *t;
  char varname [0x100], vartypes[0x100];
  int offset, fd, boffset;
  struct stat statb;
 
  bb_init_base ();
  //fname = malloc (0x400);

  fp = bb_open_names ("r");
  while (fgets (buf, 0x400,  fp)) {
    sscanf (buf, "%s %x %s", varname, &offset, vartypes);

    t = malloc (sizeof (struct bb_var));
    t->name = strdup (varname);
    t->offset = offset;
    t->type  = bb_typestring_to_enum (vartypes); 
    t->next = varlist; 
    varlist = t;
    if (t->type == BB_BIT) boffset = offset/8;
    else                   boffset = offset; 
    if (firstfree < boffset + bb_typesize (t->type))
      firstfree = boffset + bb_typesize (t->type);
  }
  fclose (fp);

  fd = bb_open_shm (O_RDWR);
  fstat (fd, &statb);
  bb_baseptr = 
    mmap (NULL, statb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close (fd);

}

int bb_get_type (char *name)
{
  struct bb_var *t;

  t = bb_find_var (name);
  if (t == NULL)
  {
    return BB_INVALID;
  }

  return t->type;
}




void *bb_get_ptr (char *name)
{
  struct bb_var *t;

  t = bb_find_var (name);
  if (t->type == BB_BIT)
    return NULL;
  return bb_baseptr + t->offset;
}


struct bb_var *bb_get_handle (char *name)
{
  return bb_find_var (name);
}


int bb_get_bit_val (struct bb_var *v)
{
  int t;

  t = v->offset;
  return ((((unsigned char *) bb_baseptr) [t/8]) >> (t%8)) & 1;
}


void bb_set_bit (struct bb_var *v)
{
  int t;

  t = v->offset;
  ((unsigned char *) bb_baseptr) [t/8] |= 0x00 + (1 << (t%8));
}


void bb_clr_bit (struct bb_var *v)
{
  int t;

  t = v->offset;
  ((unsigned char *) bb_baseptr) [t/8] &= 0xff - (1 << (t%8));
}


void bb_write_bit (struct bb_var *v, int val)
{
  if (val)
    bb_set_bit (v);
  else
    bb_clr_bit (v);
}


void bb_write_int (struct bb_var *v, int val)
{
  switch (v->type) {
  case BB_BIT:   bb_write_bit (v, val);break;
  case BB_BYTE:  *((unsigned char  *) (bb_baseptr+v->offset)) = val;break;
  case BB_SHORT: *((unsigned short *) (bb_baseptr+v->offset)) = val;break;
  case BB_INT:   *((unsigned int   *) (bb_baseptr+v->offset)) = val;break;
  case BB_FLOAT: *((         float *) (bb_baseptr+v->offset)) = val;break;
  case BB_INVALID:break;
  }
}



void bb_write_float (struct bb_var *v, float val)
{
  switch (v->type) {
  case BB_BIT:   bb_write_bit (v, val);break;
  case BB_BYTE:  *((unsigned char  *) (bb_baseptr+v->offset)) = val;break;
  case BB_SHORT: *((unsigned short *) (bb_baseptr+v->offset)) = val;break;
  case BB_INT:   *((unsigned int   *) (bb_baseptr+v->offset)) = val;break;
  case BB_FLOAT: *((         float *) (bb_baseptr+v->offset)) = val;break;
  case BB_INVALID:break;
  }
}


int bb_get_int (struct bb_var *v)
{
  switch (v->type) {
  case BB_BIT:   return bb_get_bit_val (v);
  case BB_BYTE:  return *((unsigned char  *) (bb_baseptr+v->offset));
  case BB_SHORT: return *((unsigned short *) (bb_baseptr+v->offset));
  case BB_INT:   return *((unsigned int   *) (bb_baseptr+v->offset));
  case BB_FLOAT: return *((         float *) (bb_baseptr+v->offset));
  case BB_INVALID:break;
  }
  return -1;
}

float bb_get_float (struct bb_var *v)
{
  switch (v->type) {
  case BB_BIT:   return bb_get_bit_val (v);
  case BB_BYTE:  return *((unsigned char  *) (bb_baseptr+v->offset));
  case BB_SHORT: return *((unsigned short *) (bb_baseptr+v->offset));
  case BB_INT:   return *((unsigned int   *) (bb_baseptr+v->offset));
  case BB_FLOAT: return *((         float *) (bb_baseptr+v->offset));
  case BB_INVALID:break;
  }
  return -1;
}


void bb_create_var (char *name, int type)
{
  struct bb_var *t; 
  FILE *fp;
  int fd; 

  t = malloc (sizeof (struct bb_var));
  t->name = name; 
  t->type = type; 
  t->offset = firstfree;
  t->next = varlist;
  varlist = t; 

  firstfree += bb_typesize (type);
  
  fp = bb_open_names ("a");
  fprintf (fp, "%s %lx %s\n", 
	   name, t->offset, bb_type_enum_to_string (type));
  fclose (fp);


  fd = bb_open_shm (O_RDWR);
  if (ftruncate (fd, firstfree) < 0) {
    perror ("ftruncate");
    exit (1);
  }
  close (fd);
}


enum bb_errno bb_overlay_var (char *name, int type, char *where, int offset)
{
  struct bb_var *t, *w;
  FILE *fp;

  w = bb_find_var (where);
  if (!w) return BB_ENOTFOUND; 
  // XXX check validity of offset. 

  t = malloc (sizeof (struct bb_var));
  t->name = name;
  t->type = type;
  if (type == BB_BIT) 
    t->offset = 8 * w->offset + offset;
  else
    t->offset = w->offset + offset;
  t->next = varlist;
  varlist = t; 

  fp = bb_open_names ("a");
  fprintf (fp, "%s %lx %s\n", 
	   name, t->offset, bb_type_enum_to_string (type));
  fclose (fp);
  return BB_OK;
}



struct bb_var *bb_get_next_var(struct bb_var *p)
{
  if (p) return p->next;
  else   return varlist;
}


char *bb_get_name(struct bb_var *p)
{
  return p->name;
}


int bb_createlog (char *varname, float dt, int numsamples)
{
  struct bb_var *vh;
  int tsize;
  enum bb_types type;
  struct logfile_header *logptr;
  char logfname[0x400];
  int logf;
  long long fsize;
  int t;
  int tt, dtus;

  // check existance of varname... 
  vh = bb_get_handle (varname);
  if (!vh) {
     fprintf (stderr, "Variable %s doesn't exist", varname);
     return -1;
  }
  type = bb_get_type (varname);
  tsize = bb_typesize (type);

  t = 0;
  tt = 0;
  do {
    sprintf (logfname, "%s_logs/%s_log%d", bb_get_base (), varname, t++);
    logf = open (logfname, O_RDWR | O_CREAT | O_EXCL, 0666);
    if ((tt == 0) && (errno == ENOENT)) {
      sprintf (logfname, "%s_logs", bb_get_base ());
      mkdir (logfname, 0777);
      tt = 1;
      t = 0;
      continue;
    }
    if ((logf < 0) && (errno != EEXIST)) {
      fprintf (stderr, "can't create logfile\n");
      return -1;
    }
  } while (logf < 0);

  // Lets try to initialize it. 
  fsize = sizeof(struct logfile_header) + numsamples * tsize;
  if (ftruncate (logf, fsize) < 0) {
    fprintf (stderr,"error truncating\n");
    exit (1);
  }
  dtus = dt * 1000000;
  //fstat (logf, &statb);
  logptr = mmap (NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, logf, 0);
  logptr->magic = BB_LOGFILE_MAGIC;
  logptr->datastart = sizeof (struct logfile_header);
  logptr->hdrversion = BB_LOG_HDRVERSION;
  logptr->dt = dtus;
  logptr->numsamples = numsamples;
  logptr->curpos = 0;
  return 0;
}



struct logfile *bb_openlog (char *varname, int lognum, int *logfp)
{
  struct bb_var *vh;
  // int tsize;
  struct logfile_header *logptr;
  // void *vptr;
  // enum bb_types type;
  char logfname[0x400];
  int logf;
  long long fsize;
  struct stat statb;
  struct logfile *lf;

  vh = bb_get_handle (varname); 
  if (!vh) {
    fprintf (stderr, "cant find variable %s\n", varname);
    return NULL;
  }
  // vptr = bb_get_ptr (varname);
  
  // type = bb_get_type (varname);
  // tsize = bb_typesize (type);
  
  sprintf (logfname, "%s_logs/%s_log%d", bb_get_base (), varname, lognum);

  logf = open (logfname, O_RDWR);
  if (logf < 0) {
    fprintf (stderr, "No logfile for %s_%d\n", varname, lognum);
    return (NULL);
  }

  fstat (logf, &statb);
  fsize = statb.st_size;
  logptr = mmap (NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, logf, 0);
  if ((long) logptr == -1) {
    perror ("mmap");
    return (NULL);
  }
  //  dataptr = (void *)(logptr + 1);
  if (logptr->magic != BB_LOGFILE_MAGIC) {
    fprintf (stderr, "no magic");
    return NULL;
  }
  if (logptr->hdrversion != BB_LOG_HDRVERSION) {
    fprintf (stderr, "incompatible header");
    return (NULL);
  }

  lf = malloc (sizeof (struct logfile));

  lf->lfh = logptr;
  lf->fd  = logf;
  return lf;
}
