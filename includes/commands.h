#include "../includes/lib.h"
#include <sys/time.h>

/**
   @brief nick.

   Descripcion: Registra el nick de un usuario.
   @param string: Cadena que contiene el comando NICK y el nick.
   @param sock: Puerto en el que se encuentra el usuario.
 */
void nick(char *string, int sock, char **userNick);
/**
   @brief doCommand.

   Identifica al usuario a conectarse al servidor, puede o bien crearlo si
   es su primera conexion o reconocerlo si ya ha estado en el servidor.

   @param string: Cadena que contiene el comando User y sus parametros necesarios.
   @param sock: Puerto en el que se encuentra el usuario.
 */
void user(char *string, int sock, char *userNick);
void whois(char *string, int sock, char *userNick);
void names(char *string, int sock, char *userNick);
void part(char *string, int sock, char *userNick);
void kick(char *string, int sock, char *userNick);
void away(char *string, int sock, char *userNick);
void quit(char *string, int sock, char *userNick);
void motd(char *string, int sock, char *userNick);
void topic(char *string, int sock, char *userNick);
void msg(char *string, int sock, char *userNick);
/**
   @brief doCommand.

   Identifica el commando correcto
   @param string: Cadena que contiene el comando a identificar.
   @param sock: Puerto en el que se encuentra el usuario.
 */
void doCommand(char *string, int sock, char **userNick);
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
int *getSocketsUsuarios();
char **getNickUsuarios();
char *getUsuariosCanal(char * channel);
char **getListaCanales();
int getsocket(char *nick);
int setNick(char *nick, char *userNick);
