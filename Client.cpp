#include "Client.h"
#include "Interface.h"

Client::Client() : connected(0), sv_msg(""), cl_msg("")
{
}

Client::~Client()
{
    if (connected)
        close(sd);
}

void Client::setCl_msg(char* str){

    strcpy(this->cl_msg, str);
    
}


int Client::connect_(const char *sv_adress, const char *input_port)
{
    /* stabilim portul */
    port = atoi(input_port);

    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Eroare la socket().\n");
        return -1;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(sv_adress);
    /* portul de conectare */
    server.sin_port = htons(port);

    if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
    {
        printf("Eroare la connect().\n");
        return -1;
    }
    connected = true;

    return 0;
}

int Client::read_()
{
    int must_reply = 0;

    char msg[100];
    if (read(sd, msg, 100) < 0)
    {
        printf("Eroare la read() de la server.\n");
        return -1;
    }

    if (msg[strlen(msg) - 1] == '1')
        must_reply = 1;

    msg[strlen(msg) - 1] = '\0';

    strcpy(this->sv_msg, msg);
    printf("Server: %s\n", msg);

    return must_reply;
}

int Client::reply()
{
    if (write(sd, cl_msg, 100) <= 0)
    {
        printf("Eroare la write() spre server.\n");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    Client *C = new Client();

    C->connect_(argv[1], argv[2]);

    // trimitem numele jucatorului
    // C->setCl_msg( player_name)
    C->reply();

    while (1)
    {

        bool must_reply = C->read_();

        if (must_reply)
        {

            // get input
            /*
            bzero(cl_msg, 100);
            printf("Mutare: ");
            fflush(stdout);
            read(0, cl_msg, 100);
            */

            C->reply();
        }
    }

    return 0;
}