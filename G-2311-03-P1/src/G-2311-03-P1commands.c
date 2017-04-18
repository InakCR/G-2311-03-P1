#include "../includes/G-2311-03-P1commands.h"

char motdServer[50] = "**BIENVENIDO AL SERVIDOR**";
char prefixC[10] = "REDES2";

// TODO Alarma
void ping(char *string, int sock, char *userNick) {
  char *prefix, *server, *server2, *msg, *command;

  if (IRCParse_Ping(string, &prefix, &server, &server2, &msg) == IRC_OK) {
    if (IRCMsg_Pong(&command, prefixC, prefixC, server2, server) == IRC_OK) {
      send(sock, command, strlen(command), 0);
    }
    free(command);
  }
  free(prefix);
  free(server);
  free(server2);
}

void list(char *string, int sock, char *userNick) {

  char *prefix, *channel, *command, *topic, **list, *target, numc[10] = "%ld";
  int i, tadret;
  long num;

  if (IRCParse_List(string, &prefix, &channel, &target) == IRC_OK) {

    // Miramos que haya canales
    if (getNumeroCanales() > 0) {

      // Si nos epecifican el canal O MASCARA  search_string
      if (channel != NULL) {

        tadret = IRCTAD_GetTopic(channel, &topic);

        if (tadret == IRCERR_INVALIDCHANNELNAME){

          IRCMsg_ErrNoSuchChannel(&command, prefixC, userNick, channel);
          send(sock, command, strlen(command), 0);

          //IRC_MFree(5, &prefix, &channel, &target, &topic, &command);

        } else if (tadret == IRCERR_NOENOUGHMEMORY){

          IRCMsg_ErrChannelIsFull(&command, prefixC, userNick, channel);
          send(sock, command, strlen(command), 0);

          //IRC_MFree(5, &prefix, &channel, &target, &topic, &command);

        } else {

          if (topic != NULL) {

            num = getNumUsuariosCanal(channel);
            sprintf(numc, numc, num);

            if (IRCTADChan_GetModeInt(channel) < 127) {
              IRCMsg_RplList(&command, prefixC, userNick, channel, numc, topic);
              send(sock, command, strlen(command), 0);
            }

            //IRC_MFree(5, &prefix, &channel, &target, &topic, &command);
          }

          //IRC_MFree(4, &prefix, &channel, &target, &topic);
        }

      } else {

        list = getListaCanales();

        for (i = 0; i < getNumeroCanales(); i++) {

          //No error
          IRCTAD_GetTopic(list[i], &topic);

          num = getNumUsuariosCanal(list[i]);
          sprintf(numc, numc, num);

          if (IRCTADChan_GetModeInt(list[i]) < 127) {
            IRCMsg_RplList(&command, prefixC, userNick, list[i], numc, topic);
            send(sock, command, strlen(command), 0);
          }
        }

        //IRC_MFree(5, &prefix, &channel, &target, &topic, &command, &list);
      }

      IRCMsg_RplListEnd(&command, prefixC, userNick);
      send(sock, command, strlen(command), 0);

    } else {

      syslog(LOG_INFO, "No hay canales");

      //IRC_MFree(3, &prefix, &channel, &target);

    }

  } else {

    syslog(LOG_ERR, "***Fallo en el Parseo. List");

    //IRC_MFree(3, &prefix, &channel, &target);

  }
}

void who(char *string, int sock, char *userNick) {

  char *prefix, *mask, *oppar, *command, *list;
  long num = 0, tadret;

  if (IRCParse_Who(string, &prefix, &mask, &oppar) == IRC_OK) {

    if (mask != NULL) {

      tadret = IRCTAD_ListNicksOnChannel(mask, &list, &num);

      if (tadret == IRC_OK) {

        if (num > 0) {

          IRCMsg_RplWhoIsChannels(&command, prefixC, userNick, mask, list);
          send(sock, command, strlen(command), 0);
          syslog(LOG_INFO, "%s", command);

          IRC_MFree(5, &prefix, &mask, &oppar, &list, &command);

        }

        IRC_MFree(4, &prefix, &mask, &oppar, &list);

      } else if (tadret == IRCERR_NOENOUGHMEMORY){

        IRCMsg_ErrChannelIsFull(&command, prefixC, userNick, mask);
        send(sock, command, strlen(command), 0);

        IRC_MFree(5, &prefix, &mask, &oppar, &list, &command);

      } else if (tadret == IRCERR_NOVALIDCHANNEL){

        IRCMsg_ErrNoSuchChannel(&command, prefixC, userNick, mask);
        send(sock, command, strlen(command), 0);

        IRC_MFree(5, &prefix, &mask, &oppar, &list, &command);

      }
    }

  } else {

    syslog(LOG_ERR, "***Fallo en el Parseo. Who");

    IRC_MFree(3, &prefix, &mask, &oppar);

  }
}

void quit(char *string, int sock, char *userNick) {
  char *reason, *prefix, **arraylist, **arraylistNicks, *command;
  long num, numNicks, tadret;
  int i, j, socket;

  if (IRCParse_Quit(string, &prefix, &reason) != IRC_OK) {

    syslog(LOG_ERR, "***Fallo en el Parseo. Quit");

    IRC_MFree(2, &prefix, &reason);
  }

  tadret = IRCTAD_ListChannelsOfUserArray(NULL, userNick, &arraylist, &num);

  if (tadret == IRCERR_NOVALIDUSER){

    IRCMsg_ErrNoNickNameGiven(&command, prefixC, userNick);
    send(sock, command, strlen(command), 0);

    IRC_MFree(4, &prefix, &reason, &arraylist, &command);

  } else {

    IRCTAD_Quit(userNick);
    // if (reason != NULL && num > 0) {
    //   for (i = 0; i < num; i++) {
    //
    //     if (IRCTAD_ListNicksOnChannelArray(arraylist[i], &arraylistNicks,
    //                                        &numNicks) == IRC_OK) {
    //
    //       if (IRCMsg_Quit(&command, userNick, reason) == IRC_OK) {
    //
    //         for (j = 0; j < numNicks; j++) {
    //
    //           socket = getsocket(arraylistNicks[i]);
    //           send(socket, command, strlen(command), 0);
    //         }
    //       }
    //     }
    //   }
    // }

    if (IRCMsg_Kill(&command, prefix, userNick, "Desconectado") == IRC_OK) {

      send(sock, command, strlen(command), 0);

      IRC_MFree(4, &prefix, &reason, &arraylist, &command);
    }

    close(sock);
  }
}

void motd(char *string, int sock, char *userNick) {

  char *prefix, *target, *command;

  if (IRCParse_Motd(string, &prefix, &target) != IRC_OK) {

    syslog(LOG_ERR, "***Fallo en el Parseo. Motd");

    IRC_MFree(2, &prefix, &target);
  }

  if (IRCMsg_RplMotdStart(&command, prefixC, userNick, prefixC) == IRC_OK) {
    send(sock, command, strlen(command), 0);
  }

  if (IRCMsg_RplMotd(&command, prefixC, userNick, motdServer) == IRC_OK) {
    send(sock, command, strlen(command), 0);
  }

  if (IRCMsg_RplEndOfMotd(&command, prefixC, userNick) == IRC_OK) {
    send(sock, command, strlen(command), 0);
  }

  IRC_MFree(3, &prefix, &target, &command);
}

void msg(char *string, int sock, char *userNick) {

  char *nickorchannel, *prefix, *msg, *command;

  if (IRCParse_Privmsg(string, &prefix, &nickorchannel, &msg) == IRC_OK) {

    if (IRCTADUser_Test(0, NULL, nickorchannel) == IRC_OK) {

      msgUser(nickorchannel, userNick, msg);

    } else if (IRCTAD_TestChannelOfUser(nickorchannel, userNick) == IRC_OK) {

      msgCanal(nickorchannel, userNick, msg);

    } else {

      if (IRCMsg_ErrNoSuchNick(&command, prefixC, userNick, nickorchannel) ==
          IRC_OK) {
        send(sock, command, strlen(command), 0);
      }

      IRC_MFree(4, &prefix, &nickorchannel, &msg, &command);
    }

  } else {

    syslog(LOG_ERR, "***Fallo en el Parseo. Msg");

    IRC_MFree(3, &prefix, &nickorchannel, &msg);
  }
}

void nocommand(char *string, int sock, char *userNick) {

  char *command;

  if (IRCMsg_ErrUnKnownCommand(&command, prefixC, userNick, string) == IRC_OK) {

    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);

    IRC_MFree(1, &command);
  }

  if (IRCMsg_RplTryAgain(&command, prefixC, userNick,
                         "Comando no reconocido") == IRC_OK) {

    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);

    IRC_MFree(1, &command);
  }
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
    topic(string, sock, *userNick);
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
    syslog(LOG_INFO, "MODE");
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
void setMotd(char *motd) {
  // motdServer = (char *)malloc(strlen(motd + 1) * sizeof(char));
  // strcpy(motdServer, motd);
  // setMotdUser(motd);
}