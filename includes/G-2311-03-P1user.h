/**
 * @brief Modulo de funciones la ejecucion
 *  de comandos en el servidor IRC relacionados con el usuario
 *
 * @file G-2311-03-P1user.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
#include "../includes/G-2311-03-P1utilidadesTAD.h"
/**
   @page nick() \b
   @brief nick - Registro y Cambio de Nick
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1user.h
   \n\b void nick(char *string, int sock, char **userNick);
   @section description Descripcion
   Registra el nick de un usuario.En caso de que exista
  comprueba que sea el mismo usuario que quiere cambiar el nick.
  Cambiara el nick del usuario o dara un error de nick usado.

  @param string: Cadena que contiene el comando NICK y su informacion.
  @param sock: Puerto en el que se encuentra el usuario.
  @param userNick: nick que usa el usuario conectado en el server.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */
void nick(char *string, int sock, char **userNick);
/**
   @page user() \b
   @brief user - Creacion y conexion del Usuario
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1user.h
   \n\b void user(char *string, int sock, char *userNick);
   @section description Descripcion
   Crea un nuevo usuario en el servidor, conectadolo al
  servidor para que pueda hacer uso de los comandos.
  @param string: Cadena que contiene el comando USER y su informacion.
  @param sock: Puerto en el que se encuentra el usuario.
  @param userNick: nick que usa el usuario conectado en el server.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */

void user(char *string, int sock, char *userNick);
/**
   @page whois() \b
   @brief whois - Informacion de un Usuario
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1user.h
   \n\b void whois(char *string, int sock, char *userNick);
   @section description Descripcion
   Muestra la informacion del usuario dado por el string
    del comando.
    @param string: Cadena que contiene el comando WHOIS y su informacion.
    @param sock: Puerto en el que se encuentra el usuario.
    @param userNick: nick que usa el usuario conectado en el server.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */

void whois(char *string, int sock, char *userNick);
/**
   @page away() \b
   @brief away - Mensaje de ausente
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1user.h
   \n\b void away(char *string, int sock, char *userNick);
   @section description Descripcion
   Establece al usuario en estado away con el
    mensaje dado por string.
    @param string: Cadena que contiene el comando AWAY y su informacion.
    @param sock: Puerto en el que se encuentra el usuario.
    @param userNick: nick que usa el usuario conectado en el server.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */

void away(char *string, int sock, char *userNick);
/**
   @page msg_user() \b
   @brief msg_user - Mensaje a un Usuario
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1user.h
   \n\b void msg_user(char* nick,char* userNick,char* msg);
   @section description Descripcion
    Envia un mensaje privado al usuario dado.
    @param nick: Nick del usuario a enviar el mensaje.
    @param userNick: nick que usa el usuario conectado en el server.
    @param char*: mensaje a enviar.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */

void msg_user(char* nick,char* userNick,char* msg);
