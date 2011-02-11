
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
  if (fd < 0) {
    perror ("open");
    exit (0);
  }

  if (ioctl (fd, PPEXCL, NULL) < 0) {
    perror ("ioctl PPEXCL");
  }
  if (ioctl (fd, PPCLAIM, NULL) < 0) {
    perror ("ioctl PPCLAIM");
  }

  while (scanf ("%d", &i) > 0) {
    ch = i; 
    if (ioctl (fd, PPWDATA, &ch) < 0) {
      perror ("ioctl PPWDATA");
    }
  }
  exit (0);
}
