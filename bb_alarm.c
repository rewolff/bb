

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "bb_lib.h"

static int interval = 100000; // in microseconds. 10x per second.
static char *alarm_command;


void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}


void do_alarm (void)
{
  int rv; // To silence the compiler. 

  // this program cannot do anything useful with the return value. If
  // the user decides to supply us with an error-returning alarm
  // command we cannot become verbose and report that as an error.

  if (alarm_command) 
    rv = system (alarm_command);
  else 
    printf ("Alarm!");
}


void alarm_int (struct bb_var *p, int min, int max)
{
  int val;
  int alarm_sent;

  alarm_sent = 0;
  while (1) {
    val = bb_get_int (p);
    if ((val < min) | 
	(val > max)) {
      if (!alarm_sent) do_alarm (); 
      alarm_sent = 1;
    } else {
      alarm_sent = 0;
    }
    usleep (interval);
  }
}


void alarm_float (struct bb_var *p, float min, float max)
{
  float val;
  int alarm_sent;

  alarm_sent = 0;
  while (1) {
    val = bb_get_float (p);
    if ((val < min) | 
	(val > max)) {
      if (!alarm_sent) do_alarm (); 
      alarm_sent = 1;
    } else {
      alarm_sent = 0;
    }
    usleep (interval);
  }
}



int main (int argc, char **argv)
{
  int vartype; 
  void *p;
  char *var;
  int i;
  int imin, imax;
  float fmin, fmax; 

  bb_init ();

  if (argc <= 3) usage ();

  var = argv[1];

  vartype = bb_get_type (var);
  p = bb_get_handle (var);

  alarm_command = malloc (0x400);
  alarm_command[0] = 0;
  for (i=4;i<argc;i++) {
    strcat (alarm_command, argv[i]);
    strcat (alarm_command, " ");
  }
  
  switch (vartype) {
  case BB_BIT: 
  case BB_BYTE: 
  case BB_SHORT:
  case BB_INT:
    sscanf (argv[2], "%d", &imin);
    sscanf (argv[3], "%d", &imax);
    alarm_int (p, imin, imax);break;
    break;
  case BB_FLOAT:
    sscanf (argv[2], "%f", &fmin);
    sscanf (argv[3], "%f", &fmax);
    alarm_float (p, fmin, fmax);break;
    break;
  }    

  exit (0);
}
