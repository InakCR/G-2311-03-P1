
#include "../includes/libre.h"

void on_error(int logPri,char* err){
        syslog(logPri, err);
        exit(1);
}
int ini_server(int port){
        struct sockaddr_in server;
        int sockserv,opt_val = 1;
        char mssyslog[100];

        syslog(LOG_INFO, "Creating socketServer");

        sockserv = socket(AF_INET, SOCK_STREAM, 0);
        if (sockserv < 0) on_error("Could not create socket\n");

        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = htonl(INADDR_ANY);

        if(setsockopt(sockserv, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val) < 0)
                on_error("Error setsockopt\n");
        if(bind(socketserv, (struct sockaddr *) &server, sizeof(server)) < 0 );
                on_error("Could not bind socket\n");
                
        /*Procedemos a escuchar llamadas de clientes*/
        if(listen(sockval, MAX_CLIENTS) < 0) {
                close(sockval);
                on_error(LOG_ERR,"Error listening clients");
        }

        sprintf(mssyslog,"Server is listening on %d\n", port);
        syslog(LOG_INFO, mssyslog);

        return sockserv;
}
