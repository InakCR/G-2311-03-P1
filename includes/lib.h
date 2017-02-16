#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <redes2/irc.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdarg.h>
#include <memory.h>
#include <stropts.h>
#include <sys/resource.h>
#include <arpa/inet.h> //inet_addr
#include <fcntl.h>
#include <sys/select.h>
#include <pthread.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 20
#define MAX_MNS 2000
#define NUM_BYTES 8192

void on_error(int log,char* err);
int ini_server(int port);
int daemonizar(char* service);
