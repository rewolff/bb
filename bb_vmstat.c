#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

const char * Myname;
#define MAXBUF	0x10000	/* 64 kBytes */

int main (int Argc, const char * Argv[])
{
	const char * bbname;
	const char * path;
	const char * var;
	int usecs = 1000000;
	int fd;
	int deltas = 0;
	int haveprevious = 0;
	int lastvalue = 0;

	Myname = *Argv++;
	if (Argc < 4)
	{
		fprintf (stderr, "Usage: %s bb-name path var [+] [rate]\n",
			Myname);
		exit (1);
	}
	bbname = *Argv++;
	path = *Argv++;
	var = *Argv++;
	if ((*Argv != NULL) && (strcmp (*Argv, "+") == 0))
	{
		deltas = 1;
		Argv++;
	}
	if ((*Argv != NULL) && (sscanf(*Argv, "%d", &usecs) != 1))
	{
		fprintf (stderr, "%s: Bad rate \"%s\"\n", Myname, *Argv);
		exit (1);
	}
	else
		Argv++;
	fd = open(path, O_RDONLY);
	if (fd == EOF)
	{
		fprintf (stderr, "%s: cannot open file \"%s\": %s\n",
			Myname, path, strerror(errno));
		exit (2);
	}
	while (1)
	{
		char readbuf[MAXBUF + 1];
		int got;
		char * where;
		int value;

		if (lseek(fd, 0, SEEK_SET) != 0)
		{
			fprintf (stderr, "%s: seek failed: %s\n",
				Myname, strerror(errno));
			exit (2);
		}
		got = read (fd, readbuf, sizeof(readbuf) - 1);
		if (got <= 0)
		{
			fprintf (stderr, "%s: read failed.\n", Myname);
			exit (2);
		}
		readbuf[got] = '\0';
		where = strstr(readbuf, var);
		if (where == NULL)
		{
			fprintf (stderr, "%s: cannot find var \"%s\" in %s\n",
				Myname, var, path);
			exit (2);
		}
		where += strlen(var);
		if (sscanf(where, "%d", &value) != 1)
		{
			fprintf (stderr, "%s: cannot parse value \"%10s...\"\n",
				Myname, where);
			exit (2);
		}
		if (deltas == 0)
			printf ("%d\n", value);
		else if (haveprevious)
			printf("%d\n", value - lastvalue);
		usleep(usecs);
		haveprevious = 1;
		lastvalue = value;
	}
}
