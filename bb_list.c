


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "bb_lib.h"

int main (int argc, char **argv)
{
  struct bb_var *p; 
  char *name; 
  enum bb_types type; 

  bb_init (); 
  for (p = bb_get_next_var (NULL); p != NULL; p = bb_get_next_var (p)) {
    name = bb_get_name (p);
    type = bb_get_type (name); 
    printf ("%7s %s\n", bb_type_enum_to_string (type), name);
  }
  exit (0);
}
