#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8457
#define FINGER_COMMAND "/usr/bin/finger"
#define bzero(ptr, size) memset (ptr, 0, size)

/* Create a TCP socket, bind it to a particular port, and call listen
* for connections on it. These are the three steps necessary before
* clients can connect to a server. */
int tcpserv (int port)
{
	int s, n;
	struct sockaddr_in sin;
	/* The address of this server */
	bzero (&sin, sizeof (sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons (port);
	/* We are interested in listening on any and all IP addresses this
	* machine has, so use the magic IP address INADDR_ANY. */
	sin.sin_addr.s_addr = htonl (INADDR_ANY);
	s = socket (AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		perror ("socket");
		return -1;
	}
	/* Allow the program to run again even if there are old connections
	* in TIME_WAIT. This is the magic you need to do to avoid seeing
	* "Address already in use" errors when you are killing and
	* restarting the daemon frequently. */
	n = 1;
	if (setsockopt (s, SOL_SOCKET, SO_REUSEADDR, (char *)&n, sizeof (n)) < 0) 
	{
		perror ("SO_REUSEADDR");
		close (s);
		return -1;
	}
	/* This function sets the close-on-exec bit of a file descriptor.
	* That way no programs we execute will inherit the TCP server file
	* descriptor. */
	fcntl (s, F_SETFD, 1);
	if (bind (s, (struct sockaddr *) &sin, sizeof (sin)) < 0) 
	{
		fprintf (stderr, "TCP port %d: %s\n", port, strerror (errno));
		close (s);
		return -1;
	}
	if (listen (s, 5) < 0) 
	{
		perror ("listen");
		close (s);
		return -1;
	}
	return s;
}

/* Read a line of input from a file descriptor and return it. Returns
* NULL on EOF/error/out of memory. May over-read, so don’t use this
* if there is useful data after the first line. */
static char *readline (int s)
{
	char *buf = NULL, *nbuf;
	int buf_pos = 0, buf_len = 0;
	int i, n;
	for (;;) 
	{
		/* Ensure there is room in the buffer */
		if (buf_pos == buf_len) 
		{
			buf_len = buf_len ? buf_len << 1 : 4;
			nbuf = realloc (buf, buf_len);
			if (!nbuf) 
			{
				free (buf);
				return NULL;
			}
		buf = nbuf;
		}
		/* Read some data into the buffer */
		n = read (s, buf + buf_pos, buf_len - buf_pos);
		if (n <= 0) 
		{
			if (n < 0)
				perror ("read");
			else
				fprintf (stderr, "read: EOF\n");
			free (buf);
			return NULL;
		}
		/* Look for the end of a line, and return if we got it. Be
		* generous in what we consider to be the end of a line. */
		for (i = buf_pos; i < buf_pos + n; i++)
			if (buf[i] == '\0' || buf[i] == '\r' || buf[i] == '\n') 
			{
				buf[i] = '\0';
				return buf;
			}
		buf_pos += n;
	}
}

int main (int argc, char **argv)
{
	int ss, cs;
	struct sockaddr_in sin;
	int sinlen;
	char *message;
	int pid;
	/* This system call allows one to call fork without worrying about
	* calling wait. Don’t worry about what it means unless you start
	* caring about the exit status of forked processes, in which case
	* you should delete this line and read the manual pages for wait
	* and waitpid. For a description of what this signal call really
	* does, see the manual page for sigaction and look for
	* SA_NOCLDWAIT. Signal is an older signal interface which when
	* invoked this way is equivalent to setting SA_NOCLDWAIT. */
	//signal (SIGCHLD, SIG_IGN);
	printf("Server started\n");
	ss = tcpserv (PORT);
	if (ss < 0)
	exit (1);
	for (;;) 
	{
		sinlen = sizeof (sin);
		cs = accept (ss, (struct sockaddr *) &sin, &sinlen);
		if (cs < 0) 
		{
			perror ("accept");
			exit (1);
		}
		printf ("connection from %s\n", inet_ntoa (sin.sin_addr));
		printf("Getting message...\n");
		message = readline(cs);
		printf("Message was got.");
		printf("Client says:\n%s\n", message);
		if (write (cs, message, sizeof(message)) < 0) 
		{
			perror ("resend error");
			exit (1);
		}
		printf("Goodbye\n");
		close (cs);
	}
}

