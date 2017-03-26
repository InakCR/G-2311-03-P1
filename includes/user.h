/**
 * @brief Modulo de funciones la ejecucion
 *  de comandos en el servidor IRC relacionados con el usuario
 *
 * @file user.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
#include "../includes/utilidadesTAD.h"
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
   @brief away.

   Descripcion: Establece al usuario en estado away con el
    mensaje dado por string.

   @param string: Cadena que contiene el comando AWAY y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void away(char *string, int sock, char *userNick);
void msgUser(char* nick,char* userNick,char* msg);
void setMotdUser(char *motd);
