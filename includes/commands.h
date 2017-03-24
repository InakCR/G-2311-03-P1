/**
 * @brief Modulo de funciones para el reconocimiento y ejecucion
 *  de comandos en el servidor IRC.
 *
 * @file commands.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
#include "../includes/user.h"
#include "../includes/canal.h"
/**
   @brief ping.

   Descripcion: Responde el servidor a una peticion de ping con un pong al
    cliente

   @param string: Cadena que contiene el comando NICK y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void ping(char *string, int sock, char *userNick);
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
   @brief msg.

   Descripcion:Envio de mensajes privados entre usuarios o al canal
   recibido en el string del comando.

   @param string: Cadena que contiene el comando MSG y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void msg(char *string, int sock, char *userNick);
/**
   @brief nocommand.

   Descripcion: Informa al cliente de que el servidor no ha reconocido el
    comando.

   @param string: Cadena que contiene el comando MSG y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
 */
void nocommand(char *string, int sock, char *userNick);
/**
   @brief doCommand.

   Descripcion: Identifica el commando correcto del servidor.

   @param string: Cadena que contiene el comando a identificar.
   @param sock: Puerto en el que se encuentra el usuario.
 */
void doCommand(char *string, int sock, char **userNick);
