#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 4613

void test(void);

int main(void)
{
  test();
  return 0;
}

void test(void)
{
  int ret, fd;
  int cli_fd;
  char msg[2048];
  struct sockaddr_in cli_addr;

  memset(msg, 0, sizeof(msg));

  FILE *fp = fopen("cli_log.txt", "w");
  if (fp == NULL)
  {
    perror("fopen() failed\n");
    exit(EXIT_FAILURE);
  }

  cli_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (cli_fd == -1)
  {
    fprintf(fp, "%s\n", "socket() failed");
    perror("socket() failed\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp, "%s\n", "socket() created sucessfully");
  }

  cli_addr.sin_family = AF_INET;
  cli_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, IP, &cli_addr.sin_addr.s_addr);

  ret = connect(cli_fd, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr_in));
  if (ret == -1)
  {
    fprintf(fp, "%s\n", "connect() failed");
    perror("connect() failed\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    fprintf(fp, "%s\n", "connect() successful");
  }

  do
  {

    printf("client > ");
    fgets(msg, sizeof(msg), stdin);
    ret = strcspn(msg, "\n");
    msg[ret] = '\0';
    ret = strlen(msg);
    // printf("msg_len=%ld\n",strlen(msg));
    write(cli_fd, msg, ret);

    fd = open("list2.txt", O_RDWR | O_CREAT | O_TRUNC, 0755);
    ret = read(cli_fd, msg, sizeof(msg));
    // printf("read=%d\n",ret);
    write(fd, msg, ret);
    printf("server > %s\n", msg);
    memset(msg, 0, sizeof(msg));
    close(fd);

  } while (strcmp(msg, "Bye") != 0);
  close(cli_fd);
}
