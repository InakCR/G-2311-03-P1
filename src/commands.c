#include "../includes/commands.h"

char userNick[100];

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

        strcpy(userNick, nick);

        if (parser == IRCERR_NOSTRING) {
                free(prefix);
                free(nick);
                free(msg);
                syslog(LOG_ERR,
                       "***Error {nick()} No existe una cadena para usar como Nick.");
                exit(EXIT_FAILURE);
        } else if (parser == IRC_OK) {

                if (UTestNick(nick)) {
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

        if (parser < 0)
                return;
        syslog(LOG_INFO, "ip = %s", hi->ip);
        syslog(LOG_INFO, "host = %s", hi->name);

        if (parser == IRCERR_NOSTRING) {
                free(prefix);
                free(user);
                free(modehost);
                free(serverother);
                free(realname);
                exit(EXIT_FAILURE);

        } else if (parser == IRC_OK) {
                /*  if(IRCTADUser_New(user, userNick, realname, NULL, hi->name,hi->ip, sock)
                   == IRC_OK) {
                          syslog(LOG_INFO, "%s ,%s",user,userNick);
                   }else{
                          syslog(LOG_INFO, "Usuario no creado");
                   }*/
        }
        syslog(LOG_INFO, "user = %s", user);
        syslog(LOG_INFO, "nick = %s", userNick);
        syslog(LOG_INFO, "realname = %s", realname);
        syslog(LOG_INFO, "ip = %s", hi->ip);
        syslog(LOG_INFO, "host = %s", hi->name);

        IRCMsg_RplWelcome(&command, "REDES2", userNick, user, realname, modehost);
        //  send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "command");
        IRCMsg_RplYourHost(&command, "REDES2", userNick, serverother, "1.0");
        //  send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "command");
        IRCMsg_RplCreated(&command, "REDES2", userNick, gettimeofday(&tv, NULL));
        //  send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "command");
        IRCMsg_RplLuserClient(&command, "REDES2", userNick, getNumeroClientes(), 0,
                              1);
        //  send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "command");
        IRCMsg_RplLuserChannels(&command, "REDES2", userNick, getNumeroCanales());
        // send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "command");
        IRCMsg_RplMotdStart(&command, "REDES2", userNick, serverother);
        // send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "command");
        IRCMsg_RplMotd(&command, "REDES2", userNick, "****BIENVENIDO****\n");
        // send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "command");
        IRCMsg_RplEndOfMotd(&command, "REDES2", userNick);
        // send(sock, command, strlen(command), 0);
        syslog(LOG_INFO, "command");
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
long getNumeroClientesActuales(){
        long nelements = 0;
        char **nicklist;
        IRCTADUser_GetUserList(&nicklist, &nelements);
        IRCTADUser_FreeList(nicklist, nelements);
        return nelements;
}
long getNumeroCanales(){
        char **list=NULL;
        long num = 0;
        IRCTADChan_GetList (&list, &num, NULL);
        IRCTADUser_FreeList(nicklist, num);
        return num;
}
