#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8457
#define bzero(ptr, size) memset (ptr, 0, size)

/* Create a TCP connection to host and port.
* descriptor on success, -1 on error. */
int tcpconnect (char *host, int port)
{
	struct hostent *h;
	struct sockaddr_in sa;
	int s;
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
	char *user;
	char *host;
	char *message;
	int s;
	int nread;
	char buf[1024];
	/* Get the name of the host at which to finger from the end of the
	* command line argument. */
	if (argc == 3)
	{
		user = malloc (strlen (argv[1]));
		if (!user) 
		{
			fprintf (stderr, "out of memory\n");
			exit (1);
		}
		strcpy (user, argv[1]);
		host = malloc (strlen (argv[2]));
		if (!host) 
		{
			fprintf (stderr, "out of memory\n");
			exit (1);
		}
		strcpy (host, argv[2]);
	}
	else
	{
		user = host = NULL;
	}
	printf(user);
	printf("\n");
	printf(host);
	printf("\n");
	if (!host)
	{
		fprintf (stderr, "usage: %s user@host\n", argv[0]);
		exit (1);
	}
	
	printf("%s\n","Client is connecting");
	/* Try connecting to the host. */
	s = tcpconnect (host, PORT);
	if (s < 0)
	exit (1);
	printf("%s\n","Client is connected");
	/* Send the username to finger */
//	message = malloc(strlen("Джигурда cетевой с ментолом\0"));
//	strcpy(message, "Джигурда cетевой с ментолом\0");
	printf("%s\n","Sending message...");
	if (write (s, "Fuck the system\0", 16) < 0) 
	{
		perror ("Sending error");
		exit (1);
	}
	printf("%s\n","Message is sended. Wait fo answer...");
	printf("Server says: \n ");
	/* Now copy the result of the finger command to stdout. */
	while ((nread = read (s, buf, sizeof (buf))) > 0)
	write (1, buf, nread);
	//printf("%s\n", readline(s));
  exit (0);
}



