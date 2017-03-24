#include "../includes/user.h"
void nick(char *string, int sock, char **userNick) {
  char *prefix, *nick, *msg, *command, **nicklist;
  long num, nelements;
  int socket, i;

  syslog(LOG_INFO, "Comienza Funcion NICK");
  syslog(LOG_INFO, "%d", sock);

  // Changes your online nick name. Alerts others to the change of your nick
  if (IRCParse_Nick(string, &prefix, &nick, &msg) != IRC_OK) {
    // TODO mensaje de error al cliente
    syslog(LOG_ERR, "***Error No existe una cadena para usar como Nick.");
    return;
  }

  syslog(LOG_INFO, "VALOR DE userNick ANTES DE SER ACTUALIZADO => %s",
         *userNick);

  // userNickES VARIABLE GLOBAL POR TANTO SALVO AL PRINCIPIO SIEMPRE ES != NULL
  if (*userNick == NULL) {
    *userNick = (char *)malloc(sizeof(strlen(nick) + 1));
    strcpy(*userNick, nick);
  }
  syslog(LOG_INFO, "userNick = %s || nick = %s", *userNick, nick);

  if (UTestNick(nick)) {
    syslog(LOG_INFO, "!!!!!!!!!! Ya existe el nick: %s", nick);
    // TODO MAndar mensaje error
  } else {

    syslog(LOG_INFO, "UTestNick->else Dentro");

    if (IRCTADUser_Test(0, NULL, *userNick) == IRC_OK) {

      syslog(LOG_INFO, "setNick Dentro");
      if (IRCMsg_Nick(&command, *userNick, msg, nick) == IRC_OK) {
        IRCTADUser_GetNickList(&nicklist, &nelements);
        // se manda al usuario que lo camabia
        send(sock, command, strlen(command), 0);
        // se manda al resto de usuarios
        for (i = 0; i < nelements; i++) {
          socket = getsocket(nicklist[i]);
          send(socket, command, strlen(command), 0);
        }
        syslog(LOG_INFO, "Comando %s socket: %d", command, socket);
      }
      syslog(LOG_INFO, "IRCTADUser_Test Dentro");
      setNick(nick, userNick);
      syslog(LOG_INFO, " No existe el nick: %s", nick);
    }
  }

  // TODO PROBANDO
  // Lista de Usuarios en el servidor

  IRCTADUser_GetUserList(&nicklist, &nelements);

  for (i = 0; i < nelements; i++) {
    syslog(LOG_INFO, "Usuario nº%d -> %s", i, nicklist[i]);
  }

  // Lista de Nicks en el servidor

  IRCTADUser_GetNickList(&nicklist, &nelements);

  for (i = 0; i < nelements; i++) {
    syslog(LOG_INFO, "Nick nº%d -> %s", i, nicklist[i]);
  }

  syslog(LOG_INFO, "Nick pasado correctamente como %s.", *userNick);
  free(prefix);
  free(nick);
  free(msg);
}
void user(char *string, int sock, char *userNick) {
  char *host, *ip, *prefix, *user, *modehost, *serverother, *realname, *command;
  time_t rawtime;
  int i = 0;

  hostIp(sock, &host, &ip);

  if (IRCParse_User(string, &prefix, &user, &modehost, &serverother,
                    &realname) != IRC_OK) {
    syslog(LOG_ERR, "***Error No existe una cadena para usar como User.");
    return;
  }

  if (IRCTADUser_New(user, userNick, realname, NULL, host, ip, sock) !=
      IRC_OK) {
    syslog(LOG_INFO, "************Usuario no creado");
    return;
  }

  syslog(LOG_INFO, "Creado Usuario: %s (Nick: %s)", user, userNick);
  syslog(LOG_INFO, "En Socket %d", sock);

  // Lista de Usuarios en el servidor
  char **nicklist;
  long nelements;

  IRCTADUser_GetUserList(&nicklist, &nelements);

  for (i = 0; i < nelements; i++) {
    syslog(LOG_INFO, "Usuario nº%d -> %s", i, nicklist[i]);
  }

  // Lista de Nicks en el servidor

  IRCTADUser_GetNickList(&nicklist, &nelements);

  for (i = 0; i < nelements; i++) {
    syslog(LOG_INFO, "Nick nº%d -> %s", i, nicklist[i]);
  }

  IRCMsg_RplWelcome(&command, "REDES2", userNick, user, realname, modehost);
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);

  IRCMsg_RplYourHost(&command, "REDES2", userNick, serverother, "1.0");
  send(sock, command, strlen(command), 0);
  syslog(LOG_INFO, "%s", command);
  time(&rawtime);
  IRCMsg_RplCreated(&command, "REDES2", userNick, rawtime);
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

  syslog(LOG_INFO, "%s", command);

  free(prefix);
  free(user);
  free(modehost);
  free(serverother);
  free(realname);
}
void whois(char *string, int sock, char *userNick) {
  char *prefix, *target, *maskarray, *command;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  char *listChan;
  long id = 0, actionTS = 0, creationTS = 0, num = 0;
  int socket = 0;

  if (IRCParse_Whois(string, &prefix, &target, &maskarray) == IRC_OK) {

    if (UTestNick(maskarray)) {
      syslog(LOG_INFO, "Existe el nick: %s", maskarray);

      if (IRCTADUser_GetData(&id, &user, &maskarray, &real, &host, &IP, &socket,
                             &creationTS, &actionTS, &away) == IRC_OK) {
      }
      // 311
      IRCMsg_RplWhoIsUser(&command, "REDES2", userNick, maskarray, user, host,
                          real);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
      // 312
      IRCMsg_RplWhoIsServer(&command, "REDES2", userNick, maskarray, "REDES2",
                            "No OnE");
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
      // // 313
      if (IRCTAD_ListChannelsOfUser(user, maskarray, &listChan, &num) ==
          IRC_OK) {
        if (num > 0) {
          // 319
          IRCMsg_RplWhoIsChannels(&command, "REDES2", userNick, maskarray,
                                  listChan);

          send(sock, command, strlen(command), 0);
          syslog(LOG_INFO, "%s", command);
        }
        syslog(LOG_INFO, "%ld", num);
      }
      // 317
      // 301
      syslog(LOG_INFO, "Away %s", away);
      if (away != NULL) {
        IRCMsg_RplAway(&command, "REDES2", userNick, maskarray, away);
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
      // 318
      IRCMsg_RplEndOfWhoIs(&command, "REDES2", userNick, maskarray);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
    } else {
      syslog(LOG_INFO, " No existe el nick: %s", maskarray);
    }
  } else {
    syslog(LOG_ERR, "Error Parseo Whois");
  }
}
void away(char *string, int sock, char *userNick) {
  char *reason = NULL, *command, *prefix, **nicklist;
  long nelements;
  int socket, i;

  if (IRCParse_Away(string, &prefix, &reason) != IRC_OK) {
    syslog(LOG_ERR, "Error Away");
    return;
  }

  if (setAway(userNick, reason) != IRC_OK) {
    syslog(LOG_ERR, "Error setaway");
    return;
  }
  if (reason != NULL) {
    if (IRCMsg_RplNowAway(&command, userNick, userNick) == IRC_OK) {
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
    }

    IRCTADUser_GetNickList(&nicklist, &nelements);
    // se manda al usuario que lo camabia
    send(sock, command, strlen(command), 0);
    // se manda al resto de usuarios
    for (i = 0; i < nelements; i++) {
      IRCMsg_RplAway(&command, userNick, userNick, nicklist[i], reason);
      socket = getsocket(nicklist[i]);
      send(socket, command, strlen(command), 0);
    }
  } else {
    if (IRCMsg_RplUnaway(&command, userNick, userNick) == IRC_OK) {
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
    }
  }
  syslog(LOG_INFO, "setaway");
}
void msgUser(char *nick, char *userNick, char *msg) {
  char *command, *reason;
  int socket;
  reason = isAway(nick);
  syslog(LOG_INFO, "%s es %s", nick, reason);
  if (reason != NULL) {
    IRCMsg_RplAway(&command, userNick, userNick, nick, reason);
    socket = getsocket(userNick);
    send(socket, command, strlen(command), 0);
  } else {
    if (IRCMsg_Privmsg(&command, userNick, nick, msg) == IRC_OK) {
      socket = getsocket(nick);
      send(socket, command, strlen(command), 0);
    }
  }
}
