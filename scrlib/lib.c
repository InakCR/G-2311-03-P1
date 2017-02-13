
#include "../includes/lib.h"

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
        if (sockserv < 0)
                on_error("Could not create socket\n");

        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = htonl(INADDR_ANY);

        if(setsockopt(sockserv, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val) < 0)
                on_error("Error setsockopt\n");
        if(bind(socketserv, (struct sockaddr *) &server, sizeof(server)) < 0 )
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
int daemonizar(char* service) {

        int pid, i, fd;

        pid = fork();
        if( pid < 0 ) exit(1);
        if( pid > 0 ) exit(0);

        // resetting File Creation Mask
        umask(0);
        setlogmask (LOG_UPTO (LOG_INFO));
        openlog(service, LOG_PID, LOG_LOCAL3);
        syslog(LOG_INFO, "Initiating_new_server.");

        if(setsid()<0)
                on_error(LOG_ERR,"Error_creating_a_new_SID_for_the_child_process.");

        // Directorio raiz
        if((chdir("/")) < 0)
                on_error(LOG_ERR,"Error_changing_the_current_working_directory_=_\"/\"");

        syslog(LOG_INFO, "Closing_standard_file_descriptors");

        // close all descriptors
        for (i = getdtablesize(); i>=0; --i)
                close(i);

        fd = open("dev/null", O_RDWR, 0);

        if (fd != 1) {

                dup2(fd, STDIN_FILENO);
                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);

                if (fd > 2)
                        close(fd);

        }

        syslog(LOG_INFO, "Closing_server.");
        closelog();

        return EXIT_SUCCESS;

}
