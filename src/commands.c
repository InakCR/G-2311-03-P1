#include "../includes/commands.h"
char userNick[100];
HostNameIp *hostIp(int sock) {
  int err;
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);
  HostNameIp *hi = NULL;
  char hbuf[10000], sbuf[1000];

  hi = (HostNameIp *)malloc(sizeof(HostNameIp));
  if (hi == NULL)
    return NULL;

  err = getpeername(sock, (struct sockaddr *)&addr, &addr_len);
  if (err != 0)
    return NULL;

  hi->ip = inet_ntoa(addr.sin_addr);
  syslog(LOG_INFO, "%s\n", hi->ip);

  struct hostent *he;
struct in_addr ipv4addr;

inet_pton(AF_INET, "74.125.196.105", &ipv4addr);
he = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
syslog(LOG_INFO,"Host name: %s\n", he->h_name);
/*
  if (getnameinfo((struct sockaddr *)&addr, addr_len, hbuf, sizeof(hbuf), sbuf,
                  sizeof(sbuf), NI_NUMERICHOST | NI_NUMERICSERV) == 0)

    syslog(LOG_INFO, "host=%s, serv=%s\n", hbuf, sbuf);
memcpy(hi->name, hbuf);*/
  memcpy(hi->name, he->h_name);
  return hi;
}
void nick(char *string, int sock) {
  char *prefix, *nick, *msg;
  long parser;
  HostNameIp *hi;

  hi = hostIp(sock);

  parser = IRCParse_Nick(string, &prefix, &nick, &msg);

  syslog(LOG_INFO, "parser = %ld", parser);
  syslog(LOG_INFO, "prefix = %s", prefix);
  syslog(LOG_INFO, "nick = %s", nick);
  syslog(LOG_INFO, "msg = %s", msg);
  syslog(LOG_INFO, "ip = %s", hi->ip);
  syslog(LOG_INFO, "host = %s", hi->name);
  strcpy(userNick, nick);

  if (parser == IRCERR_NOSTRING) {
    free(prefix);
    free(nick);
    free(msg);
    on_error(LOG_ERR,
             "***Error {nick()} No existe una cadena para usar como Nick.");
  } else if (parser == IRC_OK) {

    if (UTestNick(nick)) {
      syslog(LOG_INFO, " Ya existe el nick: %s", nick);
    } else {
      syslog(LOG_INFO, " No existe el nick: %s", nick);
    }
  }

  syslog(LOG_INFO, "Nick pasado correctamente, falta añadirlo al cliente");
}

void user(char *string, int sock) {
  char *prefix, *msg, **user, **modehost, **serverother, **realname;
  long parser;

  //  parser =
  //      IRCParse_User(string, &prefix, &user, &modehost, &serverother,
  //      &realname);

  // IRCTADUser_New(, userNick, *realname, NULL, host, char *IP, sock);
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
