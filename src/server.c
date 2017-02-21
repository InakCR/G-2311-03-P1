#include "../includes/server.h"

#define STIEMPO 30
#define MSTIEMPO 30000

int main(int argc, char *argv[]) {
  int socketServer, socketClient, err, ncliente, max;
  fd_set readfds;
  struct timeval timeout;
  pthread_t ptCliente[MAX_CLIENTS];
  pthread_attr_t attr;
  // Crecion de proceso Daemon
  if (daemonizar("REDES2") < 0)
    on_error(LOG_ERR, "Error daemonizar");

  socketServer = ini_server(PORT);

  // Apertura del socket
  if (socketServer < 0)
    on_error(LOG_ERR, "Error iniciar_servidor");

  // Accept and incoming connection
  syslog(LOG_INFO, "Esperando conexiones de clientes");
  ncliente = 0;
  max = MAX_CLIENTS;

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
    setsockopt(socketClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
               sizeof(timeout));
    setsockopt(socketClient, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
               sizeof(timeout));
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
