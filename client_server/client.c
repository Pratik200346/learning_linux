#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void test(void);

int main(void)
{
 test();
 return 0;
}

void test(void)
{
  int ret;
  int cli_fd;
  char msg[100];
  struct sockaddr_in cli_addr;

  FILE *fp = fopen("cli_log.txt","w");
  if(fp == NULL)
  {
   perror("fopen() failed\n");
   exit(EXIT_FAILURE);
  } 

  cli_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(cli_fd == -1)
  {
    fprintf(fp,"%s\n","socket() failed");
    perror("socket() failed\n");
    exit(EXIT_FAILURE);
  }
  else {
   fprintf(fp,"%s\n","socket() created sucessfully");
  }  

  cli_addr.sin_family = AF_INET;
  cli_addr.sin_port = htons(4613);
  inet_pton(AF_INET,"192.168.1.6",&cli_addr.sin_addr.s_addr);
 
   ret = connect(cli_fd,(struct sockaddr *)&cli_addr,sizeof(struct sockaddr_in));
   if(ret == -1)
   {
     fprintf(fp,"%s\n","connect() failed");
     perror("connect() failed\n");
     exit(EXIT_FAILURE); 
   }
   else {
     fprintf(fp,"%s\n","connect() successful");
   }
   
   do{
      printf("client >");
      scanf("%99s",msg);

      ret = write(cli_fd, msg, sizeof(msg));
      if(ret == -1) {
       fprintf(fp,"%s\n","write() failed");
       perror("write() failed\n");
       exit(EXIT_FAILURE);
      }	 
      else {
       fprintf(fp,"%s\n","write() successful");
      }
      ret = read(cli_fd,msg,sizeof(msg));
      if(ret == -1) {
	fprintf(fp,"%s\n","read() failed");
        perror("read() failed\n");
        exit(EXIT_FAILURE);
      }
      else {
       fprintf(fp,"%s\n","read() successful");
      }
       
      printf("server > %s\n",msg);
 
   }while(strncmp(msg,"bye\n",4) != 0);
   close(cli_fd);
}

