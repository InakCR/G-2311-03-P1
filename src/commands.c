#include "../includes/commands.h"

void ping(char *string, int sock, char *userNick) {
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
void join(char *string, int sock, char *userNick) {
  char *prefix, *msg, *channel, *key, *command, *topic;
  long parser;

  parser = IRCParse_Join(string, &prefix, &channel, &key, &msg);

  if (parser == IRCERR_NOSTRING || parser == IRCERR_ERRONEUSCOMMAND) {
    syslog(LOG_ERR, "***Error No existe una cadena para usar como User.");
    return;
  } else if (parser == IRC_OK) {

    parser = IRCTADChan_New(channel, "+", userNick, key, MAX_CLIENTS,
                            "Nueva creacion");
    parser = IRCTAD_Join(channel, userNick, NULL, key);
    // Usermoden o va haber administradores
    if (parser == IRC_OK) {
      if (IRCMsg_Join(&command, userNick, NULL, NULL, channel) == IRC_OK) {
        send(sock, command, strlen(command), 0);
      }
      IRCTAD_GetTopic(channel, &topic);
      if (IRCMsg_RplTopic(&command, "REDES2", userNick, channel, topic) ==
          IRC_OK) {
        send(sock, command, strlen(command), 0);
      }
      if (IRCMsg_RplNamReply(&command, "REDES2", userNick, "=", channel,
                             getUsuariosCanal(channel)) == IRC_OK) {
        send(sock, command, strlen(command), 0);
      }
      if (IRCMsg_RplEndOfNames(&command, "REDES2", userNick, channel) ==
          IRC_OK) {
        send(sock, command, strlen(command), 0);
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
  char *prefix, *user, *modehost, *serverother, *realname, *command;
  time_t rawtime;

  int i = 0;
  HostNameIp *hi;
  hi = hostIp(sock);

  if (IRCParse_User(string, &prefix, &user, &modehost, &serverother,
                    &realname) != IRC_OK) {
    syslog(LOG_ERR, "***Error No existe una cadena para usar como User.");
    return;
  }

  if (IRCTADUser_New(user, userNick, realname, NULL, hi->name, hi->ip, sock) !=
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

void list(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, *topic, **list, *target;
  int i;

  if (IRCParse_List(string, &prefix, &channel, &target) == IRC_OK) {
    // Miramos que haya canales
    if (getNumeroCanales() > 0) {
      // Si nos epecifican el canal O MASCARA  search_string
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
void who(char *string, int sock, char *userNick) {
  char *prefix, *mask, *oppar, *command, *list;
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
      // AÑADIR AWAY
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

// TODO Controlar cuando solo se manda NAMES sin ningún argumento
void names(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, *list, *target;
  long num;

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
}

/*TODO comando part*/
// Abandona el canal.
// Si no se le pasa un canal entonces abandona el canal actual
// Debe borrar de la lista de usuarios del canal al usuario
void part(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, *list, *msg;
  long parser, nUsers;
  int i, socket;

  // Al salir el dueño peta

  // IRCParse_Part() realiza el parseo del string
  // Se pasa string de ella devuelve:
  // prefix = (null) ???
  // channel = el canal que se le pasa, si no obtiene el canal actual (auto)
  // msg = el mensaje que indica el motivo del PART, default es Leaving
  if (IRCParse_Part(string, &prefix, &channel, &msg) == IRC_OK) {

    // IRCTAD_Part() realiza la acción del comando PART, saca a un usuario de un
    // canal.
    // Se le pasa el channel y el nombre del usuario
    // Devuelve parser, CONTROL DE ERRORES
    parser = IRCTAD_Part(channel, userNick);

    if (parser == IRC_OK) {

      // IRCMsg_Part() construye el comando de respuesta
      parser = IRCMsg_Part(&command, userNick, channel, msg);

      // Elimina un usuario de la lista de usuarios de un canal
      IRCTAD_KickUserFromChannel(channel, userNick);

      // Envía el comando de respuesta del servidor al socket
      send(sock, command, strlen(command), 0);

      // Envía un mesnaje de que el cliente ha abandonado el canal a todos los
      // clientes del canal

      IRCTAD_ListNicksOnChannel(channel, &list, &nUsers);

      for (i = 0; i < nUsers; i++) {
        socket = getsocket(&list[i]); // & mandar msg tb?
        send(socket, command, strlen(command), 0);
      }

    }
    // No existe el usuario en el canal
    else if (parser == IRCERR_NOVALIDUSER) {
      IRCMsg_ErrNotOnChannel(&command, "REDES2", userNick, userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }
    // No existe el canal indicado
    else if (parser == IRCERR_NOVALIDCHANNEL) {
      IRCMsg_ErrNoSuchChannel(&command, "REDES2", userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }
    // No se puede eliminar el canal porque es permanente
    else if (parser == IRCERR_UNDELETABLECHANNEL) {
      IRCMsg_ErrNoChanModes(&command, "REDES2", userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }

    syslog(LOG_INFO, "PART FIN: Canal: %s, Mensaje: %s", channel, msg);
  }
}

// El operador del canal expulsa a un usuario, otro usuario, sin privilegios no
// puede expulsar
void kick(char *string, int sock, char *userNick) {

  char *prefix, *channel, *msg, *user, *command, *list;
  long parser, nUsers;
  int i, socket;

  // IRCParse_Kick() realiza el parseo del string
  // Se pasa string de ella devuelve:
  // prefix = (null) ???
  // channel = el canal que se le pasa, si no obtiene el canal actual (auto)
  // msg = el mensaje que indica el motivo del PART, default es Leaving
  // user = usuario al que se hace kick
  if (IRCParse_Kick(string, &prefix, &channel, &user, &msg) == IRC_OK) {

    // TODO Comprueba que el usuario tiene privilegios

    parser = IRCTAD_KickUserFromChannel(channel, user);

    // Expulsa al Usuario . SOLO prodria explusar el dueño del canal?
    if (parser == IRC_OK) {

      // IRCMsg_Kick() construye el comando de respuesta
      parser = IRCMsg_Kick(&command, "REDES 2", channel, user, msg);

      // Envía el comando de respuesta del servidor al socket
      send(sock, command, strlen(command), 0);

      // Envía un mesnaje de que el cliente ha abandonado el canal a todos los
      // clientes del canal
      IRCTAD_ListNicksOnChannel(channel, &list, &nUsers);

      for (i = 0; i < nUsers; i++) {
        socket = getsocket(&list[i]); /// &
        send(socket, command, strlen(command), 0);
      }

    }
    // No existe el usuario en el canal
    else if (parser == IRCERR_NOVALIDUSER) {
      IRCMsg_ErrNotOnChannel(&command, "REDES2", userNick, user, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }
    // No existe el canal indicado
    else if (parser == IRCERR_NOVALIDCHANNEL) {
      IRCMsg_ErrNoSuchChannel(&command, "REDES2", userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }
    // No se puede eliminar el canal porque es permanente
    else if (parser == IRCERR_UNDELETABLECHANNEL) {
      IRCMsg_ErrNoChanModes(&command, "REDES2", userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }

    // TODO Comprobar user destino
    syslog(LOG_INFO, "KICK FIN: Canal: %s, Mensaje: %s", channel, msg);
  }
}

void away(char *string, int sock, char *userNick) {
  char *reason = NULL, *command, *prefix;
  if (IRCParse_Away(string, &prefix, &reason) != IRC_OK) {
    syslog(LOG_ERR, "Error Away");
    return;
  }
  if (setAway(userNick, reason) != IRC_OK) {
    syslog(LOG_ERR, "Error setaway");
    return;
  }
  IRCMsg_Privmsg (char **command, char *prefix, char * msgtarget, char
  //  *msg) a ca da usuario
  // if (IRCMsg_RplUnaway(&command, userNick, userNick) == IRC_OK) {
  //   send(sock, command, strlen(command), 0);
  //   syslog(LOG_INFO, "%s", command);
  // }
  // are sent when the client removes and sets an AWAY message.
  if (IRCMsg_RplNowAway(&command, userNick, userNick) == IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }

  syslog(LOG_INFO, "setaway");
}
//   /QUIT reason
//
// Causes you to disconnect from the server.
// If you include a reason, it will be displayed on all channels as you quit
void quit(char *string, int sock, char *userNick) {
  char *reason, *prefix, **arraylist, *command;
  long num;
  int i;

  if (IRCParse_Quit(string, &prefix, &reason) != IRC_OK) {
    syslog(LOG_ERR, "Error Quit");
    return;
  }
  if (IRCTAD_ListChannelsOfUserArray(NULL, userNick, &arraylist, &num) !=
      IRC_OK) {
    syslog(LOG_ERR, "Error ListChanUser");
    return;
  }
  // mandar mensajr channel
  if (num > 0) {
    for (i = 0; i < num; i++) {
      send(sock, command, strlen(command), 0);
    }
  }
  IRCTAD_Quit(userNick);
  close(sock);
}

void motd(char *string, int sock, char *userNick) {
  char *prefix, *target, *command;

  if (IRCParse_Motd(string, &prefix, &target) != IRC_OK) {
    syslog(LOG_ERR, "Error Motd");
    return;
  }

  if (IRCMsg_RplMotdStart(&command, "REDES2", userNick, target) == IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
  if (IRCMsg_RplMotd(&command, "REDES2", userNick, "****BIENVENIDO****\n") ==
      IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
  if (IRCMsg_RplEndOfMotd(&command, "REDES2", userNick) == IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
}

void topic(char *string, int sock, char *userNick) {
  char *channel, *prefix, *topic, *topicActual, *command;

  if (IRCParse_Topic(string, &prefix, &channel, &topic) != IRC_OK) {
    syslog(LOG_ERR, "Error Topic");
    return;
  }

  if (topic != NULL) {
    if (IRCTAD_SetTopic(channel, userNick, topic) ==
        IRC_OK) { // Comprueba internamente si puede le usuario
      syslog(LOG_ERR, "Error SetTopic");
      return;
    }
  }
  if (IRCTAD_GetTopic(channel, &topic) != IRC_OK) {
    syslog(LOG_ERR, "Error Topic");
    return;
  } else if (topicActual != NULL) {

    if (IRCMsg_RplTopic(&command, "REDES2", userNick, channel, topic) ==
        IRC_OK) {
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
    }
  } else {
    if (IRCMsg_RplNoTopic(&command, "REDES2", userNick, channel) == IRC_OK) {
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
    }
  }
}
void mode(char *string, int sock, char *userNick) {}
void msg(char *string, int sock, char *userNick) {
  char *command, *nickorchannel, *msg, **arraylist;
  long nUsers;
  if (IRCUserParse_Msg(string, &nickorchannel, &msg) == IRC_OK) {
    if (IRCTADUser_Test(0, NULL, nickorchannel) == IRC_OK) {
      if (IRCMsg_Privmsg(&command, "REDES2", nickorchannel, msg) == IRC_OK)
        send(getsocket(nickorchannel), command, strlen(command), 0);
    } else if (IRCTAD_TestUserOnChannel(nickorchannel, userNick) == IRC_OK) {
      // Si el usuario tiene mensaje away
      // IRCMsg_RplAway (char **command, char *prefix, char * nick, char
      // *nick2,
      // char *msg);
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
void nocommand(char *string, int sock, char *userNick) {
  char *command;
  if (IRCMsg_RplTryAgain(&command, "REDES2", userNick,
                         "Comando no reconocido") != IRC_OK) {
    syslog(LOG_ERR, "ERROR noCommand");
    return;
  }
  send(sock, command, strlen(command), 0);
}

void doCommand(char *string, int sock, char **userNick) {

  if (string == NULL)
    return;

  switch (IRC_CommandQuery(string)) {

  case NICK:
    syslog(LOG_INFO, "NICK");
    nick(string, sock, userNick);
    break;
  case USER:
    syslog(LOG_INFO, "USER");
    user(string, sock, *userNick);
    break;
  case PING:
    syslog(LOG_INFO, "PING");
    ping(string, sock, *userNick);
    break;
  case TOPIC:
    syslog(LOG_INFO, "TOPIC");
    break;
  case LIST:
    syslog(LOG_INFO, "LIST");
    list(string, sock, *userNick);
    break;
  case JOIN:
    syslog(LOG_INFO, "JOIN");
    join(string, sock, *userNick);
    break;
  case WHOIS:
    syslog(LOG_INFO, "WHO IS");
    whois(string, sock, *userNick);
    break;
  case WHO:
    syslog(LOG_INFO, "WHO");
    who(string, sock, *userNick);
    break;
  case NAMES:
    syslog(LOG_INFO, "NAMES");
    names(string, sock, *userNick);
    break;
  case PART:
    syslog(LOG_INFO, "PART");
    part(string, sock, *userNick);
    break;
  case KICK:
    syslog(LOG_INFO, "KICK");
    kick(string, sock, *userNick);
    break;
  case AWAY:
    syslog(LOG_INFO, "AWAY");
    away(string, sock, *userNick);
    break;
  case QUIT:
    syslog(LOG_INFO, "QUIT");
    quit(string, sock, *userNick);
    break;
  case MOTD:
    syslog(LOG_INFO, "MOTD");
    motd(string, sock, *userNick);
    break;
  case MODE:
    syslog(LOG_INFO, "MOTD");
    mode(string, sock, *userNick);
    break;
  case PRIVMSG:
    syslog(LOG_INFO, "MSG");
    msg(string, sock, *userNick);
    break;
  default:
    syslog(LOG_INFO, "No command reconozaice");
    nocommand(string, sock, *userNick);
    break;
  }
}
int setAway(char *nick, char *reason) {
  char *user, *real, *host, *IP, *away;
  long id, actionTS, creationTS;
  int socket;
  if (IRCTADUser_GetData(&id, &user, &nick, &real, &host, &IP, &socket,
                         &creationTS, &actionTS, &away) == IRC_OK) {

    return IRCTADUser_SetAway(id, user, nick, real, reason);
  }
  return 0;
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
void setNick(char *nick, char **userNick) {
  long id = 0, creationTS = 0, actionTS = 0;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  int socket = 0;

  if (IRCTADUser_GetData(&id, &user, userNick, &real, &host, &IP, &socket,
                         &creationTS, &actionTS, &away) == IRC_OK)
    if (IRCTADUser_Set(id, user, *userNick, real, user, nick, real) == IRC_OK) {
      free(*userNick);
      *userNick = (char *)malloc(sizeof(strlen(nick) + 1));
      strcpy(*userNick, nick);
    }
}
char *isAway(char *nick) {
  char *user = NULL, *real = NULL, *away;
  int id = -1;
  IRCTADUser_GetAway(id, user, nick, real, &away);
  return away;
}
// LIBERAR ESTRUCTURAS
