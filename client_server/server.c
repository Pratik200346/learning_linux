#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void test(void);

int main()
{
  test(); 
 
 return 0;
}

void test () {
 
 int ret;
 int serv_fd, cli_fd;
 struct sockaddr_in serv_addr, cli_addr;
 
 serv_fd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
 
 if(serv_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  } else {
    printf("socket() created successfully=%d\n",serv_fd);
  }
 
  memset(&serv_addr,0,sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(4613);
  inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr.s_addr);

  ret = bind(serv_fd, (struct sockaddr *)&serv_addr,sizeof(serv_addr));
  if(ret != 0) {
    perror("bind() failed");
    exit(EXIT_FAILURE);
  } else {
    printf("bind() successful\n");
  }   

  ret = listen(serv_fd, 3);
  if (ret != 0) {
     perror("listen() failed");
     exit(EXIT_FAILURE);
  } else {
    printf("listen() successful\n");
  }

  memset(&cli_addr, 0, sizeof(cli_addr));
  socklen_t cli_len = sizeof(cli_addr);

  cli_fd = accept(serv_fd,(struct sockaddr *)&cli_addr,&cli_len);
  if(ret == -1) {
     perror("accept() failed");
     exit(EXIT_FAILURE);
  } else {
    printf("accept() successful=%d\n",cli_fd);
  }
  
}
