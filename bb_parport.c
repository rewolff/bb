
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>


int main (int argc, char **argv)
{
  char *dev; 
  int fd, i; 
  unsigned char ch; 

  dev = argv[1];

  fd = open (dev, O_RDWR); 
  ioctl (fd, PPEXCL, NULL); 
  ioctl (fd, PPCLAIM, NULL); 

  while (scanf ("%d", &i) > 0) {
    ch = i; 
    ioctl (fd, PPWDATA, &ch); 
  }
  exit (0);
}
