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
	char *fieldnp;
	int usecs = 1000000;
	int fd;
	int deltas = 0;
	int haveprevious = 0;
	double lastvalue = 0;
	int fieldno = -1;
	FILE * pipe_mnom;

	Myname = *Argv++;
	if (Argc < 4)
	{
		fprintf (stderr, "Usage: %s bb-name path var [+] [rate]\n"
			" or: %s bb-name path var#rank [+] [rate]\n",
			Myname, Myname);
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
	fieldnp = strchr (var, '#');
	if (fieldnp) {
	  if (sscanf (fieldnp+1, "%d", &fieldno) != 1) {
	    fprintf (stderr, "%s: bad fieldno in %s\n", *Argv, var);
	    exit (2);
	  }
	  *fieldnp = 0;
	} else {
	  fieldno = -1;
	}
#if 0	  
	if (var[1] == '#')
	{
		if (sscanf(var + 2, "%d", &fieldno) != 1)
		{
			fprintf (stderr, "%s: bad fieldno \"%s\"\n",
				Myname, *Argv + 1);
			exit (2);
		}
	}
#endif
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
	if (fd < 0)
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
		double value;

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

		// printf ("searching for var: \"%s\"\n", var);
		// printf ("readbuf = %p\n", readbuf);
		if (strlen (var) > 1) {
		  while (NULL != (where = strstr(where, var))) {
		    where += strlen(var);
		    if (! isalnum(*where))
		      break;
		  }
		  if (where == NULL)
		    {
		      if (var[0] == '\n') {
			var [0] = ' ';
			continue;
		      }
		      fprintf (stderr, "%s: cannot find var \"%s\" "
			       "in %s\n", Myname, var + 1, path);
		      exit (2);
		    }
		  if (! isspace(*where))
		    where++;	/* skip separator */
		} else {
		  where = readbuf;
		}
		// printf ("found var: %p %.20s\n", where, where);
		if (fieldno >= 0)
		{
			char field[sizeof(readbuf)];
			int currentfield;
			for (currentfield = 0; currentfield < fieldno;
				currentfield++)
			{
				int fieldlen;
				if (sscanf(where, "%s%n", field, &fieldlen)
					<= 0)
				{
					fprintf (stderr, "%s: cannot find "
						"field %d in %s\n",
						Myname, fieldno, readbuf);
					exit (2);
				}
				where += fieldlen;
			}
		}
		//printf ("found fieldno: %p >%.20s<\n", where, where);

		if (sscanf(where, "%lf", &value) != 1)
		{
			fprintf (stderr, "%s: cannot parse value \"%10s...\"\n",
				Myname, where);
			exit (2);
		}
		if (deltas == 0)
			fprintf (pipe_mnom, "%f\n", value);
		else if (haveprevious)
			fprintf(pipe_mnom, "%f\n", value - lastvalue);
		fflush (pipe_mnom);
		usleep(usecs);
		haveprevious = 1;
		lastvalue = value;
	}
}
