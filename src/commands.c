#include "../includes/commands.h"

void doCommand(char *string, int sock) {

    if (string == NULL)
        return;

    switch(IRC_CommandQuery(string)) {

        case NICK:
            syslog(LOG_INFO, "NICK");
            break;
        case USER:
            syslog(LOG_INFO, "USER");
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
        default:
            syslog(LOG_INFO, "COMANDO NO RECONOCIDO");
            break;
    }
}
