#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFSZ 500

void usage(int argc, char **argv)
{
    printf("usage: %s <server IP> <server port>\n", argv[0]);
    printf("example: %s 127.0.0.1 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (addrparse(argv[1], argv[2], &storage) != 0)
    {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    // s = socket(AF_INET, SOCK_STREAM, 0);
    // erro no socket -> retorna -1
    if (s == -1)
    {
        logexit("socket");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);

    // connect retorna 0 quando da certp
    if (connect(s, addr, sizeof(storage)) != 0)
    {
        logexit("connect");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);

    printf("connected to %s\n", addrstr);

    // comunicacao do cliente com o servidor

    // define um buffer para a mensagem
    char buf[BUFSZ];

    while (1)
    {
        memset(buf, 0, BUFSZ);

        // le uma mensagem do teclado o que vai ser enviado para o servidor
        printf("< user > ");
        fgets(buf, BUFSZ - 1, stdin);

        // envia a mensagem ao servidor, count = # de bytes transmitidos
        size_t count = send(s, buf, strlen(buf), 0);
        // se o # de bytes efetivamente transmitidos é diferente do tamanho do buf+1 houve erro
        if (count != strlen(buf))
        {
            logexit("send");
        }

        // reseta para receber o dado
        memset(buf, 0, BUFSZ);
        // controle do total de bytes recebidos (a msg pode vir em partes)
        unsigned total = 0;
        // loop para receber dados do servidos
        // while(1) {
        count = recv(s, buf + total, BUFSZ - total, 0);
        // se nao recebeu nada, a conexao foi fechada
        if (count == 0)
        {
            // connection terminated
            break;
        }
        printf("<server> %s", buf);
        // quando ha esse erro, o servidor desconecta o cliente
        if (strcmp(buf, "invalid instruction\n") == 0) break;
    }

    close(s);

    exit(EXIT_SUCCESS);
}
