#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

void test()
{

  int ret;
  int serv_fd, cli_fd;
  struct sockaddr_in serv_addr, cli_addr;
  char msg[100];

  FILE *fp = fopen("log.txt", "w");
  if (fp == NULL)
  {
    perror("fopen() failed");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp,"log.txt file created successfully\n");
    fflush(fp);
  }

  serv_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (serv_fd == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp,"socket() created successfully=%d\n", serv_fd);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(4613);
  inet_pton(AF_INET, "192.168.1.6", &serv_addr.sin_addr.s_addr);

  ret = bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (ret != 0)
  {
    perror("bind() failed");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp,"bind() successful\n");
  }

  ret = listen(serv_fd, 3);
  if (ret != 0)
  {
    perror("listen() failed");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp,"listen() successful\n");
  }

  memset(&cli_addr, 0, sizeof(cli_addr));
  socklen_t cli_len = sizeof(cli_addr);

  cli_fd = accept(serv_fd, (struct sockaddr *)&cli_addr, &cli_len);
  if (cli_fd == -1)
  {
    perror("accept() failed");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp,"accept() successful=%d\n", cli_fd);
  }

  do
  {

    ret = read(cli_fd, msg, sizeof(msg));
    if (ret == -1)
    {
      perror("read() failed");
      exit(EXIT_FAILURE);
    }
    msg[ret] = '\0';
    fprintf(fp, "client >%s\n",msg);
    printf("client > %s\n", msg);

    printf("server >");
    scanf("%99s", msg);
    fprintf(fp,"server > %s\n", msg);

    ret = write(cli_fd, msg, sizeof(msg));
    if (ret == -1)
    {
      perror("write() failed ");
      exit(EXIT_FAILURE);
    }
  } while (strncmp(msg,"Bye", 3) != 0);

  fprintf(fp,"socket closed\n");
  fclose(fp);
  close(cli_fd);
  close(serv_fd);
}
