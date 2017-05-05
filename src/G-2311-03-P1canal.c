#include "../includes/G-2311-03-P1canal.h"

char prefixS[10] = "REDES2";

void join(char *string, int sock, char *userNick) {
  char *prefix, *msg, *channel, *key, *command, *topic, *list, **listArray,
      *lista;
  long parser, tadret, nUsers, num;
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

    tadret = IRCTAD_Join(channel, userNick, "o", key);

    if (tadret == IRC_OK) {

      if (IRCMsg_Join(&command, userNick, NULL, NULL, channel) == IRC_OK) {
        send(sock, command, strlen(command), 0);
      }

      IRCTAD_GetTopic(channel, &topic);

      if (topic != NULL) {
        if (IRCMsg_RplTopic(&command, prefixS, userNick, channel, topic) ==
            IRC_OK) {
          send(sock, command, strlen(command), 0);
        }
      } else {
        if (IRCMsg_RplNoTopic(&command, prefixS, userNick, channel) == IRC_OK) {
          send(sock, command, strlen(command), 0);
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

      IRC_MFree(7, &prefix, &channel, &key, &msg, &command, &list, &listArray);

    } else if (tadret == IRCERR_NOVALIDUSER) {

      IRCMsg_ErrNoNickNameGiven(&command, prefixS, userNick);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);

    } else if (tadret == IRCERR_NOVALIDCHANNEL) {

      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);

    } else if (tadret == IRCERR_USERSLIMITEXCEEDED) {

      IRCMsg_ErrChannelIsFull(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);

    } else if (tadret == IRCERR_NOENOUGHMEMORY) {

      IRCMsg_ErrChannelIsFull(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);

    } else if (tadret == IRCERR_BANEDUSERONCHANNEL) {

      IRCMsg_ErrBannedFromChan(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);

    } else if (tadret == IRCERR_NOINVITEDUSER) {

      IRCMsg_ErrInviteOnlyChan(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);

    } else if (tadret == IRCERR_YETINCHANNEL) {

      IRCMsg_ErrUserOnChannel(&command, prefixS, userNick, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);

    } else if (tadret == IRCERR_INVALIDCHANNELNAME) {

      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);

    } else if (tadret == IRCERR_INVALIDNICK) {

      IRCMsg_ErrUserNotInChannel(&command, prefixS, userNick, userNick,
                                 channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &key, &msg, &command);
    }

  } else {

    syslog(LOG_ERR, "***Fallo en el Parseo. Join");

    IRC_MFree(5, &prefix, &channel, &key, &msg, &command);
  }
}

void names(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, *list, *target;
  long num, tadret;

  if (IRCParse_Names(string, &prefix, &channel, &target) == IRC_OK) {

    tadret = IRCTAD_ListNicksOnChannel(channel, &list, &num);

    if (tadret == IRC_OK) {

      if (num > 0) {
        IRCMsg_RplNamReply(&command, prefixS, userNick, "=", channel, list);
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
        IRC_MFree(1, &list);
      }
    } else if (tadret == IRCERR_NOENOUGHMEMORY) {

      IRCMsg_ErrChannelIsFull(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

    } else if (tadret == IRCERR_NOVALIDCHANNEL) {

      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);
    }

    IRCMsg_RplEndOfNames(&command, prefixS, userNick, channel);
    send(sock, command, strlen(command), 0);

    IRC_MFree(3, &prefix, &channel, &command);
    if (target != NULL)
      IRC_MFree(1, &target);
  } else {

    syslog(LOG_ERR, "***Fallo en el Parseo. Names");

    IRC_MFree(2, &prefix, &channel);
    if (target != NULL)
      IRC_MFree(1, &target);
  }
}

void part(char *string, int sock, char *userNick) {

  char *prefix, *channel, *command, **list, *msg;
  long parser, nUsers;
  int i, socket;

  if (IRCParse_Part(string, &prefix, &channel, &msg) == IRC_OK) {

    parser = IRCTAD_Part(channel, userNick);

    if (parser == IRC_OK) {

      IRCMsg_Part(&command, userNick, channel, msg);
      send(sock, command, strlen(command), 0);

      if (IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers) == IRC_OK) {
        for (i = 0; i < nUsers; i++) {
          socket = getsocket(list[i]);
          send(socket, command, strlen(command), 0);
        }
        if (list != NULL)
          IRC_MFree(1, &list);
      }

      IRC_MFree(4, &prefix, &channel, &msg, &command);
    }
    // No existe el usuario en el canal
    else if (parser == IRCERR_NOVALIDUSER) {

      IRCMsg_ErrNotOnChannel(&command, prefixS, userNick, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(4, &prefix, &channel, &msg, &command);

    }
    // No existe el canal indicado
    else if (parser == IRCERR_NOVALIDCHANNEL) {

      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(4, &prefix, &channel, &msg, &command);

    }
    // No se puede eliminar el canal porque es permanente
    else if (parser == IRCERR_UNDELETABLECHANNEL) {

      IRCMsg_ErrNoChanModes(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(4, &prefix, &channel, &msg, &command);
    }

  } else {

    syslog(LOG_ERR, "***Fallo en el Parseo. Part");

    IRC_MFree(3, &prefix, &channel, &msg);
  }
}

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

      parser = IRCMsg_Kick(&command, userNick, channel, user, msg);
      socket = getsocket(user);
      send(socket, command, strlen(command), 0);

      IRCMsg_Part(&command, user, channel, msg);
      IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers);

      for (i = 0; i < nUsers; i++) {
        socket = getsocket(list[i]); /// &
        send(socket, command, strlen(command), 0);
      }

      IRC_MFree(6, &prefix, &channel, &user, &msg, &command, &list);

      // No existe el usuario en el canal
    } else if (parser == IRCERR_NOVALIDUSER) {

      IRCMsg_ErrNotOnChannel(&command, prefixS, userNick, user, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &user, &msg, &command);

    }
    // No existe el canal indicado
    else if (parser == IRCERR_NOVALIDCHANNEL) {

      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &user, &msg, &command);

    }
    // No se puede eliminar el canal porque es permanente
    else if (parser == IRCERR_UNDELETABLECHANNEL) {

      IRCMsg_ErrNoChanModes(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(5, &prefix, &channel, &user, &msg, &command);
    }

  } else {

    syslog(LOG_ERR, "***Fallo en el Parseo. Kick");

    IRC_MFree(4, &prefix, &channel, &user, &msg);
  }
}

void topic(char *string, int sock, char *userNick) {

  char *channel, *prefix, *topic, *topicActual, *command, **list;
  long nUsers, tadret;
  int i, socket;

  if (IRCParse_Topic(string, &prefix, &channel, &topic) != IRC_OK) {

    syslog(LOG_ERR, "***Fallo en el Parseo. Topic");

    IRC_MFree(3, &prefix, &channel, &topic);
  }

  if (topic != NULL) {

    // Comprueba internamente si puede le usuario
    if (IRCTAD_SetTopic(channel, userNick, topic) != IRC_OK) {

      IRCMsg_ErrChanOPrivsNeeded(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(4, &prefix, &channel, &topic, &command);

      syslog(LOG_ERR, "Error SetTopic");
      return;
    }

    if (IRCMsg_Topic(&command, userNick, channel, topic) == IRC_OK) {

      tadret = IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers);

      if (tadret == IRC_OK) {

        for (i = 0; i < nUsers; i++) {
          socket = getsocket(list[i]);
          send(socket, command, strlen(command), 0);
        }

        IRC_MFree(5, &prefix, &channel, &topic, &command, &list);

      } else if (tadret == IRCERR_NOENOUGHMEMORY) {

        IRCMsg_ErrChannelIsFull(&command, prefixS, userNick, channel);
        send(sock, command, strlen(command), 0);

        IRC_MFree(5, &prefix, &channel, &topic, &command, &list);

      } else if (tadret == IRCERR_NOVALIDCHANNEL) {

        IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
        send(sock, command, strlen(command), 0);

        IRC_MFree(5, &prefix, &channel, &topic, &command, &list);
      }
    }

  } else {

    tadret = IRCTAD_GetTopic(channel, &topicActual);

    if (tadret == IRCERR_INVALIDCHANNELNAME) {

      IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(4, &prefix, &channel, &topicActual, &command);

    } else if (tadret == IRCERR_NOENOUGHMEMORY) {

      IRCMsg_ErrChannelIsFull(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

      IRC_MFree(4, &prefix, &channel, &topicActual, &command);

    } else {

      if (topicActual != NULL) {

        if (IRCMsg_RplTopic(&command, prefixS, userNick, channel,
                            topicActual) == IRC_OK) {

          send(sock, command, strlen(command), 0);

          IRC_MFree(4, &prefix, &channel, &topicActual, &command);
        }

      } else {

        if (IRCMsg_RplNoTopic(&command, prefixS, userNick, channel) == IRC_OK) {

          send(sock, command, strlen(command), 0);

          IRC_MFree(4, &prefix, &channel, &topicActual, &command);
        }
      }
    }
  }
}

void mode(char *string, int sock, char *userNick) {

  char *prefix, *channeluser, *mode, *user, *command, *modeChannel;

  if (IRCParse_Mode(string, &prefix, &channeluser, &mode, &user) == IRC_OK) {

    if (mode == NULL && user == NULL) {

      if (IRCTADUser_Test(0, NULL, channeluser) == IRC_OK) {

        IRC_MFree(4, &prefix, &channeluser, &mode, &user);

      } else {

        modeChannel = IRCTADChan_GetModeChar(channeluser);

        if (modeChannel != NULL) {

          IRCMsg_RplChannelModeIs(&command, prefixS, userNick, channeluser,
                                  modeChannel);
          send(sock, command, strlen(command), 0);
          syslog(LOG_INFO, "%s", command);

          IRC_MFree(5, &prefix, &channeluser, &mode, &user, &command);
        }

        IRC_MFree(4, &prefix, &channeluser, &mode, &user);
      }

    } else if (user == NULL) {

      IRCTAD_Mode(channeluser, userNick, mode);

      IRCMsg_Mode(&command, prefixS, channeluser, mode, NULL);
      send(sock, command, strlen(command), 0);
      syslog(LOG_INFO, "%s", command);

      IRC_MFree(5, &prefix, &channeluser, &mode, &user, &command);

    } else {

      IRCTAD_Mode(channeluser, userNick, mode);

      if (strcmp(mode, "\\+k") == 0) {

        IRCTADChan_SetPassword(channeluser, user);

        IRCMsg_Mode(&command, userNick, channeluser, mode, user);
        send(sock, command, strlen(command), 0);

        IRC_MFree(5, &prefix, &channeluser, &mode, &user, &command);
      }

      IRC_MFree(4, &prefix, &channeluser, &mode, &user);
    }
  }
}

void msg_canal(char *channel, char *userNick, char *msg) {

  char *command, **list;
  int i, socket, sock;
  long nUsers, tadret;

  sock = getsocket(userNick);

  tadret = IRCTAD_ListNicksOnChannelArray(channel, &list, &nUsers);

  if (tadret == IRC_OK) {

    tadret = IRCMsg_Privmsg(&command, userNick, channel, msg);

    if (tadret == IRC_OK) {

      for (i = 0; i < nUsers; i++) {

        if (strcmp(list[i], userNick) != 0) {
          socket = getsocket(list[i]);
          send(socket, command, strlen(command), 0);
        }
      }

    } else if (tadret == IRCERR_NOTARGET) {

      IRCMsg_ErrCanNotSendToChan(&command, prefixS, userNick, channel);
      send(sock, command, strlen(command), 0);

    } else if (tadret == IRCERR_NOMESSAGE) {

      IRCMsg_ErrNoTextToSend(&command, prefixS, userNick);
      send(sock, command, strlen(command), 0);
    }

    IRC_MFree(2, &command, &list);

  } else if (tadret == IRCERR_NOENOUGHMEMORY) {

    IRCMsg_ErrChannelIsFull(&command, prefixS, userNick, channel);
    send(sock, command, strlen(command), 0);

    IRC_MFree(2, &command, &list);

  } else if (tadret == IRCERR_NOVALIDCHANNEL) {

    IRCMsg_ErrNoSuchChannel(&command, prefixS, userNick, channel);
    send(sock, command, strlen(command), 0);

    IRC_MFree(2, &command, &list);
  }
}
