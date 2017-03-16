#include "../includes/lib.h"
#include <sys/time.h>
/**
   @brief ping.

   Descripcion:

   @param string: Cadena que contiene el comando NICK y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void ping(char *string, int sock, char *userNick);
/**
   @brief join.

   Descripcion:

   @param string: Cadena que contiene el comando PING y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void join(char *string, int sock, char *userNick);
/**
   @brief nick.

   Descripcion: Registra el nick de un usuario.En caso de que exista
   comprueba que sea el mismo usuario que quiere cambiar el nick.
   Cambiara el nick del usuario o dara un error de nick usado.

   @param string: Cadena que contiene el comando NICK y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void nick(char *string, int sock, char **userNick);
/**
   @brief user.

   Descripcion: Crea un nuevo usuario en el servidor, conectadolo al
   servidor para que pueda hacer uso de los comandos.

   @param string: Cadena que contiene el comando USER y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void user(char *string, int sock, char *userNick);
/**
   @brief whois.

   Descripcion: Muestra la informacion del usuario dado por el string
    del comando.

   @param string: Cadena que contiene el comando WHOIS y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void whois(char *string, int sock, char *userNick);
/**
   @brief names.

   Descripcion: Lista los usuarios presentes en el canal dado por
    por el string del comando.

   @param string: Cadena que contiene el comando NAMES y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void names(char *string, int sock, char *userNick);
/**
   @brief part.

   Descripcion: El usuario sale del canal dado eliminandolo del canal y
    notificando de su marcha.

   @param string: Cadena que contiene el comando PART y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void part(char *string, int sock, char *userNick);
/**
   @brief kick.

   Descripcion: Elimina el usuario dado por string del canal tambien dado.
    El usuario solicitante del comando debera tener privilegios para
    hacerlo

   @param string: Cadena que contiene el comando KICK y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void kick(char *string, int sock, char *userNick);
/**
   @brief away.

   Descripcion: Establece al usuario en estado away con el
    mensaje dado por string.

   @param string: Cadena que contiene el comando AWAY y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void away(char *string, int sock, char *userNick);
/**
   @brief quit.

   Descripcion: Termina la sesion del usuario en el servidor.

   @param string: Cadena que contiene el comando QUIT y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void quit(char *string, int sock, char *userNick);
/**
   @brief motd.

   Descripcion: Envia al usaurio la informacion motd del servidor.

   @param string: Cadena que contiene el comando MOTD y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void motd(char *string, int sock, char *userNick);
/**
   @brief topic.

   Descripcion:  Cambia el topic de un canal dado.
    Comprueba el nivel de proteccion del canal para el
    cambio por parte del dueño del canal.

   @param string: Cadena que contiene el comando TOPIC y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void topic(char *string, int sock, char *userNick);
/**
   @brief msg.

   Descripcion:Envio de mensajes privados entre usuarios o al canal
   recibido en el string del comando.

   @param string: Cadena que contiene el comando MSG y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void msg(char *string, int sock, char *userNick);
/**
   @brief doCommand.

   Descripcion: Identifica el commando correcto del servidor.

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
  @return long: num, numero de canales en el servidor.
*/
long getNumeroCanales();
/**
  @brief getSocketsUsuarios.

  Descripcion: Devuelve los sockets de todos los usuarios.
  @return int*: array de int con los sockets de todos los usuarios.
*/
int *getSocketsUsuarios();
/**
  @brief getNickUsuarios.

  Descripcion: Devuelve los nicks de los usuarios registrados en el servidor.
  @return char**: array de char con los nicks de usuarios.
*/
char **getNickUsuarios();
/**
  @brief getUsuariosCanal.

  Descripcion: Devuelve los nicks de los usuarios conectados a un canal.
  @param channel: Nombre del canal a buscar.
  @return char**: array de char con los nicks de usuarios.
*/
char *getUsuariosCanal(char * channel);
/**
  @brief getListaCanales.

  Descripcion: Devuelve la lista de canales registrados en el servidor.
  @return char**: array de char con los nombres de los canales.
*/
char **getListaCanales();
/**
  @brief getsocket.

  Descripcion: Devuelve el socket asociado al nick del usuario.
  @return int: socket del usuario.
*/
int getsocket(char *nick);
/**
  @brief setNick.

  Descripcion: Cambia el nick de un usuario.
  @param nick: Nuevo nick del usuario.
  @param userNick: Antiguo nick del usuario.
*/
void setNick(char *nick, char *userNick);
