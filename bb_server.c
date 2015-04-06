

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include "bb_lib.h"


int server_socket (int port)
{
  int sock;
  int yes=1;
  int sinlen;
  struct sockaddr_in sin;

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock < 0) {
    perror("socket()");
    return -1;
  }

  bzero(&sin, sizeof(struct sockaddr_in));
  sin.sin_family = AF_INET;
  sin.sin_port   = htons(port);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);
  sinlen = sizeof(struct sockaddr_in);

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  if(bind(sock, (struct sockaddr *)&sin, sinlen) < 0) {
    perror("bind()");
    close (sock);
    return -1;
  }
  if(listen(sock, 1) < 0) {
    perror("listen()");
    close (sock);
    return -1;
  }
  return sock;
}


static void sigchld_handler(int num)
{
  sigset_t set, oldset;
  pid_t pid;
  int status, exitstatus;

  /* block other incoming SIGCHLD signals */
  sigemptyset(&set);
  sigaddset(&set, SIGCHLD);
  sigprocmask(SIG_BLOCK, &set, &oldset);

  /* wait for child */
  while((pid = waitpid((pid_t)-1, &status, WNOHANG)) > 0) {
    if(WIFEXITED(status)) {
      exitstatus = WEXITSTATUS(status);

      fprintf(stderr, 
	      "parent: child %d exited, exit status = %d\n", 
	      (int)pid, exitstatus);
    } else if(WIFSIGNALED(status)) {
      exitstatus = WTERMSIG(status);

      fprintf(stderr,
	      "parent: child %d terminated by signal %d\n",
	      (int)pid, exitstatus);
    } else if(WIFSTOPPED(status)) {
      exitstatus = WSTOPSIG(status);
      fprintf(stderr,
	      "parent: child %d stopped by signal %d\n",
	      (int)pid, exitstatus);
    } else {
      fprintf(stderr,
	      "parent: child %d exited magically\n",
	      (int)pid);
    }
  }

  /* re-install the signal handler (some systems need this) */
  signal(SIGCHLD, sigchld_handler);
  
  /* and unblock it */
  sigemptyset(&set);
  sigaddset(&set, SIGCHLD);
  sigprocmask(SIG_UNBLOCK, &set, &oldset);
}



#define INTERVAL 100000
#define PINGTIME 30

#define MAX_VARS 100

int main (int argc, char **argv)
{
  int port;
  int sockfd;
  socklen_t sinlen;
  struct sockaddr_in sin;
  int net; 

  int i, nv, rv;
  int flags, pid;
  char *peername;

  struct bb_var *vars[MAX_VARS];
  char *names[MAX_VARS];
  int oldvals[MAX_VARS];

  char buf[0x100], *p;
  int interval = INTERVAL;
  int newv, pingtime;
  char cmd [0x20], val[0x20];

  bb_init ();
  port = atoi (argv[1]);

  signal(SIGCHLD, sigchld_handler);

  sockfd = server_socket (port);

  pingtime = PINGTIME * 1000000/interval;

  while(1) {
    sinlen = sizeof (struct sockaddr_in);
    net = accept(sockfd, (struct sockaddr *)&sin, &sinlen);
    if(net < 0) {
      perror("accept()");
      continue;
    }
    /* information */
    sinlen = sizeof (struct sockaddr_in);
    if(getpeername(net, (struct sockaddr *)&sin, &sinlen) < 0) {
      perror("getpeername()");
    } else {
      peername = inet_ntoa(sin.sin_addr);
      fprintf(stderr, "Connect from %s\n", peername);
    }
    pid = fork ();
    if (pid == 0) {
      // serve the socket. 
      flags = fcntl (net, F_GETFL, NULL);
      fcntl (net, F_SETFL, flags | O_NONBLOCK);
      nv = 0;
      while ( 1) {
	rv = read (net, buf, 256);
	if (rv == 0) break;
	if ((rv < 0) &&
	    (errno != EAGAIN) &&
	    (errno != EWOULDBLOCK)) {
	  // hmm error. 
	  perror ("net read");
	  break;
	}
	if (rv > 0) {
	  buf[rv] = 0;
	  printf ("got something from the client: %s", buf);

	  for (p=buf;p && *p;) {
	    sscanf (p, "%s %s", cmd, val);
	    if (strcmp (cmd, "ping") == 0) {
	      printf ("got a ping. Good.\n");
	    } else if (strcmp (cmd, "uid") == 0) {
	      printf ("XXX todo: dosomething with the uid.\n");
	    } else if (strcmp (cmd, "mon") == 0) {
	      names[nv] = strdup (val);
	      vars[nv] = bb_get_handle (val);
	      if (vars[nv] == NULL) {
		printf ("Variable %s not found! config error!!!\n", val);
	      } else {
		oldvals[nv] = -1;
		nv++;
	      }
	    }
	    p = strchr (p, '\n')+1;
	  }
	}
	if (pingtime <= 0) {
	  pingtime = PINGTIME * 1000000 / interval;
	  sprintf (buf, "pong\n");
	  printf ("Sending ping: %s", buf); 
	  if (write(net, buf, strlen (buf)) < 0) {
	    perror ("write net");
	    exit (-1);
	  }
	} else {
	  pingtime--;
	}
	for (i=0;i<nv;i++) {
	  newv = bb_get_int(vars[i]);
	  if (newv != oldvals[i]) {
	    sprintf (buf, "output %s %d\n", 
		     names[i], newv);
	    printf ("Sending: %s", buf); 
	    if (write(net, buf, strlen (buf)) < 0) {
	      perror ("write net");
	      exit (-1);
	    }
	    oldvals[i] = newv;
	  }
	}
	usleep (interval);
      }
      exit (0);
    } else {
      // parent: close the (copy of the) net                                                                                                                                                                            socket and go back to accepting connections. 
      close (net);
    }
  }
  exit (0);
}
