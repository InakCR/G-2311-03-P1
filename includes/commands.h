#include "../includes/lib.h"

/**
   @brief nick.

   Descripcion: Registra el nick de un usuario.
   @param string: Cadena que contiene el comando NICK y el nick.
   @param sock: Puerto en el que se encuentra el usuario.
 */
void nick(char *string, int sock);
/**
   @brief doCommand.

   Identifica al usuario a conectarse al servidor, puede o bien crearlo si
   es su primera conexion o reconocerlo si ya ha estado en el servidor.

   @param string: Cadena que contiene el comando User y sus parametros necesarios.
   @param sock: Puerto en el que se encuentra el usuario.
 */
void user(char *string, int sock);
void whois(char *string, int sock);
void names(char *string, int sock);
void part(char *string, int sock);
void kick(char *string, int sock);
void away(char *string, int sock);
void quit(char *string, int sock);
void motd(char *string, int sock);
/**
   @brief doCommand.

   Identifica el commando correcto
   @param string: Cadena que contiene el comando a identificar.
   @param sock: Puerto en el que se encuentra el usuario.
 */
void doCommand(char *string, int sock);
/**
@brief getNumeroClientesActuales.

Descripcion: Devuelve el número de clintes conectados actualmente al servidor.
@return int: nClientes.
*/
long getNumeroClientesActuales();
/**
@brief getNumeroCanales.

Descripcion: Devuelve el número de canales activos actualmente en el servidor.
@return int: num.
*/
long getNumeroCanales();
char *getUsuariosCanal(char * channel);
char **getListaCanales();
