#include "../includes/commands.h"

char *userNick = NULL;

void ping(char *string, int sock) {
  char *prefix, *server, *server2, *msg, *command;
  if (IRCParse_Ping(string, &prefix, &server, &server2, &msg) == IRC_OK) {

    if (IRCMsg_Pong(&command, "REDES2", "REDES2", server2, server) != IRC_OK) {
      syslog(LOG_ERR, "Error PONG");
    } else {
      send(sock, command, strlen(command), 0);
    }
    free(command);
  }

  free(prefix);
  free(server);
  free(server2);
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
      parser = IRCMsg_Join(&command, userNick, NULL, NULL, channel);
      if (parser == IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s 1", command);

        IRCTAD_GetTopic(channel, &topic);
      }
      parser = IRCMsg_RplTopic(&command, "REDES2", userNick, channel, topic);
      if (parser == IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s 2", command);
      }

      parser = IRCMsg_RplNamReply(&command, "REDES2", userNick, "=", channel,
                                  getUsuariosCanal(channel));
      if (parser == IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s 3", command);
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
  char *prefix, *nick, *msg, *command, **nicks;
  long parser, num;
  int *sockets, i;
  // Changes your online nick name. Alerts others to the change of your nick
  parser = IRCParse_Nick(string, &prefix, &nick, &msg);
  if (parser < 0)
    return;

  if (userNick == NULL) {
    userNick = (char *)malloc(sizeof(strlen(nick) + 1));
    strcpy(userNick, nick);
  }
  if (parser == IRCERR_NOSTRING) {
    free(prefix);
    free(nick);
    free(msg);
    syslog(LOG_ERR, "***Error No existe una cadena para usar como Nick.");
    return;
  } else if (parser == IRC_OK) {

    if (UTestNick(nick)) {
      syslog(LOG_INFO, " Ya existe el nick: %s", nick);
      // MAndar mensaje error
    } else {
      if (IRCTADUser_Test(0, NULL, userNick) == IRC_OK) {
        sockets = getSocketsUsuarios();
        nicks = getNickUsuarios();
        int s = getsocket(nicks[0]);
        if (setNick(nick) == IRC_OK) {

          if (IRCMsg_GeneralCommand(&command, "REDES2", "NICK", "cambio nick",
                                    NULL) == IRC_OK) {
            num = getNumeroClientesActuales();

            for (i = 0; i < num; i++) {
              syslog(LOG_INFO, "Sockect 1 %d", sock);
              syslog(LOG_INFO, "Sockect 3 %d", s);
              syslog(LOG_INFO, "Sockect 2 %d", sockets[i]);
              send(sock, command, strlen(command), 0);
              syslog(LOG_INFO, "Cimmando%s", command);
            }
          }
        }
        syslog(LOG_INFO, " No existe el nick: %s", nick);
      }
    }

    syslog(LOG_INFO, "Nick pasado correctamente, falta añadirlo al cliente");
    free(prefix);
    free(nick);
    free(msg);
  }
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
      syslog(LOG_INFO, "Sockect 1 %d", sock);
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

  syslog(LOG_INFO, "%s", command);
  free(prefix);
  free(user);
  free(modehost);
  free(serverother);
  free(realname);
}
void list(char *string, int sock) {
  char *prefix, *channel, *key, *command, *topic, **list, *target;
  long parser;
  int i;
  if (IRCParse_List(string, &prefix, &channel, &target) == IRC_OK) {
    // Miramos que haya canales
    if (getNumeroCanales() > 0) {
      // Si nos epecifican el canal O MASCARA
      if (channel != NULL) {
        syslog(LOG_INFO, "Canal encontrado");
        IRCTAD_GetTopic(channel, &topic);
        if (topic != NULL) {
          IRCMsg_RplList(&command, "REDES2", userNick, channel, "2-3", topic);
          send(sock, command, strlen(command), 0);

          free(command);
        }
        free(channel);
      } else {
        syslog(LOG_INFO, "Varios Canales");
        list = getListaCanales();
        for (i = 0; i < getNumeroCanales(); i++) {
          IRCTAD_GetTopic(list[i], &topic);
          // Mirar Nivel "2-3"
          IRCMsg_RplList(&command, "REDES2", userNick, list[i], "2-3", topic);
          send(sock, command, strlen(command), 0);
          free(topic);
          free(command);
        }
      }

      IRCMsg_RplListEnd(&command, "REDES2", userNick);
      send(sock, command, strlen(command), 0);
    } else {
      syslog(LOG_INFO, "No hay canales");
    }
  } else {
    syslog(LOG_ERR, "Error de parseo");
  }
  free(prefix);
  free(command);
  free(target);
}
void who(char *string, int sock) {
  char *prefix, *target, *mask, *oppar, *command, *list;
  long num = 0;
  if (IRCParse_Who(string, &prefix, &mask, &oppar) == IRC_OK) {

    if (IRCTAD_ListNicksOnChannel(mask, &list, &num) == IRC_OK) {
      if (num > 0) {
        // 319
        IRCMsg_RplWhoIsChannels(&command, "REDES2", userNick, mask, list);

        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
      syslog(LOG_INFO, "%ld", num);
    }
  }
}
void whois(char *string, int sock) {
  char *prefix, *target, *maskarray, *command;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  char *listChan;
  long id = 0, actionTS = 0, creationTS = 0, num = 0;
  int socket = 0;
  if (IRCParse_Whois(string, &prefix, &target, &maskarray) == IRC_OK) {
    syslog(LOG_INFO, "%s prefix", prefix);
    syslog(LOG_INFO, "%s target", target);
    syslog(LOG_INFO, "%s maskarray", maskarray);
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
      // IRCMsg_RplWhoIsOperator(&command, "REDES2", userNick, maskarray);
      // send(sock, command, strlen(command), 0);
      // syslog(LOG_INFO, "%s", command);

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
      // IRCMsg_RplWhoIsIdle(&command, "REDES2", userNick, maskarray,
      //                     (int)actionTS, "seconds idle");
      // send(sock, command, strlen(command), 0);
      // syslog(LOG_INFO, "%s", command);

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

void names(char *string, int sock) {
  char *prefix, *channel, *key, *command, *topic, *list, *target;
  long parser, num;

  if (IRCParse_Names(string, &prefix, &channel, &target) == IRC_OK) {
    if (IRCTAD_ListNicksOnChannel(channel, &list, &num) == IRC_OK) {
      if (num > 0) {
        IRCMsg_RplNamReply(&command, "REDES2", userNick, "=", channel, list);
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
    }
    IRCMsg_RplEndOfNames(&command, "REDES2", userNick, channel);
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
  syslog(LOG_INFO, "%s channel,%s prefix, %s target", channel, prefix, target);
}
void part(char *string, int sock) {
  char *prefix, *channel, *key, *command, *topic, **list, *msg;
  long parser;
  int i;

  // /PART channel msg
  //
  // Leave channel. If no channel given then the channel is the current ones.
  if (IRCParse_Part(string, &prefix, &channel, &msg) == IRC_OK) {
    parser = IRCTAD_Part(channel, userNick);
    if (parser == IRC_OK) {
      parser = IRCMsg_Part(&command, userNick, channel, msg);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s 1", command);
    }
    syslog(LOG_INFO, "%s channel,%s prefix, %s target", channel, prefix, msg);
  }
}
void kick(char *string, int sock) {}
void away(char *string, int sock) {}
void quit(char *string, int sock) {
  //   /QUIT reason
  //
  // Causes you to disconnect from the server.
  // If you include a reason, it will be displayed on all channels as you quit
}
void motd(char *string, int sock) {}
void topic(char *string, int sock) {
  //   /TOPIC topic
  //
  // Topic <channel> will display the current topic of the given channel.
  //  Topic <channel> <topic> will change the topic of the given channel.
}
void msg(char *string, int sock) {
  char *command, *nickorchannel, *msg, **arraylist;
  long nUsers;
  if (IRCUserParse_Msg(string, &nickorchannel, &msg) == IRC_OK) {
    if (IRCTADUser_Test(0, NULL, nickorchannel) == IRC_OK) {
      if (IRCMsg_Privmsg(&command, "REDES2", nickorchannel, msg) == IRC_OK)
        send(getsocket(nickorchannel), command, strlen(command), 0);
    } else if (IRCTAD_TestUserOnChannel(nickorchannel, userNick) == IRC_OK) {
      // if (IRCTAD_ListNicksOnChannelArray(nickorchannel, &arraylist,
      // &nUsers)
      // ==
      //     IRC_OK){
      //       for(i=0;i<nUsers;i++) {
      //
      //       }
      //     }
      if (IRCMsg_Privmsg(&command, "REDES2", nickorchannel, msg) == IRC_OK)
        send(sock, command, strlen(command), 0);
    } else {
      // Ni exite Nick no channel
    }
  }
  free(nickorchannel);
  free(msg);
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
    list(string, sock);
    break;
  case JOIN:
    syslog(LOG_INFO, "JOIN");
    join(string, sock);
    break;
  case WHOIS:
    syslog(LOG_INFO, "WHO IS");
    whois(string, sock);
    break;
  case WHO:
    syslog(LOG_INFO, "WHO");
    who(string, sock);
    break;
  case NAMES:
    syslog(LOG_INFO, "NAMES");
    names(string, sock);
    break;
  case PART:
    syslog(LOG_INFO, "PART");
    part(string, sock);
    break;
  case KICK:
    syslog(LOG_INFO, "KICK");
    kick(string, sock);
    break;
  case AWAY:
    syslog(LOG_INFO, "AWAY");
    away(string, sock);
    break;
  case QUIT:
    syslog(LOG_INFO, "QUIT");
    quit(string, sock);
    break;
  case MOTD:
    syslog(LOG_INFO, "MOTD");
    motd(string, sock);
    break;
  case PRIVMSG:
    syslog(LOG_INFO, "MSG");
    msg(string, sock);
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
int *getSocketsUsuarios() {
  char **users = NULL, **nicks = NULL, **realnames = NULL, **passwords = NULL,
       **hosts = NULL, **IPs = NULL;
  int *sockets = NULL;
  long nelements = 0, *ids = NULL, *modes = NULL, *creationTSs = NULL,
       *actionTSs = NULL;
  if (IRCTADUser_GetAllLists(&nelements, &ids, &users, &nicks, &realnames,
                             &passwords, &hosts, &IPs, &sockets, &modes,
                             &creationTSs, &actionTSs) == IRC_OK) {
    syslog(LOG_INFO, "%s", nicks[nelements - 1]);
    syslog(LOG_INFO, "%s", IPs[nelements - 1]);
    syslog(LOG_INFO, "%d", sockets[nelements - 1]);
    return sockets;
  }
  return NULL;
}
char **getNickUsuarios() {
  char **users = NULL, **nicks = NULL, **realnames = NULL, **passwords = NULL,
       **hosts = NULL, **IPs = NULL;
  int *sockets = NULL;
  long nelements = 0, *ids = NULL, *modes = NULL, *creationTSs = NULL,
       *actionTSs = NULL;
  if (IRCTADUser_GetAllLists(&nelements, &ids, &users, &nicks, &realnames,
                             &passwords, &hosts, &IPs, &sockets, &modes,
                             &creationTSs, &actionTSs) == IRC_OK) {
    syslog(LOG_INFO, "%s", nicks[nelements - 1]);
    syslog(LOG_INFO, "%s", IPs[nelements - 1]);
    syslog(LOG_INFO, "%d", sockets[nelements - 1]);
    return nicks;
  }
  return NULL;
}
char *getUsuariosCanal(char *channel) {
  long num = 0;
  char *list;
  IRCTAD_ListNicksOnChannel(channel, &list, &num);
  return list;
}
char **getListaCanales() {
  long num = 0;
  char **list;
  IRCTADChan_GetList(&list, &num, NULL);
  return list;
}
long getNumeroCanales() {
  char **list = NULL;
  long num = 0;
  IRCTADChan_GetList(&list, &num, NULL);
  IRCTADUser_FreeList(list, num);
  return num;
}
int getsocket(char *nick) {
  long id = 0, creationTS = 0, actionTS = 0, parser = 0;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  int socket = 0;
  parser = IRCTADUser_GetData(&id, &user, &nick, &real, &host, &IP, &socket,
                              &creationTS, &actionTS, &away);
  if (parser == IRC_OK)
    return socket;

  return -1;
}
int setNick(char *nick) {
  long id = 0, creationTS = 0, actionTS = 0;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  int socket = 0;

  if (IRCTADUser_GetData(&id, &user, &userNick, &real, &host, &IP, &socket,
                         &creationTS, &actionTS, &away) == IRC_OK)
    if (IRCTADUser_Set(id, user, userNick, real, user, nick, real) == IRC_OK) {
      free(userNick);
      userNick = (char *)malloc(sizeof(strlen(nick) + 1));
      strcpy(userNick, nick);
    }
  return IRC_OK;
}
// LIBERAR ESTRUCTURAS
