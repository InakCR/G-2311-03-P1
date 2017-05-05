/**
 * @brief Modulo de funciones axuliares para el manejo de los
 * TADs que gestionan canales y usuarios.
 *
 * @file G-2311-03-P1utilidadesTAD.h
 * @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
 * Enrique Aracil <enrique.aracil@estudiante.uam.es>
 * Grupo 2311
 * @version 1.0
 * @date 13-02-2017
 */
#include "../includes/lib.h"
/**
@page send_all_user() \b
@brief send_all_user. - Envia a todos los usuarios
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b void send_all_user(char *command);
@section description Descripcion
  Envia el commando dado a todos los clietes conectados al servidor
@section return RETORNO
@return void
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
void send_all_user(char *command);
/**
@page set_away() \b
@brief set_away. - Setea el mensaje de ausente
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b int set_away(char* nick,char * reason);
@section description Descripcion
  Asigna el mensaje de ausente dado al actual usuario en el servidor
@section return RETORNO
@return int: Menor de 0 en caso de ERROR.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int set_away(char* nick,char * reason);
/**
@page get_numero_nlientes_actuales() \b
@brief get_numero_nlientes_actuales. - Numero de usuarios conectados
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b long get_numero_nlientes_actuales();
@section description Descripcion
Devuelve el número de clintes conectados actualmente al servidor.
@section return RETORNO
@return long: nClientes.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
long get_numero_nlientes_actuales();
/**
@page get_numero_canales() \b
@brief get_numero_canales. - Numero de canales en el servidor
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b long get_numero_canales();
@section description Descripcion
Devuelve el número de canales activos actualmente en el servidor.
@section return RETORNO
@return long: num, numero de canales en el servidor.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
long get_numero_canales();
/**
@page get_sockets_usuarios() \b
@brief get_sockets_usuarios - Sockets de Conexion
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b int *get_sockets_usuarios();
@section description Descripcion
Devuelve los sockets de todos los usuarios.
@section return RETORNO
@return int*: array de int con los sockets de todos los usuarios.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int *get_sockets_usuarios();
/**
@page get_nick_usuarios() \b
@brief get_nick_usuarios - Nicks de los usuarios en el Servidor
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b char **get_nick_usuarios();
@section description Descripcion
Devuelve los nicks de los usuarios registrados en el servidor.
@section return RETORNO
 @return char**: array de char con los nicks de usuarios.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
char **get_nick_usuarios();
/**
@page get_usuarios_canal() \b
@brief get_usuarios_canal - Nicks de los usuarios en Canal
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b char *get_usuarios_canal(char * channel);
@section description Descripcion
Devuelve los nicks de los usuarios conectados a un canal.
@param channel: Nombre del canal a buscar.
@section return RETORNO
 @return char**: array de char con los nicks de usuarios.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
char *get_usuarios_canal(char * channel);
/**
@page get_lista_canales() \b
@brief get_lista_canales - Lista de canales en el servidor
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b char **get_lista_canales();
@section description Descripcion
 Devuelve la lista de canales registrados en el servidor.
@section return RETORNO
@return char**: array de char con los nombres de los canales.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
char **get_lista_canales();
/**
@page getsocket() \b
@brief getsocket - Socket de un usuario
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b int getsocket(char *nick);
@section description Descripcion
Devuelve el socket asociado al nick del usuario.
@param nick: Nombre del usuario a cambiar.
@section return RETORNO
@return int: socket del usuario.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
int getsocket(char *nick);
/**
@page set_nick() \b
@brief set_nick - Settea el nick del usuario
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b void set_nick(char *nick, char **userNick);
@section description Descripcion
Cambia el nick de un usuario.
@param nick: Nuevo nick del usuario.
@param userNick: Antiguo nick del usuario.
@section return RETORNO
@return int: socket del usuario.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
void set_nick(char *nick, char **userNick);
/**
@page is_away() \b
@brief is_away - Comprobacion ausente
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b char* is_away(char *nick) ;
@section description Descripcion
consulta el mensaje away de un usuario.
@param nick: Nick del usuario.
@section return RETORNO
@return char*: mensaje de away.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
char* is_away(char *nick) ;
/**
@page get_num_usuarios_canal() \b
@brief get_num_usuarios_canal - Numero de usuarios en un canal
@section synopsis SINOPSIS
\b #include \b G-2311-03-P1utilidadesTAD.h
\n\b long get_num_usuarios_canal(char *channel);
@section description Descripcion
consulta el numero de usuarios en un canal.
@param channel: Nombre del canal.
@section return RETORNO
@return long: numero de usuarios del canal.
@section seealso VER TAMBIEN
@section authors AUTORES
Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
*/
long get_num_usuarios_canal(char *channel);
