#include "Client.h"

pthread_mutex_t lock;

Client::Client()
{
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
    }

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

    return 0;
}
