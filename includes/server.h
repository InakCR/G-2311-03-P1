/**
 * @brief Libreria de funciones para la gestion del servidor IRC
 *
 * @file server.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
#include "../includes/commands.h"
/**
   @brief tiempo maximo de alarma en segundos
 */
#define STIEMPO 30
/**
   @brief tiempo maximo de alarma en milisegundos
 */
#define MSTIEMPO 30000
/**
   @brief puerto de escucha del servidor
 */
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
int recibir(int sock, char** userNick);
/**
@brief getNumeroClientes.

Descripcion: Devuelve el número de clintes conectados actualmente al servidor.
@return int: nClientes.
*/
long getNumeroClientes();
