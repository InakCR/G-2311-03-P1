#include "../includes/clients.h"


int iniClient(int sock){

  int nClientes = getNumeroClientes();

  clients[nClientes] = malloc(sizeof(Client));

  if(clients[nClientes] == NULL){
    return EXIT_FAILURE;
  }

  clients[nClientes]->socket = sock;

  syslog(LOG_INFO, "Cliente creado en estructura en el socket %d",
    clients[nClientes]->socket);

  return EXIT_SUCCESS;
}
