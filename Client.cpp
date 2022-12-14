#include "Client.h"

Client::Client() : connected(0)
{
}

Client::~Client()
{
    if(connected)
        close(sd);
}

int Client::connect_(const char * sv_adress, const char * input_port)
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

int Client::read_(char* sv_msg)
{
    int must_reply = 0;

    char msg[100];
    if (read(sd, msg, 100) < 0)
    {
        printf("Eroare la read() de la server.\n");
        return -1;
    }

    if( msg[ strlen(msg) - 1] == '1')
        must_reply = 1;

    msg[ strlen(msg) - 1] = '\0';

    strcpy(sv_msg, msg);
    printf("Server: %s\n", msg);

    return must_reply;
}

int Client::write_(char* cl_msg)
{
    if (write(sd, cl_msg, 100) <= 0)
        {
            printf("Eroare la write() spre server.\n");
            return -1;
        }
    return 0;
}

int main (int argc, char *argv[])
{
    char sv_msg[100];
    char cl_msg[100];
    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    Client* C = new Client();

    C->connect_(argv[1], argv[2]);

    // get input
    bzero(cl_msg, 100);
    printf("Numele jucatorului: ");
    fflush(stdout);
    read(0, cl_msg, 100);

    C->write_(cl_msg);

    while(1){

        bool must_reply = C->read_(sv_msg);

        if(must_reply){
            // get input
            bzero(cl_msg, 100);
            printf("Mutare: ");
            fflush(stdout);
            read(0, cl_msg, 100);

            C->write_(cl_msg);
        }
    }

    return 0;
}