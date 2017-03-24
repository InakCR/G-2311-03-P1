#include "../includes/utilidadesTAD.h"

int setAway(char *nick, char *reason) {
  long id = 0, creationTS = 0, actionTS = 0, parser = 0;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  int socket = 0;
  parser = IRCTADUser_GetData(&id, &user, &nick, &real, &host, &IP, &socket,
                              &creationTS, &actionTS, &away);
  if (parser == IRC_OK) {
    return IRCTADUser_SetAway(id, user, nick, real, reason);
  }
  return -1;
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
  if (parser == IRC_OK) {
    return socket;
  }

  return -1;
}
void setNick(char *nick, char **userNick) {
  long id = 0, creationTS = 0, actionTS = 0;
  char *user = NULL, *real = NULL, *host = NULL, *IP = NULL, *away = NULL;
  int socket = 0;

  if (IRCTADUser_GetData(&id, &user, userNick, &real, &host, &IP, &socket,
                         &creationTS, &actionTS, &away) == IRC_OK) {
    if (IRCTADUser_Set(id, user, *userNick, real, user, nick, real) == IRC_OK) {
      free(*userNick);
      *userNick = (char *)malloc(sizeof(strlen(nick) + 1));
      strcpy(*userNick, nick);
    }
  }
}
char *isAway(char *nick) {
  char *away = NULL;
  IRCTADUser_GetAway(0, NULL, nick, NULL, &away);
  syslog(LOG_INFO, "Away es %s", away);
  return away;
}
// LIBERAR ESTRUCTURAS
