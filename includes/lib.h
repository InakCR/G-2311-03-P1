/**
* @brief Libreria de funciones para la creacion
* y uso de un servidor basado en IRC
*
* @file lib.h
* @author Iñaki Cadalso <innaki.cadalso@estudiante.uam.es>,
* Enrique Aracil <enrique.aracil@estudiante.uam.es>
* Grupo 2311
* @version 1.0
* @date 13-02-2017
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <redes2/irc.h>
#include <redes2/irctad.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdarg.h>
#include <memory.h>
#include <stropts.h>
#include <sys/resource.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>

/**
@brief tamaño maximo que puede tener un buffer.
*/
#define BUFFER_SIZE 1024
/**
@brief numero de clientes maximo
que soporta el servidor.
*/
#define MAX_CLIENTS 20
/**
@brief numero maximo de milisegundos.
*/
#define MAX_MNS 2000
/**
@brief tamaño maximo de bytes.
*/
#define NUM_BYTES 8192
/**
@brief on_error.

Descripcion: Imprime el mensaje dado en el syslog y finaliza el proceso.
@param log: Nivel de prioridad del log.
@param err: mensaje a imprimir.
@return void
*/
void on_error(int log,char* err);
/**
@brief ini_server.

Descripcion: Inicializa un socket y lo deja escuchando
@param port: Puerto donde escucha el servidor.
@return int: EXIT_SUCCESS si todo fue correcto, EXIT_FAILURE en caso de error.
*/
int ini_server(int port);
/**
@brief daemonizar.

Descripcion: Convierte el proceso a depender de ini (daemon)
@param service: Nombre de servicion a daemonizar.
@return int: EXIT_SUCCESS si todo fue correcto, EXIT_FAILURE en caso de error.
*/
int daemonizar(char* service);
/**
@brief accept_conex.

Descripcion: Acepta la conexion por parte de un cliente
@param sock: socket del servidor.
@return int: socket de conexion al cliente si todo fue correcto, EXIT_FAILURE en caso de error.
*/
int accept_conex(int sock);
/**
@brief deal_cliente.

Descripcion: Trata con el cliente recibiendo sus mensajes
  y contestandole
@param sock: Socket de la conexion cliente.
@return void
*/
void* deal_cliente(void* sock);
/**
@brief accept_conex.

Descripcion: Recibe un comman do por parte del cliente y lo realiza ne el servidor
@param sock: socket del servidor.
@return int: socket de conexion al cliente si todo fue correcto, EXIT_FAILURE en caso de error.
*/
int recibir(int sock);
