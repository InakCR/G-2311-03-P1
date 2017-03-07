#include "../includes/commands.h"

char *userNick;

void ping(char *string, int sock) {
  char *prefix, *server, *server2, *msg = NULL, *command, *receiver;
  receiver = (char *)malloc(sizeof(5));
  strcpy(receiver, "PONG");
  if (IRCParse_Ping(string, &prefix, &server, &server2, &msg) == IRC_OK) {
    if (msg != NULL) {
      free(receiver);
      receiver = msg;
    }
    if (IRCMsg_Pong(&command, "REDES2", server, server2, receiver) != IRC_OK) {
      syslog(LOG_ERR, "Error PONG");
    } else {
      send(sock, command, strlen(command), 0);
    }
    free(command);
  }

  free(prefix);
  free(server);
  free(server2);
  free(receiver);
}
void join(char *string, int sock) {
  char *prefix, *msg, *channel, *key, *command, *topic;
  long parser;
  parser = IRCParse_Join(string, &prefix, &channel, &key, &msg);
  if (parser == IRCERR_NOSTRING || parser == IRCERR_ERRONEUSCOMMAND) {
    free(prefix);
    free(msg);
    free(channel);
    free(key);
    syslog(LOG_ERR, "***Error No existe una cadena para usar como User.");
    return;
  } else if (parser == IRC_OK) {

    parser = IRCTADChan_New(channel, "+", userNick, key, MAX_CLIENTS,
                            "Nueva creacion");
    parser = IRCTAD_Join(channel, userNick, NULL, key);
    // Usermoden o va haber administradores
    if (parser == IRC_OK) {
      parser = IRCMsg_Join(&command, "REDES2", NULL, NULL, channel);
      if (parser == IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);

        IRCTAD_GetTopic(channel, &topic);
      }
      parser = IRCMsg_RplTopic(&command, "REDES2", userNick, channel, topic);
      if (parser == IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }

      parser = IRCMsg_RplNamReply(&command, "REDES2", userNick, "=", channel,
                                  getUsuariosCanal());
      if (parser == IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
      parser = IRCMsg_RplEndOfNames(&command, "REDES2", userNick, channel);
      if (parser == IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
      free(command);
    }

  } else {
    syslog(LOG_ERR, "***Fallo al crear el canal");
    // Puede devolcer muchos errores
  }
  free(prefix);
  free(msg);
  free(channel);
  free(key);
}
void nick(char *string, int sock) {
  char *prefix, *nick, *msg;
  long parser;

  parser = IRCParse_Nick(string, &prefix, &nick, &msg);
  if (parser < 0)
    return;

  syslog(LOG_INFO, "parser = %ld", parser);
  syslog(LOG_INFO, "prefix = %s", prefix);
  syslog(LOG_INFO, "nick = %s", nick);
  syslog(LOG_INFO, "msg = %s", msg);

  userNick = (char *)malloc(sizeof(strlen(nick) + 1));
  strcpy(userNick, nick);

  if (parser == IRCERR_NOSTRING) {
    free(prefix);
    free(nick);
    free(msg);
    syslog(LOG_ERR, "***Error No existe una cadena para usar como Nick.");
    return;
  } else if (parser == IRC_OK) {

    if (UTestNick(nick)) { // MAndar mensaje
      syslog(LOG_INFO, " Ya existe el nick: %s", nick);
    } else {
      syslog(LOG_INFO, " No existe el nick: %s", nick);
    }
  }

  syslog(LOG_INFO, "Nick pasado correctamente, falta añadirlo al cliente");
  free(prefix);
  free(nick);
  free(msg);
}

void user(char *string, int sock) {
  char *prefix, *msg, *user, *modehost, *serverother, *realname, *command;
  long parser;
  struct timeval tv;
  HostNameIp *hi;
  hi = hostIp(sock);

  parser =
      IRCParse_User(string, &prefix, &user, &modehost, &serverother, &realname);

  if (parser < 0) // Controlar este error con una MACRO
    return;
  syslog(LOG_INFO, "ip = %s", hi->ip);
  syslog(LOG_INFO, "host = %s", hi->name);

  if (parser == IRCERR_NOSTRING) {
    free(prefix);
    free(user);
    free(modehost);
    free(serverother);
    free(realname);
    syslog(LOG_ERR, "***Error No existe una cadena para usar como User.");
    return;

  } else if (parser == IRC_OK) {
    if (IRCTADUser_New(user, userNick, realname, NULL, hi->name, hi->ip,
                       sock) == IRC_OK) {
      syslog(LOG_INFO, "%s ,%s", user, userNick);
    } else {
      syslog(LOG_INFO, "Usuario no creado");
    }
  }

  IRCMsg_RplWelcome(&command, "REDES2", userNick, user, realname, modehost);
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  IRCMsg_RplYourHost(&command, "REDES2", userNick, serverother, "1.0");
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  IRCMsg_RplCreated(&command, "REDES2", userNick, gettimeofday(&tv, NULL));
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  IRCMsg_RplLuserClient(&command, "REDES2", userNick,
                        getNumeroClientesActuales(), 0, 1);
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  IRCMsg_RplLuserChannels(&command, "REDES2", userNick, getNumeroCanales());
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  IRCMsg_RplMotdStart(&command, "REDES2", userNick, serverother);
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  IRCMsg_RplMotd(&command, "REDES2", userNick, "****BIENVENIDO****\n");
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  IRCMsg_RplEndOfMotd(&command, "REDES2", userNick);
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  free(prefix);
  free(user);
  free(modehost);
  free(serverother);
  free(realname);
}
void doCommand(char *string, int sock) {

  if (string == NULL)
    return;

  switch (IRC_CommandQuery(string)) {

  case NICK:
    syslog(LOG_INFO, "NICK");
    nick(string, sock);
    break;
  case USER:
    syslog(LOG_INFO, "USER");
    user(string, sock);
    break;
  case PING:
    syslog(LOG_INFO, "PING");
    ping(string, sock);
    break;
  case TOPIC:
    syslog(LOG_INFO, "TOPIC");
    break;
  case LIST:
    syslog(LOG_INFO, "LIST");
    break;
  case JOIN:
    syslog(LOG_INFO, "JOIN");
    join(string, sock);
    break;
  case WHOIS:
    syslog(LOG_INFO, "WHO IS");
    break;
  }
}
long getNumeroClientesActuales() {
  long nelements = 0;
  char **nicklist;
  IRCTADUser_GetUserList(&nicklist, &nelements);
  IRCTADUser_FreeList(nicklist, nelements);
  return nelements;
}
char *getUsuariosCanal() {
  long num = 0;
  char **list;
  IRCTADChan_GetList(&list, &num, NULL);
  return *list;
}
long getNumeroCanales() {
  char **list = NULL;
  long num = 0;
  IRCTADChan_GetList(&list, &num, NULL);
  IRCTADUser_FreeList(list, num);
  return num;
}
