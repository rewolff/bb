

#include <stdio.h>
#include <stdlib.h>
#include <usb.h>
#include <ftdi.h>
#include <getopt.h>
#include <unistd.h>



void ftdi_fatal (struct ftdi_context *ftdi, char *str)
{
  fprintf (stderr, "%s: %s\n", 
           str, ftdi_get_error_string (ftdi));
  exit (1); 
}


int main (int argc, char **argv)
{
  struct ftdi_context ftdic;
  int t;
  unsigned char data[16]; /* some extra space */ 
  int deviceid = 0x6001;
  int delay = 100000; /* 100 thousand microseconds: 1 tenth of a second */
  int verbose = 0;

  while ((t = getopt (argc, argv, "d:D:v")) != -1) {
    switch (t) {
    case 'd':
      delay = atoi (optarg);
      break;
    case 'D':
      sscanf(optarg, "%i", &deviceid);
      break;
    case 'v':
      verbose++;
      break;
    }
  }
  
  ftdi_init(&ftdic);
  
  if (verbose) 
    printf("Attempting to open device pid=%x hex\n", deviceid );

  if (ftdi_usb_open(&ftdic, 0x0403, deviceid) < 0)
    ftdi_fatal (&ftdic, "Can't open ftdi device %x");

#ifdef OLD
  if (ftdi_enable_bitbang(&ftdic, 0xFF) < 0)
    ftdi_fatal (&ftdic, "Can't enable bitbang"); 
#else
  if (ftdi_set_bitmode (&ftdic, 0xff, BITMODE_BITBANG))
    ftdi_fatal (&ftdic, "Can't enable bitbang"); 
#endif

  if (verbose) printf("Device %x openend OK\n", deviceid);

  while (scanf ("%d", &t) > 0) {
     data[0] = t;
     data[1] = t;
     ftdi_write_data(&ftdic, data, 1);
     if(ftdi_write_data(&ftdic, data, 1) < 0) {
        fprintf(stderr,"write failed for 0x%x: %s\n", 
                data[0], ftdic.error_str);
     }
     usleep(delay);
  }

  ftdi_usb_close(&ftdic);
  ftdi_deinit(&ftdic);
  exit (0);
}

