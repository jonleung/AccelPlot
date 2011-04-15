#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <slipstream.h>

static int sock, length, n, i, cnt;
static struct sockaddr_in server, from;
static struct hostent *hp;

/* 
  This function can be used to send ACKed data.  The receiver should simply
  return 'A' if the checksum passes or 'N' if the checksum fails.

  The function will time out after 5 seconds if no data is received returning a
  failure code of 0.  1 is returned upon an ACK.
*/
int slipstream_acked_send(char *buf, uint8_t len, uint8_t retries )
{
uint16_t v,i,cnt;
time_t start;
char tmp_buf[MAX_BUF];

start=time(NULL);
cnt=0;
do {
	v=slipstream_send(buf,len);
	for(i=0; i<100; i++ )
	{
		v = slipstream_receive (tmp_buf);
		if(v==1 && tmp_buf[0]=='A' ) return 1;
		if(v==1 && tmp_buf[0]=='N' ) 
			{
			 cnt++;
			 v=slipstream_send(buf,len);
			 i=0;
			 if(cnt>retries) return 0;
			 if((time(NULL)-start)>5) return 0;
			}
		usleep(1000);
	}
} while((time(NULL)-start)<5 );
return 0;
}

int slipstream_open(char *addr, int port, int blocking_read)
{
  sock = socket (AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
	  {
    		perror ("socket");
		return 0;
	  }

  if(blocking_read==0)
	{
  	// make socket non-blocking...
  	fcntl (sock, F_SETFL, O_NONBLOCK);
	}
  
  server.sin_family = AF_INET;
  hp = gethostbyname (addr);
  if (hp == 0)
	  {
    		perror ("Unknown host");
		return 0;
	  }

  bcopy ((char *) hp->h_addr, (char *) &server.sin_addr, hp->h_length);
  server.sin_port = htons (port);
  length = sizeof (struct sockaddr_in);

return 1;
}

int slipstream_send(char *buf, int size)
{
int n;
    n = sendto (sock, buf, size, 0, (struct sockaddr *) &server, length);
    if (n < 0)
      {
      perror ("Sendto");
      return 0;
      }

return 1;
}

int slipstream_receive(char *buf)
{
int n;
    n = recvfrom (sock, buf, MAX_BUF, 0, (struct sockaddr *) &from, &length);
return n;
}

