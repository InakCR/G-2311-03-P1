#include "../includes/canal.h"
char prefixS[10] = "REDES2";
void join(char *string, int sock, char *userNick) {
  char *prefix, *msg, *channel, *key, *command, *topic, *list, **listArray,
      *lista;
  long parser, nUsers, num;
  int i, socket;

  parser = IRCParse_Join(string, &prefix, &channel, &key, &msg);

  if (channel == NULL && key == NULL && msg == NULL) {

    if (IRCMsg_ErrNeedMoreParams(&command, userNick, userNick, string) ==
        IRC_OK) {
      send(sock, command, strlen(command), 0);
      return;
    }
  } else if (parser == IRC_OK) {
    if (IRCTADChan_TestPassword(channel, key) == IRCERR_FAIL) {
      IRCMsg_ErrBadChannelKey(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);
      return;
    }
    parser = IRCTAD_Join(channel, userNick, "o", key);
    // Usermoden o va haber administradores
    if (parser == IRC_OK) {
      if (IRCMsg_Join(&command, userNick, NULL, NULL, channel) == IRC_OK) {
        send(sock, command, strlen(command), 0);
      }
      IRCTAD_GetTopic(channel, &topic);
      if (topic != NULL) {
        if (IRCMsg_RplTopic(&command, prefixS, userNick, channel, topic) ==
            IRC_OK) {
          send(sock, command, strlen(command), 0);
          syslog(LOG_INFO, "%s", command);
        }
      } else {
        if (IRCMsg_RplNoTopic(&command, prefixS, userNick, channel) == IRC_OK) {
          send(sock, command, strlen(command), 0);
          syslog(LOG_INFO, "%s", command);
        }
      }
      IRCTAD_ListNicksOnChannel(channel, &list, &num);
      lista = (char *)malloc(strlen(list + num) * sizeof(char));
      strcpy(lista, "");
      IRCTAD_ListNicksOnChannelArray(channel, &listArray, &num);
      for (i = 0; i < num; i++) {

        if (IRCTAD_GetUserModeOnChannel(channel, listArray[i]) >=
            IRCUMODE_OPERATOR) {
          strcat(lista, "@");
        }
        strcat(lista, listArray[i]);
        strcat(lista, " ");
      }
      if (IRCMsg_RplNamReply(&command, prefixS, userNick, "=", channel,
                             lista) == IRC_OK) {
        if (IRCTAD_ListNicksOnChannelArray(channel, &listArray, &nUsers) ==
            IRC_OK) {
          for (i = 0; i < nUsers; i++) {
            socket = getsocket(listArray[i]);
            send(socket, command, strlen(command), 0);
          }
        }
      }
      if (IRCMsg_RplEndOfNames(&command, prefixS, userNick, channel) ==
          IRC_OK) {
        send(sock, command, strlen(command), 0);
      }
      free(command);
    } else {
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
// TODO Controlar cuando solo se manda NAMES sin ningún argumento
void names(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, *list, *target;
  long num;

  if (IRCParse_Names(string, &prefix, &channel, &target) == IRC_OK) {
    if (IRCTAD_ListNicksOnChannel(channel, &list, &num) == IRC_OK) {
      if (num > 0) {
        IRCMsg_RplNamReply(&command, prefixS, userNick, "=", channel, list);
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
    }
    IRCMsg_RplEndOfNames(&command, prefixS, userNick, channel);
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
}

void part(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, **list, *msg;
  long parser, nUsers;
  int i, socket;

  if (IRCParse_Part(string, &prefix, &channel, &msg) == IRC_OK) {

    // IRCTAD_Part() realiza la acción del comando PART, saca a un usuario de un
    // canal.
    // Se le pasa el channel y el nombre del usuario
    // Devuelve parser, CONTROL DE ERRORES
    parser = IRCTAD_Part(channel, userNick);
    syslog(LOG_ERR, "PART 1");
    if (parser == IRC_OK) {
      syslog(LOG_ERR, "PART 2");
      // IRCMsg_Part() construye el comando de respuesta
      parser = IRCMsg_Part(&command, userNick, channel, msg);

      // Elimina un usuario de la lista de usuarios de un canal
      // Lo hace part, mira la documentacion IRCTAD_KickUserFromChannel(channel,
      // userNick);

      // Envía el comando de respuesta del servidor al socket
      send(sock, command, strlen(command), 0);

      // Envía un mesnaje de que el cliente ha abandonado el canal a todos los
      // clientes del canal

      if (IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers) == IRC_OK) {
        syslog(LOG_ERR, "PART 3 %s,%ld", list[0], nUsers);
        for (i = 0; i < nUsers; i++) {
          socket = getsocket(list[i]); // & mandar msg tb?
          send(socket, command, strlen(command), 0);
        }
        syslog(LOG_ERR, "PART 4");
      }
    }
    // No existe el usuario en el canal
    else if (parser == IRCERR_NOVALIDUSER) {
      IRCMsg_ErrNotOnChannel(&command, prefixS, userNick, userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }
    // No existe el canal indicado
    else if (parser == IRCERR_NOVALIDCHANNEL) {
      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }
    // No se puede eliminar el canal porque es permanente
    else if (parser == IRCERR_UNDELETABLECHANNEL) {
      IRCMsg_ErrNoChanModes(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }

    syslog(LOG_INFO, "PART FIN: Canal: %s, Mensaje: %s", channel, msg);
  }
}

// El operador del canal expulsa a un usuario, otro usuario, sin privilegios no
// puede expulsar
void kick(char *string, int sock, char *userNick) {

  char *prefix, *channel, *msg, *user, *command, **list;
  long parser, nUsers, mode;
  int i, socket;

  if (IRCParse_Kick(string, &prefix, &channel, &user, &msg) == IRC_OK) {

    mode = IRCTAD_GetUserModeOnChannel(channel, userNick);

    if (mode < IRCUMODE_OPERATOR) { //== (IRCUMODE_CREATOR | IRCUMODE_OPERATOR
      IRCMsg_ErrChanOPrivsNeeded(&command, userNick, userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
      return;
    }

    parser = IRCTAD_KickUserFromChannel(channel, user);
    if (parser == IRC_OK) {
      // IRCMsg_Kick() construye el comando de respuesta
      parser = IRCMsg_Kick(&command, userNick, channel, user, msg);
      // Envía el comando de respuesta del servidor al socket
      socket = getsocket(user);
      send(socket, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
      //  Envía un mesnaje de que el cliente ha abandonado el canal a todos
      //  los
      //  clientes del canal
      IRCMsg_Part(&command, user, channel, msg);
      IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers);
      for (i = 0; i < nUsers; i++) {
        socket = getsocket(list[i]); /// &
        send(socket, command, strlen(command), 0);
      }

      // No existe el usuario en el canal
    } else if (parser == IRCERR_NOVALIDUSER) {
      IRCMsg_ErrNotOnChannel(&command, prefixS, userNick, user, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }
    // No existe el canal indicado
    else if (parser == IRCERR_NOVALIDCHANNEL) {
      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }
    // No se puede eliminar el canal porque es permanente
    else if (parser == IRCERR_UNDELETABLECHANNEL) {
      IRCMsg_ErrNoChanModes(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "***Error - %s", command);
    }

    // TODO Comprobar user destino
    syslog(LOG_INFO, "KICK FIN: Canal: %s, Mensaje: %s", channel, msg);
  }
}
void topic(char *string, int sock, char *userNick) {
  char *channel, *prefix, *topic, *topicActual, *command, **list;
  long nUsers;
  int i, socket;

  if (IRCParse_Topic(string, &prefix, &channel, &topic) != IRC_OK) {
    syslog(LOG_ERR, "Error Topic");
    return;
  }
  if (topic != NULL) {
    if (IRCTAD_SetTopic(channel, userNick, topic) !=
        IRC_OK) { // Comprueba internamente si puede le usuario
      IRCMsg_ErrChanOPrivsNeeded(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);
      syslog(LOG_ERR, "Error SetTopic");
      return;
    }
    if (IRCMsg_Topic(&command, userNick, channel, topic) == IRC_OK) {
      if (IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers) == IRC_OK) {
        for (i = 0; i < nUsers; i++) {
          socket = getsocket(list[i]);
          send(socket, command, strlen(command), 0);
        }
      }
      syslog(LOG_INFO, "%s", command);
    }
  } else {
    if (IRCTAD_GetTopic(channel, &topicActual) != IRC_OK) {
      syslog(LOG_ERR, "Error Topic");
      return;
    }
    if (topicActual != NULL) {

      if (IRCMsg_RplTopic(&command, prefixS, userNick, channel, topicActual) ==
          IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
    } else {
      if (IRCMsg_RplNoTopic(&command, prefixS, userNick, channel) == IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
    }
  }
}
void mode(char *string, int sock, char *userNick) {
  char *prefix, *channeluser, *mode, *user, *command, *modeChannel;
  if (IRCParse_Mode(string, &prefix, &channeluser, &mode, &user) == IRC_OK) {
    if (mode == NULL && user == NULL) {
      if (IRCTADUser_Test(0, NULL, channeluser) == IRC_OK) {

      } else {
        modeChannel = IRCTADChan_GetModeChar(channeluser);
        if (modeChannel != NULL) {

          IRCMsg_RplChannelModeIs(&command, prefixS, userNick, channeluser,
                                  modeChannel);
          send(sock, command, strlen(command), 0);
          syslog(LOG_INFO, "%s", command);
        }
      }
    } else if (user == NULL) {
      IRCTAD_Mode(channeluser, userNick, mode);
      IRCMsg_Mode(&command, prefixS, channeluser, mode, NULL);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);
    } else {
      IRCTAD_Mode(channeluser, userNick, mode);
      if (strcmp(mode, "\\+k") == 0) {
        IRCTADChan_SetPassword(channeluser, user);
        IRCMsg_Mode(&command, userNick, channeluser, mode, user);

        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
    }
  }
}
void msgCanal(char *channel, char *userNick, char *msg) {
  char *command, **list;
  int i, socket;
  long nUsers;
  if (IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers) == IRC_OK) {
    if (IRCMsg_Privmsg(&command, userNick, channel, msg) == IRC_OK) {
      for (i = 0; i < nUsers; i++) {
        if (strcmp(list[i], userNick) != 0) {
          socket = getsocket(list[i]);
          send(socket, command, strlen(command), 0);
        }
      }
    }
  }
}
