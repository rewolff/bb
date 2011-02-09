
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "bb_lib.h"


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

  char *fname; 
  FILE *fp; 
  char buf[0x400];
  struct bb_var *t;
  char varname [0x100], vartypes[0x100];
  int offset, fd;
  struct stat statb;
 
  bb_init_base ();
  fname = malloc (0x400);

  fp = bb_open_names ("r");
  while (fgets (buf, 0x400,  fp)) {
    sscanf (buf, "%s %x %s", varname, &offset, vartypes);

    t = malloc (sizeof (struct bb_var));
    t->name = strdup (varname);
    t->offset = offset;
    t->type  = bb_typestring_to_enum (vartypes); 
    t->next = varlist; 
    varlist = t;
    if (firstfree < offset + bb_typesize (t->type))
      firstfree = offset + bb_typesize (t->type);
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
  return t->type;
}


void *bb_get_ptr (char *name)
{
  struct bb_var *t;

  t = bb_find_var (name);
  if (t->type == BB_BIT)
    return (void *) t->offset;
  return bb_baseptr +  t->offset;
}


int bb_getbitval (void *p)
{
  long t;
  t = (long) p;

  return ((((unsigned char *) bb_baseptr) [t/8]) >> (t%8)) & 1;
}


void bb_setbit (void *p)
{
  long t;
  t = (long) p;

  ((unsigned char *) bb_baseptr) [t/8] |=  1 << (t%8);
}


void bb_clrbit (void *p)
{
  long t;
  t = (long) p;

  ((unsigned char *) bb_baseptr) [t/8] &= 0xff- (1 << (t%8));
}


void bb_writebit (void *p, int val)
{
  if (val)
    bb_setbit (p);
  else
    bb_clrbit (p);
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
  ftruncate (fd, firstfree);
  close (fd);
}


enum bb_errno bb_overlay_var (char *name, int type, char *where, int offset)
{
  struct bb_var *t, *w;
  FILE *fp;
  int fd; 

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
  fprintf (fp, "%s %x %s\n", 
	   name, t->offset, bb_type_enum_to_string (type));
  fclose (fp);
  return BB_OK;
}



