#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>

#define MAXBUF	0x10000	/* 64 kBytes */
#define BB_MNOM	"bb_mnom %s"	/* %s will be bb-name of variable */

const char * Myname;
int main (int Argc, const char * Argv[])
{
	const char * bbname;
	const char * path;
	char * var;
	char * pipe_command;
	int usecs = 1000000;
	int fd;
	int deltas = 0;
	int haveprevious = 0;
	int lastvalue = 0;
	FILE * pipe_mnom;

	Myname = *Argv++;
	if (Argc < 4)
	{
		fprintf (stderr, "Usage: %s bb-name path var [+] [rate]\n",
			Myname);
		exit (1);
	}
	bbname = *Argv++;
	path = *Argv++;
	var = calloc(strlen(*Argv) + 2, 1);
	if (NULL == var)
	{
		fprintf (stderr, "%s: calloc failed \"%s\"\n",
			Myname, strerror(errno));
		exit (2);
	}
	sprintf (var, "\n%s", *Argv);
	Argv++;
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
	pipe_command = calloc(strlen(BB_MNOM) + strlen(bbname), 1);
	if (NULL == pipe_command)
	{
		fprintf (stderr, "%s: calloc failed \"%s\"\n",
			Myname, strerror(errno));
		exit (2);
	}
	sprintf(pipe_command, BB_MNOM, bbname);
	pipe_mnom = popen(pipe_command, "w");
	if (NULL == pipe_mnom)
	{
		fprintf (stderr, "%s: popen \"%s\"failed \"%s\"\n",
			Myname, pipe_command, strerror(errno));
		exit (2);
	}
	while (1)
	{
		char readbuf[MAXBUF + 1];
		int got;
		char * where = readbuf;
		int value;

		if (lseek(fd, 0, SEEK_SET) != 0)
		{
			fprintf (stderr, "%s: seek failed: %s\n",
				Myname, strerror(errno));
			exit (2);
		}
		/* put a newline at the start of the buffer to simplify 
		 * searching for our variable
		 */
		readbuf[0] = '\n';
		got = read (fd, readbuf + 1, sizeof(readbuf) - 2);
		if (got <= 0)
		{
			fprintf (stderr, "%s: read failed.\n", Myname);
			exit (2);
		}
		readbuf[got] = '\0';
		while (NULL != (where = strstr(where, var)))
		{
			if (where == NULL)
			{
				fprintf (stderr, "%s: cannot find var \"%s\" "
					"in %s\n", Myname, var, path);
				exit (2);
			}
			where += strlen(var);
			if (! isalnum(*where))
				break;
		}
		if (! isspace(*where))
			where++;	/* skip separator */
		if (sscanf(where, "%d", &value) != 1)
		{
			fprintf (stderr, "%s: cannot parse value \"%10s...\"\n",
				Myname, where);
			exit (2);
		}
		if (deltas == 0)
			fprintf (pipe_mnom, "%d\n", value);
		else if (haveprevious)
			fprintf(pipe_mnom, "%d\n", value - lastvalue);
		fflush (pipe_mnom);
		usleep(usecs);
		haveprevious = 1;
		lastvalue = value;
	}
}
