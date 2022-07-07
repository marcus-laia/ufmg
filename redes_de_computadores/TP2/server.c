#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#define BUFSZ 500
#define NUMEQUIPMENTS 4
#define NUMSENSORS 4
#define MAXSENSORS 15
#define ELEMSWAP(a, b) (a += b, b = a - b, a -= b)

void usage(int argc, char **argv)
{
    printf("usage: %s <server port>\n", argv[0]);
    printf("example: %s 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    // cria variavel de instrucoes
    Instruction inst;
    reset_instruction(&inst);
    // // cria e inicializa um banco de dados
    Database db;
    initializeDB(&db);
    // initializeIndustry(&industria);

    if (argc < 2)
    {
        usage(argc, argv);
        exit(1);
    }

    // inicializa as configuracoes do socket
    struct sockaddr_storage storage;
    if (server_sockaddr_init("v4", argv[1], &storage) != 0)
    {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    // erro no socket -> retorna -1
    if (s == -1)
    {
        logexit("socket");
    }

    // define que a porta utilizada ficara disponivel para ser reaproveitada logo quando fechada
    int enable = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0)
    {
        logexit("setsockopt");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);

    // da bind no socket
    if (bind(s, addr, sizeof(storage)) != 0)
    {
        logexit("bind");
    }
    // da listen no socket
    if (listen(s, 10) != 0)
    {
        logexit("listen");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("bound to %s, waiting connections\n", addrstr);

    int kill = 0;

    while (1)
    {
        // cria um canal para comunicacao com o cliente
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        // aceita solicitacoes para conexao
        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1)
        {
            logexit("accept");
        }

        char caddrstr[BUFSZ];
        addrtostr(caddr, caddrstr, BUFSZ);
        printf("[log] connection from %s\n", caddrstr);

        char buff[BUFSZ];

        // SETUP
        memset(buff, 0, BUFSZ);
        // recebe a mensagem do cliente
        size_t count = recv(csock, buff, BUFSZ, 0);
        int new_id = handleMessage(buff, &db, 0);
        // int new_id = newEquipment(&db);
        printf("new id: %d\n", new_id);
        if (new_id > 0)
        {   
            if (new_id < 10) sprintf(buff, "03 0%d", new_id);
            else sprintf(buff, "03 %d", new_id);
        }
        else strcpy(buff, "07 04");        
        send(csock, buff, strlen(buff), 0);
        // FIM SETUP

        // validador de instrucao
        int valid_inst = 0;
        // flag de sucesso da operacao
        int op_success = 1;

        while (1)
        {
            memset(buff, 0, BUFSZ);
            // recebe a mensagem do cliente
            size_t count = recv(csock, buff, BUFSZ, 0);
            // avalia se a mensagem indica fechamento da conexao ou do server
            if (strcmp(buff, "disconnect\n") == 0)
                break;
            if (strcmp(buff, "kill\n") == 0)
            {
                kill = 1;
                break;
            }
            // imprime o endereco, a mensagem recebida e seu tamanho
            printf("[msg] %s, %d bytes: %s", caddrstr, (int)count, buff);

            // faz a analise da instrucao recebida
            // reset_instruction(&inst);
            // valid_inst = parseInstruction(&inst, buff);

            // se a instrucao eh valida
            // if (valid_inst)
            // {
            //     op_success = execute_operation(inst, &industria, buff);
            // }
            // envia uma mensagem de retorno ao cliente
            count = send(csock, buff, strlen(buff), 0);
            if (count != strlen(buff))
            {
                logexit("send");
            }
            // se a operacao solicitada nao esta entre as 4 implementadas 
            if (!op_success)
            // fecha a conexao
                break;
        }
        // fecha a conexao com o cliente
        close(csock);
        // fecha o socket
        if (kill)
            break;
    }

    exit(EXIT_SUCCESS);
}
