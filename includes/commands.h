#include "../includes/server.h"

/**
@brief nick.

Descripcion: Registra el nick de un usuario.
@param string: Cadena que contiene el comando NICK y el nick.
@param sock: Puerto en el que se encuentra el usuario.
*/
void nick(char *string, int sock);
void doCommand(char *string, int sock);
