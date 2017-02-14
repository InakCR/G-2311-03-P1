#include "../includes/lib.h"

int main(int argc, char *argv[]) {
        int socketServer;

        if (daemonizar("REDES2") < 0)
                on_error(LOG_ERR, "Error daemonizar");

        socketServer = iniciar_servidor(PORT);

        // Apertura del socket
        if (socketServer < 0)
                on_error(LOG_ERR, "Error iniciar_servidor");

        //Accept and incoming connection
        syslog(LOG_INFO, "Waiting for incoming connections...");

        return 0;
        //
}
