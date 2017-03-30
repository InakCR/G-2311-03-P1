#include "../includes/commands.h"

char motdServer[50] = "**BIENVENIDO AL SERVIDOR**";

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

void list(char *string, int sock, char *userNick) {
  char *prefix, *channel, *command, *topic, **list, *target, numc[10] = "%ld";
  int i, modei;
  long num;

  if (IRCParse_List(string, &prefix, &channel, &target) == IRC_OK) {
    // Miramos que haya canales
    if (getNumeroCanales() > 0) {
      // Si nos epecifican el canal O MASCARA  search_string
      if (channel != NULL) {
        syslog(LOG_INFO, "Canal encontrado");
        IRCTAD_GetTopic(channel, &topic);
        if (topic != NULL) {

          num = getNumUsuariosCanal(list[i]);
          sprintf(numc, numc, num);
          if (IRCTADChan_GetModeInt(channel) < 127) {
            IRCMsg_RplList(&command, "REDES2", userNick, channel, "2-3", topic);
            send(sock, command, strlen(command), 0);
          }
        }
        free(channel);
      } else {
        syslog(LOG_INFO, "Varios Canales");
        list = getListaCanales();
        for (i = 0; i < getNumeroCanales(); i++) {
          IRCTAD_GetTopic(list[i], &topic);
          num = getNumUsuariosCanal(list[i]);
          sprintf(numc, numc, num);
          if (IRCTADChan_GetModeInt(list[i]) < 127) {
            IRCMsg_RplList(&command, "REDES2", userNick, list[i], numc, topic);
            send(sock, command, strlen(command), 0);
          }
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
  char *prefix, *mask, *oppar, *command, *list, **listArray;
  char *lista = NULL;
  long num = 0;
  int i;

  if (IRCParse_Who(string, &prefix, &mask, &oppar) == IRC_OK) {
    if (mask != NULL) {
      if (IRCTAD_ListNicksOnChannel(mask, &list, &num) == IRC_OK) {
        if (num > 0) {

          IRCMsg_RplWhoIsChannels(&command, "REDES2", userNick, mask, list);
          send(sock, command, strlen(command), 0);
          syslog(LOG_INFO, "%s", command);
        }
      }
    }
  }
}

//   /QUIT reason
//
// Causes you to disconnect from the server.
// If you include a reason, it will be displayed on all channels as you quit
void quit(char *string, int sock, char *userNick) {
  char *reason, *prefix, **arraylist, **arraylistNicks, *command;
  long num, numNicks;
  int i, j, socket;

  if (IRCParse_Quit(string, &prefix, &reason) != IRC_OK) {
    syslog(LOG_ERR, "Error Quit");
    return;
  }

  if (IRCTAD_ListChannelsOfUserArray(NULL, userNick, &arraylist, &num) !=
      IRC_OK) {
    syslog(LOG_ERR, "Error ListChanUser");
    return;
  }

  IRCTAD_Quit(userNick);
  if (reason != NULL && num > 0) {
    for (i = 0; i < num; i++) {

      if (IRCTAD_ListNicksOnChannelArray(arraylist[i], &arraylistNicks,
                                         &numNicks) != IRC_OK) {
        syslog(LOG_ERR, "Error ListNickChan en %s", arraylist[i]);
        return;
      }

      if (IRCMsg_Quit(&command, userNick, reason) == IRC_OK) {

        for (j = 0; j < numNicks; j++) {

          socket = getsocket(arraylistNicks[i]);
          send(socket, command, strlen(command), 0);
        }
      }
    }
  }
  if (IRCMsg_Kill(&command, prefix, userNick, "Desconectado") == IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
  close(sock);
}

void motd(char *string, int sock, char *userNick) {
  char *prefix, *target, *command;

  if (IRCParse_Motd(string, &prefix, &target) != IRC_OK) {
    syslog(LOG_ERR, "Error Motd");
    return;
  }

  if (IRCMsg_RplMotdStart(&command, "REDES2", userNick, "REDES2") == IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }

  if (IRCMsg_RplMotd(&command, "REDES2", userNick, motdServer) == IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
  if (IRCMsg_RplEndOfMotd(&command, "REDES2", userNick) == IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
}

void msg(char *string, int sock, char *userNick) {
  char *nickorchannel, *prefix, *msg, *command;

  if (IRCParse_Privmsg(string, &prefix, &nickorchannel, &msg) == IRC_OK) {
    if (IRCTADUser_Test(0, NULL, nickorchannel) == IRC_OK) {
      syslog(LOG_INFO, "User");
      msgUser(nickorchannel, userNick, msg);
    } else if (IRCTAD_TestChannelOfUser(nickorchannel, userNick) == IRC_OK) {
      syslog(LOG_INFO, "Canal");
      msgCanal(nickorchannel, userNick, msg);
    } else {
      if (IRCMsg_ErrNoSuchNick(&command, "REDES2", userNick, nickorchannel) ==
          IRC_OK) {
        send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "%s", command);
      }
    }
  }
  free(nickorchannel);
  free(msg);
}
void nocommand(char *string, int sock, char *userNick) {
  char *command;
  if (IRCMsg_ErrUnKnownCommand(&command, "REDES2", userNick, string) ==
      IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
  }
  if (IRCMsg_RplTryAgain(&command, "REDES2", userNick,
                         "Comando no reconocido") == IRC_OK) {
    send(sock, command, strlen(command), 0);
    syslog(LOG_INFO, "%s", command);
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
