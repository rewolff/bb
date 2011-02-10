

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


#include "bb_lib.h"

int interval = 100000; // in microseconds. 10x per second.


void usage (void)
{
  fprintf (stderr, "usage error.\n");
  exit (1);
}


void do_alarm (void)
{
  system (alarm_command);
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


void alarm_float (struct bb_var *p)
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

  bb_init ();

  var = argv[1];
  if (!var) usage (); 
 
  vartype = bb_get_type (var);
  p = bb_get_handle (var);

  switch (vartype) {
  case BB_BIT: 
  case BB_BYTE: 
  case BB_SHORT:
  case BB_INT:  alarm_int (p, min, max);break;
  case BB_FLOAT:alarm_float (p, min, max);break;
  }

  exit (0);
}
