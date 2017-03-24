/**
 * @brief Modulo de funciones la ejecucion
 *  de comandos en el servidor IRC relacionados con el usuario
 *
 * @file canal.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
 #include "../includes/utilidadesTAD.h"
 /**
    @brief join.

    Descripcion: Crea un nuevo canal y/o se une el usuario al canal, mostrando
    la informacion del canal.

    @param string: Cadena que contiene el comando PING y su informacion.
    @param sock: Puerto en el que se encuentra el usuario.
    @param userNick: nick que usa el usuario conectado en el server.
  */
 void join(char *string, int sock, char *userNick);
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
    @brief mode.

    Descripcion:  Cambia la proteccion de un canal.

    @param string: Cadena que contiene el comando TOPIC y su informacion.
    @param sock: Puerto en el que se encuentra el usuario.
    @param userNick: nick que usa el usuario conectado en el server.
  */
 void mode(char *string, int sock, char *userNick);
 void msgCanal(char*channel,char* userNick, char*msg);
