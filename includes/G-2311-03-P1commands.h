/**
 * @brief Modulo de funciones para el reconocimiento y ejecucion
 *  de comandos en el servidor IRC.
 *
 * @file G-2311-03-P1commands.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
#include "../includes/G-2311-03-P1user.h"
#include "../includes/G-2311-03-P1canal.h"
/**
   @page ping() \b
   @brief ping - Conexion Ping-Pong
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1commands.h
   \n\b void ping(char *string, int sock, char *userNick)
   @section description Descripcion
   Responde el servidor a una peticion de ping con un pong al
    cliente

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

void ping(char *string, int sock, char *userNick);
/**
   @page quit() \b
   @brief quit - Desconexion con el Servidor
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1commands.h
   \n\b void quit(char *string, int sock, char *userNick)
   @section description Descripcion
   Termina la sesion del usuario en el servidor.

   @param string: Cadena que contiene el comando QUIT y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */

void quit(char *string, int sock, char *userNick);
/**
   @page motd() \b
   @brief motd - Mensaje del Servidor
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1commands.h
   \n\b void motd(char *string, int sock, char *userNick)
   @section description Descripcion
   Envia al usaurio la informacion motd del servidor.

   @param string: Cadena que contiene el comando MOTD y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */
void motd(char *string, int sock, char *userNick);
/**
   @page msg() \b
   @brief msg - Mensajes al Servidor
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1commands.h
   \n\b void msg(char *string, int sock, char *userNick)
   @section description Descripcion
   Envio de mensajes privados entre usuarios o al canal
   recibido en el string del comando.
   @param string: Cadena que contiene el comando MSG y su informacion.
   @param sock: Puerto en el que se encuentra el usuario.
   @param userNick: nick que usa el usuario conectado en el server.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   \b msg_user() msg_canal()
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */
void msg(char *string, int sock, char *userNick);
/**
   @page nocommand() \b
   @brief nocommand - Commando no reconocido
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1commands.h
   \n\b void nocommand(char *string, int sock, char *userNick)
   @section description Descripcion
   Informa al cliente de que el servidor no ha reconocido el
    comando.
    @param string: Cadena que contiene el comando no reconocido y su informacion.
    @param sock: Puerto en el que se encuentra el usuario.
    @param userNick: nick que usa el usuario conectado en el server.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */

void nocommand(char *string, int sock, char *userNick);
/**
   @page do_command() \b
   @brief do_command - Switch de comandos
   @section synopsis SINOPSIS
   \b #include \b G-2311-03-P1commands.h
   \n\b void do_command(char *string, int sock, char **userNick);
   @section description Descripcion
    Identifica el commando correcto del servidor.
    @param string: Cadena que contiene el comando a identificar.
    @param sock: Puerto en el que se encuentra el usuario.
   @section return RETORNO
   Retorno void
   @section seealso VER TAMBIEN
   @section authors AUTORES
   Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
   * Enrique Aracil <enrique.aracil@estudiante.uam.es>
   */

void do_command(char *string, int sock, char **userNick);
