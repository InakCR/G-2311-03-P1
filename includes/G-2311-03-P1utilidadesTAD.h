/**
 * @brief Modulo de funciones axuliares para el manejo de los
 * TADs que gestionan canales y usuarios.
 *
 * @file utilidadesTAD.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
#include "../includes/lib.h"
void sendAllUser(char *command);
/**
   @brief getNumeroClientesActuales.

   Descripcion: Cambia el mensaje away del usuario.
   @param nick: nick que usa el usuario conectado en el server.
   @param reason: mensaje de away.
   @return int: error.
 */
int setAway(char* nick,char * reason);
/**
   @brief getNumeroClientesActuales.

   Descripcion: Devuelve el número de clintes conectados actualmente al servidor.
   @return long: nClientes.
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
void setNick(char *nick, char **userNick);
/**
  @brief isAway.

  Descripcion: consulta el mensaje away de un usuario.
  @param nick: Nick del usuario.
  @return char*: mensaje de away.
*/
char* isAway(char *nick) ;
/**
  @brief getNumUsuariosCanal.

  Descripcion: consulta el numero de usuarios en un canal.
  @param channel: Nombre del canal.
  @return long: numero de usuarios del canal.
*/
long getNumUsuariosCanal(char *channel);
