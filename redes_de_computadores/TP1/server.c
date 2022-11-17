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

typedef struct instruction
{
    char operation[20];
    int sensors[MAXSENSORS];
    int equipment;
    int sensCount;
} instruction;

void reset_instruction(instruction *inst)
{
    memset(inst->operation, 0, 20);
    memset(inst->sensors, 0, MAXSENSORS * sizeof(int));
    inst->equipment = 0;
    inst->sensCount = 0;
}

int parseInstruction(instruction *in, char *str)
{
    char *token = strtok(str, " ");

    // auxiliar na identificacao de valores numericos validos
    int strNum = 0;
    // indica que o proximo valor deve se referir ao equipamento
    int equipFlag = 0;
    // conta o numero de sensores na instrucao
    in->sensCount = 0;

    // itera pelas palavras do comando de entrada
    while (token != NULL)
    {
        // id do equipamento foi lido
        if (equipFlag == 2)
        {
            strcpy(str, "invalid input format\n");
            return 0;
        }

        strNum = atoi(token);
        // eh uma palavra ou o numero zero, que invalida o comando
        if (strNum == 0)
        {
            if ((int)strcmp(token, "in") == 0)
            {
                // o proximo valor deve ser referente ao equipamento
                equipFlag = 1;
            }
            else
            {
                // concatena strings que devem se referir a operacao
                strcat(in->operation, token);
            }
        }
        else
        {
            if (equipFlag == 1)
            {
                // verifica o id do equipamento
                if (strNum > NUMEQUIPMENTS || strNum < 1)
                {
                    strcpy(str, "invalid equipment\n");
                    return 0;
                }
                // define o equipamento
                in->equipment = strNum;
                equipFlag = 2;
            }
            else if (equipFlag == 0)
            {
                // verifica o id do sensor
                if (strNum > NUMSENSORS || strNum < 1)
                {
                    strcpy(str, "invalid sensor\n");
                    return 0;
                }
                
                if (in->sensCount >= MAXSENSORS)
                {
                    strcpy(str, "limit exceeded\n");
                    return 0;
                }

                in->sensors[in->sensCount] = strNum;
                in->sensCount++;
            }
        }
        token = strtok(NULL, " ");
    }
    return 1;
}

// implementacao com matriz
typedef struct industry
{
    // NUMSENSORS ao inves de MAXSENSORS pq nn eh permitido sensor igual para um mesmo equipamento
    int equipments[NUMEQUIPMENTS][NUMSENSORS];
    int sensors_count;
} industry;

void initializeIndustry(industry *ind)
{
    // inicializa uma matriz nula para os sensores dos equipamentos
    for (int equipment = 0; equipment < NUMEQUIPMENTS; equipment++)
    {
        for (int sensor = 0; sensor < NUMSENSORS; sensor++)
        {
            ind->equipments[equipment][sensor] = 0;
        }
    }
    ind->sensors_count = 0;
}

int addSensor(int sensorId, int equipmentId, industry *ind)
{
    int equipment_idx = equipmentId - 1;
    for (int sensor = 0; sensor < NUMSENSORS; sensor++)
    {
        // posicao nula na matriz = posicao vaga para adicao de sensor
        if (ind->equipments[equipment_idx][sensor] == 0)
        {
            ind->equipments[equipment_idx][sensor] = sensorId;
            ind->sensors_count++;
            return 1;
        }
        else if (ind->equipments[equipment_idx][sensor] == sensorId)
        {
            // este sensorId ja esta no equipamento
            return 0;
        }
    }
    return 0;
}

int removeSensor(int sensorId, int equipmentId, industry *ind)
{
    int equipment_idx = equipmentId - 1;
    int last_element_idx = 0;

    // para cada sensor do equipamento
    for (int sensor = NUMSENSORS - 1; sensor >= 0; sensor--)
    {
        // quando encontra um sensor adicionado
        if (ind->equipments[equipment_idx][sensor] != 0)
        {
            // se ele eh igual ao que se deseja remover
            if (ind->equipments[equipment_idx][sensor] == sensorId)
            {
                // remove o sensor do equipamento na matriz
                ind->equipments[equipment_idx][sensor] = 0;

                // se o ultimo sensor do equipamento nao foi definido (nao achou ou eh esse), retorna
                if (!last_element_idx)
                    return 1;

                // coloca o elemento da ultima posicao no lugar do elemento removido
                ELEMSWAP(ind->equipments[equipment_idx][sensor],
                         ind->equipments[equipment_idx][last_element_idx]);

                return 1;
            }
            // se o ultimo sensor do equipamento nao foi definido (nao achou ou eh esse), define
            if (!last_element_idx)
                last_element_idx = sensor;
        }
    }
    return 0;
}

int listSensors(industry ind, int equipmentId, int *i)
{
    int sensor = 0;
    int sensCount = 0;

    for (int s_id = 0; s_id < NUMSENSORS; s_id++)
    {
        sensor = ind.equipments[equipmentId - 1][s_id];
        if (sensor != 0)
        {
            i[s_id] = sensor;
            sensCount++;
        }
        else
            break;
    }
    return sensCount;
}

float readSensor(int sensorId, int equipmentId, industry ind)
{
    int equipment_idx = equipmentId - 1;
    for (int s_id = 0; s_id < NUMSENSORS; s_id++)
    {
        if (ind.equipments[equipment_idx][s_id] == 0)
        {
            return -1;
        }
        else if (ind.equipments[equipment_idx][s_id] == sensorId)
        {
            srand(sensorId * time(NULL));
            return 10 * (double)rand() / RAND_MAX;
        }
    }
    return -1;
}

void printInd(industry ind)
{
    for (int i = 0; i < NUMEQUIPMENTS; i++)
    {
        for (int j = 0; j < NUMSENSORS; j++)
        {
            printf("%d ", ind.equipments[i][j]);
        }
        printf("\n");
    }
}

int execute_operation(instruction com, industry *ind, char *response)
{
    strcpy(response, "");

    if (strcmp(com.operation, "addsensor") == 0)
    {
        // verifica se o limite de sensores sera excedido
        if (ind->sensors_count + com.sensCount > MAXSENSORS)
        {
            strcat(response, "limit exceeded\n");
        }
        else
        {
            // aux para acompanhar sensores nao adicionados
            char not_added_sensors[50] = "";
            // aux para formatar strings antes de adicionar ao buffer
            char aux_str[80] = "";

            // adiciona cada sensor
            strcat(response, "sensor(s) ");
            for (int i = 0; i < com.sensCount; i++)
            {
                if (addSensor(com.sensors[i], com.equipment, ind))
                {
                    sprintf(aux_str, "0%d ", com.sensors[i]);
                    strcat(response, aux_str);
                }
                else
                {
                    sprintf(aux_str, "0%d ", com.sensors[i]);
                    strcat(not_added_sensors, aux_str);
                }
            }
            int not_added_aux = strlen(not_added_sensors) / 3;
            if (not_added_aux < com.sensCount)
                strcat(response, "added ");
            if (not_added_aux)
            {
                sprintf(aux_str, "%salready exists in 0%d", not_added_sensors, com.equipment);
                strcat(response, aux_str);
            }
            strcat(response, "\n");
        }
    }
    else if (strcmp(com.operation, "removesensor") == 0)
    {
        // aux para acompanhar sensores nao removidos
        char not_removed_sensors[50] = "";
        // aux para formatar strings antes de adicionar ao buffer
        char aux_str[80] = "";

        // remove cada sensor
        strcat(response, "sensor(s) ");
        for (int i = 0; i < com.sensCount; i++)
        {
            if (removeSensor(com.sensors[i], com.equipment, ind))
            {
                sprintf(aux_str, "0%d ", com.sensors[i]);
                strcat(response, aux_str);
            }
            else
            {
                sprintf(aux_str, "0%d ", com.sensors[i]);
                strcat(not_removed_sensors, aux_str);
            }
        }
        int not_removed_aux = strlen(not_removed_sensors) / 3;
        if (not_removed_aux < com.sensCount)
            strcat(response, "removed ");
        if (not_removed_aux)
        {
            sprintf(aux_str, "%sdoes not exist in 0%d", not_removed_sensors, com.equipment);
            strcat(response, aux_str);
        }
        strcat(response, "\n");
    }
    else if (strcmp(com.operation, "listsensors") == 0)
    {
        // aux para formatar strings antes de adicionar ao buffer
        char aux_str[20] = "";

        int sensors[4];
        int num_sensors = listSensors(*ind, com.equipment, sensors);

        if (num_sensors == 0)
        {
            sprintf(aux_str, "0%d has no sensors", com.equipment);
            strcat(response, aux_str);
        }
        else
        {
            for (int i = 0; i < num_sensors; i++)
            {
                sprintf(aux_str, "0%d ", sensors[i]);
                strcat(response, aux_str);
            }
        }
        strcat(response, "\n");
    }
    else if (strcmp(com.operation, "read") == 0)
    {
        // aux para formatar strings antes de adicionar ao buffer
        char aux_str[80] = "";
        // aux para acompanhar sensores nao lidos
        char not_read_sensors[50] = "";

        float value_read = -1;
        // le cada sensor
        for (int i = 0; i < com.sensCount; i++)
        {
            value_read = readSensor(com.sensors[i], com.equipment, *ind);
            if (value_read >= 0)
            {
                sprintf(aux_str, "%.2f ", value_read);
                strcat(response, aux_str);
            }
            else
            {
                sprintf(aux_str, "0%d ", com.sensors[i]);
                strcat(not_read_sensors, aux_str);
            }
        }
        int not_read_aux = strlen(not_read_sensors) / 3;
        // se ha sensor que nao foi lido
        if (not_read_aux)
        {
            // se o numero de sensores nao lidos nao eh igual ao total da instrucao
            if (not_read_aux != com.sensCount)
                strcat(response, "and ");
            sprintf(aux_str, "sensor(s) %snot installed", not_read_sensors);
            strcat(response, aux_str);
        }
        strcat(response, "\n");
    }
    else
    {
        strcat(response, "invalid instruction\n");
        return 0;
    }
    return 1;
}

void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    // cria variavel de instrucoes
    instruction inst;
    // cria e inicializa uma industria
    industry industria;
    initializeIndustry(&industria);

    if (argc < 3)
    {
        usage(argc, argv);
    }

    // inicializa as configuracoes do socket
    struct sockaddr_storage storage;
    if (server_sockaddr_init(argv[1], argv[2], &storage) != 0)
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
            reset_instruction(&inst);
            valid_inst = parseInstruction(&inst, buff);

            // se a instrucao eh valida
            if (valid_inst)
            {
                op_success = execute_operation(inst, &industria, buff);
            }
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
