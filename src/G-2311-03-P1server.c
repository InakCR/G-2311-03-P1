#include "../includes/G-2311-03-P1server.h"

long ncliente = 0;
int port = 0;
static struct option long_options[] = {{"help", no_argument, NULL, 'h'},
                                       {"ssl", no_argument, NULL, 's'},
                                       {"port", required_argument, NULL, 'p'},
                                       {NULL, 0, NULL, 0}};
int main(int argc, char *argv[]) {

  int socketServer, socketClient, err, max;
  pthread_t ptCliente[MAX_CLIENTS];
  struct timeval timeout;
  pthread_attr_t attr;
  fd_set readfds;
  char ch;

  while ((ch = getopt_long(argc, argv, "h:s:", long_options, NULL)) != -1) {
    switch (ch) {
    case 'h':
      printf("Los argumentos reconocidos son:\n --ssl para activar la "
             "seguridad SSL\n --help para informacion\n");
      break;
    case 's':
      break;
    case 'p':
      port = atoi(optarg);
      printf("Escuchando puerto %d\n", port);
      break;
    case '?':
      printf(
          "Argumento no reconocido, pruebe con --help paraa mas informacion\n");
      break;
    }
  }
  // Crecion de proceso Daemon
  if (daemonizar("REDES2") < 0)
    on_error(LOG_ERR, "Error daemonizar");
  if (port == 0)
    port = PORT;
  socketServer = ini_server(PORT);

  // Apertura del socket
  if (socketServer < 0)
    on_error(LOG_ERR, "Error iniciar_servidor");

  // Accept and incoming connection
  syslog(LOG_INFO, "Esperando conexiones de clientes");
  ncliente = 0;
  max = MAX_CLIENTS;
  // setMotd("*BIENVENIDO AL SERVIDOR*");
  while (1) {

    // Preparando select
    FD_ZERO(&readfds);
    FD_SET(socketServer, &readfds);

    timeout.tv_sec = STIEMPO;
    timeout.tv_usec = MSTIEMPO;

    if (select(max + 1, &readfds, NULL, NULL, NULL) < 0)
      on_error(LOG_ERR, "Error en Select");

    syslog(LOG_INFO, "Nuevo Cliente");

    // Accept
    socketClient = accept_conex(socketServer);
    // Tiempo de espera a una peticion

    // hilo conexion cliente, atendemos al cliente en el hilo
    pthread_attr_init(&attr);
    err = pthread_create(&ptCliente[ncliente], &attr, deal_cliente,
                         (void *)(intptr_t)socketClient);
    if (err < 0)
      on_error(LOG_ERR, "Error al crear hilo cliente");
    ncliente++;
  }

  return 0;
}

void *deal_cliente(void *sock) {
  char *userNick = NULL;
  int socket;

  socket = *((int *)(&sock));

  while (1) {

    if (recibir(socket, &userNick) == -1) {
      break;
    }
  }
  return (void *)1;
}

/*Función que recibe un comando del cliente y realiza la acción
   correspondiente
   Recibe el socket al que escuchar como argumento*/
int recibir(int sock, char **userNick) {

  char command[BUFFER_SIZE] = "";
  char *pipe, *pipeCommand;
  int n_command = 1;

  /*Recibimos el comando*/
  if (recv(sock, command, BUFFER_SIZE, 0) == 0) {
    close(sock);
    return -1;
  }

  syslog(LOG_INFO, "Comando recibido: %s", command);

  /*Se llama a IRC_UnPipelineCommands*/

  if ((pipe = IRC_UnPipelineCommands(command, &pipeCommand)) != NULL) {

    syslog(LOG_INFO, "Llega Bloque de Comandos:");
    syslog(LOG_INFO, "Comando nº%d", n_command++);
    do_command(pipeCommand, sock, userNick);

    while ((pipe = IRC_UnPipelineCommands(pipe, &pipeCommand)) != NULL) {

      syslog(LOG_INFO, "Comando nº%d", n_command++);
      do_command(pipeCommand, sock, userNick);
    }

    if (pipe == NULL) {

      bzero(command, BUFFER_SIZE);
      syslog(LOG_INFO, "Comando nº%d", n_command++);
      do_command(pipeCommand, sock, userNick);
    }

  } else {

    if (pipeCommand == NULL) {
      syslog(LOG_INFO, "Se han buscado comandos en cadena vacia.");
      return -1;
    } else {

      syslog(LOG_INFO, "Comando simple:");
      do_command(pipeCommand, sock, userNick);
    }
  }

  //  free(pipeCommand);
  //  free(pipe);

  return 1;
}

long get_numero_clientes() { return ncliente; }
