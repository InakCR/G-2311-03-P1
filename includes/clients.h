#include "../includes/channels.h"

#define MAX_CARACTERS 30
#define MAX_CHANNELS 20

typedef struct client
{
  char nick[MAX_CARACTERS];
  char name[MAX_CARACTERS];
  int socket;

} *Client;

Client clients[MAX_CLIENTS];

/**
@brief iniClient.

Descripcion: Inicializa un cliente, reserva memoria para el nuevo cliente
y le añade el puerto en el que se encuentra.
@param sock: Puerto (socket) en el que se encuentra el cliente.
@return EXIT_SUCCESS o EXIT_FAILURE en caso de error.
*/
int iniClient(int sock);
