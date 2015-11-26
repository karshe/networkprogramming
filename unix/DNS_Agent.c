/******************************
 * WHERE IS GOOGLE.COM
 * ****************************/

#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char*argv[]) {
  char *host = "www.google.com";
  struct hostent *h=gethostbyname(host);
  if (h) 
   printf("GOOGLE.COM LIVES HERE : %s\n", inet_ntoa(*(struct in_addr*)h->h_addr)); /* DNS! */
  else 
   printf("%s\n", hstrerror(h_errno));
  return 0;
}
