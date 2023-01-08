#include "Client.h"

Client::Client() : connected(0), sv_msg(""), cl_msg(""), must_reply(false)
{
    // interface = new Interface(900.f, 900.f);
}

Client::~Client()
{
    if (connected)
        close(sd);
}

void Client::set_cl_msg(char *str)
{

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

char* Client::get_sv_msg()
{
    return this->sv_msg;
}

int Client::read_()
{
    int read_status;
    char msg[100];
    if (read(sd, msg, 100) < 0)
    {
        printf("Eroare la read() de la server.\n");
        return -1;
        ;
    }

    printf("Server: %s\n", msg);

    if (strlen(msg) == 0)
    {
        return -1;
    }

    switch (msg[strlen(msg) - 1])
    {
    case '0':
        must_reply = false;
        break;
    case '1':
        must_reply = true;
        break;
    case '2':
        must_reply = false;
        // update interface
        break;
    default:
        break;
    }

    strcpy(this->sv_msg, msg);
    //printf("Server: %s\n", msg);

    return 0;
}

int Client::write_()
{
    if (write(sd, cl_msg, 100) <= 0)
    {
        printf("Eroare la write() spre server.\n");
        return -1;
    }
    return 0;
}

void Client::mainLoop()
{
    // interface->displayFirstScreen();

    // connect_(interface->getIp(), interface->getPort());
    connect_("0", "2728");

    // set_cl_msg(interface->getPlayerName());
    char str[100];
    strcpy(str, "Andrei");
    set_cl_msg(str);
    write_();

    while (1)
    {
        if (read_() == -1)
        {
            printf("S-a deconectat serverul");
            break;
        }

        if (must_reply) // has to select pawn to move
        {
            // char* pawn = interface->getPawn();
            // set_cl_msg(pawn);
            write_();
        }
    }
}

/*
int main(int argc, char *argv[])
{
    Client *C = new Client();

    C->mainLoop();

    if (argc != 3)
    {
        printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }



    C->connect_(argv[1], argv[2]);

    // trimitem numele jucatorului
    // C->set_cl_msg( player_name)
    C->reply();

    while (1)
    {

        bool must_reply = C->read_();

        if (must_reply)
        {

            // get input

            bzero(cl_msg, 100);
            printf("Mutare: ");
            fflush(stdout);
            read(0, cl_msg, 100);


            C->reply();
        }
    }

    return 0;
}
*/