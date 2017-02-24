#include "../includes/commands.h"

void nick(char *string, int sock){

  char *prefix, *nick, *msg;
  long parser;
  int i = 0;

  parser = IRCParse_Nick(string, &prefix, &nick, &msg);

  syslog(LOG_INFO, "parser = %ld", parser);
  syslog(LOG_INFO, "prefix = %s", prefix);
  syslog(LOG_INFO, "nick = %s", nick);
  syslog(LOG_INFO, "msg = %s", msg);

  if(parser == IRCERR_NOSTRING){

    free(prefix);
    free(nick);
    free(msg);
    on_error(LOG_ERR, "***Error {nick()} No existe una cadena para usar como Nick.");
  }
  else if(parser == IRC_OK){

    // Comprobar que el nick no existe (moviendose por todos los clientes)
    for(i=0; i<getNumeroClientes(); i++){

      syslog(LOG_INFO, "Nick %d -> %s", i, clients[i]->nick);

    }

    // Añadir el nick al cliente que está usando el socket
    for(i=0; i<getNumeroClientes(); i++){

      if(clients[i]->socket == sock){

        strcpy(clients[i]->nick, nick);
        syslog(LOG_INFO, "Nick %d -> %s", i, clients[i]->nick);
      }
    }

    syslog(LOG_INFO, "Nick pasado correctamente, falta añadirlo al cliente");
  }


}

void doCommand(char *string, int sock) {

    if (string == NULL)
        return;

    switch(IRC_CommandQuery(string)) {

        case NICK:
            syslog(LOG_INFO, "NICK");
            nick(string, sock);
            break;
        case USER:
            syslog(LOG_INFO, "USER");
            break;
        case PING:
            syslog(LOG_INFO, "PING");
            break;
        case TOPIC:
            syslog(LOG_INFO, "TOPIC");
            break;
        case LIST:
            syslog(LOG_INFO, "LIST");
            break;
        case JOIN:
            syslog(LOG_INFO, "JOIN");
            break;
        case WHOIS:
            syslog(LOG_INFO, "WHO IS");
            break;
    }
}
