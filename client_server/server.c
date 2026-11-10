#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 4613

void test(void);
int check_cmd(char *);
void file_handel(int, char *, int);

int main()
{
  test();

  return 0;
}

void test()
{

  int ret, cmd;
  int serv_fd, cli_fd;
  struct sockaddr_in serv_addr, cli_addr;
  char msg[2048];
  FILE *fp = NULL;

  fp = fopen("log.txt", "w");
  if (fp == NULL)
  {
    perror("fopen() failed");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp, "log.txt file created successfully\n");
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
    fprintf(fp, "socket() created successfully=%d\n", serv_fd);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, IP, &serv_addr.sin_addr.s_addr);

  ret = bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (ret != 0)
  {
    perror("bind() failed");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp, "bind() successful\n");
  }

  ret = listen(serv_fd, 3);
  if (ret != 0)
  {
    perror("listen() failed");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp, "listen() successful\n");
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
    fprintf(fp, "accept() successful=%d\n", cli_fd);
  }

  do
  {
    memset(msg, 0, sizeof(msg));
    ret = read(cli_fd, msg, sizeof(msg));
    cmd = check_cmd(msg);

    switch (cmd)
    {
    case 1:
      file_handel(cli_fd, msg, sizeof(msg));
      break;
    case 0:
      write(cli_fd, "command not supported", 21);
      break;
    default:
      write(cli_fd, "Enter vaild command", 19);
      break;
    }
  } while (strncmp(msg, "Bye", 3) != 0);

  fprintf(fp, "socket closed\n");
  fclose(fp);
  close(cli_fd);
  close(serv_fd);
}

int check_cmd(char *msg)
{
  if (strcmp(msg, "client list") == 0)
    return 1;
  else
    return 0;
}

void file_handel(int cli_fd, char *msg, int size)
{
  int fd, ret;
  printf("client > %s\n", msg);

  chdir("/home/pratik2003/Downloads");
  system("ls -l > list.txt");
  fd = open("list.txt", O_RDONLY);

  while ((ret = read(fd, msg, size)) > 0)
  {
    write(cli_fd, msg, ret);
    printf("ret=%d\n", ret);
  }

  printf("server > list send\n");
  close(fd);
}
