
#include "../includes/lib.h"

void on_error(int logPri, char *err) {
  syslog(logPri, "%s", err);
  exit(EXIT_FAILURE);
}

int ini_server(int port) {
  struct sockaddr_in server;
  int sockserv, opt_val = 1;

  syslog(LOG_INFO, "Creating socketServer");

  sockserv = socket(AF_INET, SOCK_STREAM, 0);
  if (sockserv < 0)
    on_error(LOG_ERR, "Could not create socket\n");

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  if (setsockopt(sockserv, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val) <
      0)
    on_error(LOG_ERR, "Error setsockopt\n");
  if (bind(sockserv, (struct sockaddr *)&server, sizeof(server)) < 0)
    on_error(LOG_ERR, "Could not bind socket\n");

  /*Procedemos a escuchar llamadas de clientes*/
  if (listen(sockserv, MAX_CLIENTS) < 0) {
    close(sockserv);
    on_error(LOG_ERR, "Error listening clients");
  }

  syslog(LOG_INFO, "Server is listening on %d", port);

  return sockserv;
}
int daemonizar(char *service) {

  int pid, i, fd;

  pid = fork();
  if (pid < 0)
    exit(EXIT_FAILURE);
  if (pid > 0)
    exit(EXIT_SUCCESS);

  // resetting File Creation Mask
  umask(0);
  setlogmask(LOG_UPTO(LOG_INFO));
  openlog(service, LOG_PID, LOG_LOCAL3);
  syslog(LOG_INFO, "Initiating_new_server.");

  if (setsid() < 0)
    on_error(LOG_ERR, "Error_creating_a_new_SID_for_the_child_process.");

  // Directorio raiz
  if ((chdir("/")) < 0)
    on_error(LOG_ERR, "Error_changing_the_current_working_directory_=_\"/\"");

  syslog(LOG_INFO, "Closing_standard_file_descriptors");

  // close all descriptors
  for (i = getdtablesize(); i >= 0; --i)
    close(i);

  fd = open("dev/null", O_RDWR, 0);

  if (fd != 1) {

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    if (fd > 2)
      close(fd);
  }

  syslog(LOG_INFO, "Daemon creado");
  closelog();

  return EXIT_SUCCESS;
}
int accept_conex(int sock) {

  int length, socketClient;
  struct sockaddr_in client;

  length = sizeof(struct sockaddr_in);

  // Accept connection from an incoming client
  if ((socketClient =
           accept(sock, (struct sockaddr *)&client, (socklen_t *)&length)) < 0)
    on_error(LOG_ERR, "Error accept conexion cliente");
  return socketClient;
}
void hostIp(int sock, char **host, char **ip) {
  char hbuf[1024], sbuf[20];
  struct sockaddr_in addr;
  struct hostent *he;
  int err;

  socklen_t addr_len = sizeof(addr);

  err = getpeername(sock, (struct sockaddr *)&addr, &addr_len);
  if (err != 0)
    return;

  *ip = inet_ntoa(addr.sin_addr);

  struct in_addr ipv4addr;
  inet_pton(AF_INET, *ip, &ipv4addr); // Google  "74.125.196.105"
  he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);

  if (he == NULL || h_errno == HOST_NOT_FOUND) {

    if (getnameinfo((struct sockaddr *)&addr, addr_len, hbuf, sizeof(hbuf),
                    sbuf, sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
      *host = hbuf;
    return;
  }
  *host = he->h_name;
  return;
}
