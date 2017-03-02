#include "../includes/commands.h"

#define PORT 6667
/**
@brief deal_cliente.

Descripcion: Trata con el cliente recibiendo sus mensajes
  y contestandole
@param sock: Socket de la conexion cliente.
@return void
*/
void* deal_cliente(void* sock);
/**
@brief accept_conex.

Descripcion: Recibe un comman do por parte del cliente y lo realiza ne el servidor
@param sock: socket del servidor.
@return int: socket de conexion al cliente si todo fue correcto, EXIT_FAILURE en caso de error.
*/
int recibir(int sock);
/**
@brief getNumeroClientes.

Descripcion: Devuelve el número de clintes conectados actualmente al servidor.
@return int: nClientes.
*/
long getNumeroClientes();
/**
@brief getNumeroCanales.

Descripcion: Devuelve el número de canales activos actualmente en el servidor.
@return int: num.
*/
long getNumeroCanales();
