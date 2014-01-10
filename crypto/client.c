#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define FINGER_PORT 79
#define bzero(ptr, size) memset (ptr, 0, size)

/* Create a TCP connection to host and port.
* descriptor on success, -1 on error. */
int tcpconnect (char *host, int port)
{
	struct hostent *h;
	struct sockaddr_in sa;
	int s;
//	Returns a file
	/* Get the address of the host at which to finger from the
	* hostname. */
	h = gethostbyname (host);
	if (!h || h->h_length != sizeof (struct in_addr))
	{
		fprintf (stderr, "%s: no such host\n", host);
		return -1;
	}
	/* Create a TCP socket. */
	s = socket (AF_INET, SOCK_STREAM, 0);
	/* Use bind to set an address and port number for our end of the
	* finger TCP connection. */
	bzero (&sa, sizeof (sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons (0);
	/* tells OS to choose a port */
	sa.sin_addr.s_addr = htonl (INADDR_ANY); /* tells OS to choose IP addr */
	if (bind (s, (struct sockaddr *) &sa, sizeof (sa)) < 0) 
	{
		perror ("bind");
		close (s);
		return -1;
	}
	/* Now use h to set set the destination address. */
	sa.sin_port = htons (port);
	sa.sin_addr = *(struct in_addr *) h->h_addr;
	/* And connect to the server */
	if (connect (s, (struct sockaddr *) &sa, sizeof (sa)) < 0) 
	{
		perror (host);
		close (s);
		return -1;
	}
	return s;
}

int
main (int argc, char **argv)
{
	char *user;
	char *host;
	int s;
	int nread;
	char buf[1024];
	/* Get the name of the host at which to finger from the end of the
	* command line argument. */
	if (argc == 2)
	{
		user = malloc (1 + strlen (argv[1]));
		if (!user) 
		{
			fprintf (stderr, "out of memory\n");
			exit (1);
		}
		strcpy (user, argv[1]);
		host = strrchr (user, '@');
	}
	else
	user = host = NULL;
	if (!host)
	{
		fprintf (stderr, "usage: %s user@host\n", argv[0]);
		exit (1);
	}
	*host++ = '\0';
	/* Try connecting to the host. */
	s = tcpconnect (host, FINGER_PORT);
	if (s < 0)
	exit (1);
	/* Send the username to finger */
	if (write (s, user, strlen (user)) < 0 || write (s, "\r\n", 2) < 0) 
	{
		perror (host);
		exit (1);
	}
	/* Now copy the result of the finger command to stdout. */
	while ((nread = read (s, buf, sizeof (buf))) > 0)
	write (1, buf, nread);
	exit (0);
}



