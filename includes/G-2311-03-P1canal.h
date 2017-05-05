/**
 * @brief Modulo de funciones la ejecucion
 *  de comandos en el servidor IRC relacionados con el usuario
 *
 * @file G-2311-03-P1canal.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
 #include "../includes/G-2311-03-P1utilidadesTAD.h"
 /**
    @page join() \b
    @brief join - Union a un Canal
    @section synopsis SINOPSIS
    \b #include \b G-2311-03-P1canal.h
    \n\b void join(char *string, int sock, char *userNick)
    @section description Descripcion
    Crea un nuevo canal y/o se une el usuario al canal, mostrando
    la informacion del canal.
    @param string: Cadena que contiene el comando JOIN y su informacion.
    @param sock: Puerto en el que se encuentra el usuario.
    @param userNick: nick que usa el usuario conectado en el server.
    @section return RETORNO
    Retorno void
    @section seealso VER TAMBIEN
    @section authors AUTORES
    Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
    * Enrique Aracil <enrique.aracil@estudiante.uam.es>
    */
 void join(char *string, int sock, char *userNick);
 /**
    @page names() \b
    @brief names - Lista de usuarios
    @section synopsis SINOPSIS
    \b #include \b G-2311-03-P1canal.h
    \n\b void names(char *string, int sock, char *userNick)
    @section description Descripcion
    Lista los usuarios presentes en el canal dado por
    por el string del comando.
    @param string: Cadena que contiene el comando NAMES y su informacion.
    @param sock: Puerto en el que se encuentra el usuario.
    @param userNick: nick que usa el usuario conectado en el server.
    @section return RETORNO
    Retorno void
    @section seealso VER TAMBIEN
    @section authors AUTORES
    Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
    * Enrique Aracil <enrique.aracil@estudiante.uam.es>
    */
 void names(char *string, int sock, char *userNick);
 /**
    @page part() \b
    @brief part - Salida de un canal
    @section synopsis SINOPSIS
    \b #include \b G-2311-03-P1canal.h
    \n\b void part(char *string, int sock, char *userNick)
    @section description Descripcion
    El usuario sale del canal dado eliminandolo del canal y
     notificando de su marcha.
     @param string: Cadena que contiene el comando PART y su informacion.
     @param sock: Puerto en el que se encuentra el usuario.
     @param userNick: nick que usa el usuario conectado en el server.
    @section return RETORNO
    Retorno void
    @section seealso VER TAMBIEN
    @section authors AUTORES
    Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
    * Enrique Aracil <enrique.aracil@estudiante.uam.es>

  */
 void part(char *string, int sock, char *userNick);
 /**
    @page kick() \b
    @brief kick - Salida forzada de un canal
    @section synopsis SINOPSIS
    \b #include \b G-2311-03-P1canal.h
    \n\b  void kick(char *string, int sock, char *userNick);
    @section description Descripcion
    Elimina el usuario dado por string del canal tambien dado.
     El usuario solicitante del comando debera tener privilegios para
     hacerlo
     @param string: Cadena que contiene el comando KICK y su informacion.
     @param sock: Puerto en el que se encuentra el usuario.
     @param userNick: nick que usa el usuario conectado en el server.
     @section return RETORNO
    Retorno void
    @section seealso VER TAMBIEN
    @section authors AUTORES
    Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
    * Enrique Aracil <enrique.aracil@estudiante.uam.es>

  */
 void kick(char *string, int sock, char *userNick);
  /**
 @page topic() \b
 @brief topic - Mensaje canal
 @section synopsis SINOPSIS
 \b #include \b G-2311-03-P1canal.h
 \n\b  void topic(char *string, int sock, char *userNick)
 @section description Descripcion
 Cambia el topic de un canal dado.
 Comprueba el nivel de proteccion del canal para el
 cambio por parte del dueño del canal.
     @param string: Cadena que contiene el comando TOPIC y su informacion.
     @param sock: Puerto en el que se encuentra el usuario.
     @param userNick: nick que usa el usuario conectado en el server.
  @section return RETORNO
 Retorno void
 @section seealso VER TAMBIEN
 @section authors AUTORES
 Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>

*/

 void topic(char *string, int sock, char *userNick);
 /**
@page mode() \b
@brief mode - Opcion mode del canal
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1canal.h
\n\b  void topic(char *string, int sock, char *userNick)
@section description Descripcion
Cambia la proteccion de un canal.

    @param string: Cadena que contiene el comando MODE y su informacion.
    @param sock: Puerto en el que se encuentra el usuario.
    @param userNick: nick que usa el usuario conectado en el server.
 @section return RETORNO
Retorno void
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>

*/

 void mode(char *string, int sock, char *userNick);
 /**
@page msg_canal() \b
@brief msg_canal - Mensaje a canal
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1canal.h
\n\b   void msg_canal(char*channel,char* userNick, char*msg)
@section description Descripcion
Envia un mensaje privado al canal dado.

@param channel: Nombre del canal a enviar el mensaje.
@param userNick: nick que usa el usuario conectado en el server.
@param char*: mensaje a enviar.
 @section return RETORNO
Retorno void
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>

*/

 void msg_canal(char*channel,char* userNick, char*msg);
