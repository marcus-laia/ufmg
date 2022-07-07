#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common.h"

#define BUFSZ 500
#define NUMEQUIPMENTS 4
#define NUMSENSORS 4
#define MAXSENSORS 15
#define MAXEQUIPMENTS 15
#define ELEMSWAP(a, b) (a += b, b = a - b, a -= b)

/*
typedef struct command
{
    char operation[25];
    int equipment;
} command;

void reset_command(command *comm)
{
    memset(comm->operation, 0, 25);
    comm->equipment = 0;
}

int parseCommand(command *co, char *str)
{
    char *token = strtok(str, " ");

    // auxiliar na identificacao de valores numericos validos
    int strNum = 0;
    // indica que o proximo valor deve se referir ao equipamento
    int equipFlag = 0;

    // itera pelas palavras do comando de entrada
    while (token != NULL)
    {        
        strNum = atoi(token);

        // id do equipamento foi lido
        if (equipFlag == 2)
        {
            strcpy(str, "invalid input format\n");
            return 0;
        }
        else if (equipFlag == 1)
        {
            // verifica o id do equipamento
            if (strNum > MAXEQUIPMENTS || strNum < 1)
            {
                strcpy(str, "invalid equipment\n");
                return 0;
            }
            // define o equipamento
            co->equipment = strNum;
            equipFlag = 2;
        }

        // eh uma palavra ou o numero zero, que invalida o comando
        if (strNum == 0)
        {
            if ((int)strcmp(token, "from") == 0)
            {
                // o proximo valor deve ser referente ao equipamento
                equipFlag = 1;
            }
            else
            {
                // concatena strings que devem se referir a operacao
                strcat(co->operation, token);
            }
        }
        token = strtok(NULL, " ");
    }
    return 1;
}

typedef struct instruction
{
    int idMsg, idOrig, idDest;
    char payload[20];
    char message[50];
} instruction;

void reset_instruction(instruction *inst)
{
    memset(inst->payload, 0, 20);
    inst->idMsg = 0;
    inst->idOrig = 0;
    inst->idDest = 0;
    memset(inst->message, 0, 50);
}

void msg_from_instruction(instruction *inst)
{
    switch (inst->idMsg)
    {
    case 1:
        sprintf(inst->message, "01");
        break;
    case 2:
        sprintf(inst->message, "02 0%d", inst->idOrig);
        break;
    case 3:
        sprintf(inst->message, "03 %s", inst->payload);
        break;
    case 4:
        sprintf(inst->message, "04 %s", inst->payload);
        break;
    case 5:
        sprintf(inst->message, "05 0%d 0%d", inst->idOrig, inst->idDest);
        break;
    case 6:
        sprintf(inst->message, "06 0%d 0%d %s", inst->idOrig, inst->idDest, inst->payload);
        break;
    case 7:
        sprintf(inst->message, "07 0%d %s", inst->idDest, inst->payload);
        break;
    case 8:
        sprintf(inst->message, "08 0%d %s", inst->idDest, inst->payload);
        break;
    
    default:
        printf("INSTRUCAO INVALIDA\n");
        break;
    }    
}

int instruction_from_command(instruction *inst, command* comm, int selfId)
{
    if(strcmp(comm->operation, "closeconnection") == 0)
    {
        inst->idMsg = 2;
        inst->idOrig = selfId;
        msg_from_instruction(inst);
        // sprintf(inst->message, "02 0%d", selfId);
        return 1;
    }
    else if(strcmp(comm->operation, "listequipment") == 0)
    {
        return 0;
    }
    else if(strcmp(comm->operation, "requestinformation") == 0)
    {
        inst->idMsg = 5;
        inst->idOrig = selfId;
        inst->idDest = comm->equipment;
        msg_from_instruction(inst);
        // sprintf(inst->message, "05 0%d 0%d", selfId, comm->equipment);
        return 1;
    }
    return -1;
}

typedef struct database
{
    // NUMSENSORS ao inves de MAXSENSORS pq nn eh permitido sensor igual para um mesmo equipamento
    int equipments[MAXEQUIPMENTS];
    int selfId;
    int equip_count;
} database;

void initializeDB(database *db)
{
    // inicializa um vetor nulo para os equipamentos
    for (int equipment = 0; equipment < MAXEQUIPMENTS; equipment++)
    {
        db->equipments[equipment] = 0;
    }
    db->equip_count = 0;
}

void listEquipments(database *db)
{
    for (int i = 0; i < db->equip_count; i++)
    {
        printf("0%d ", db->equipments[i]);
    }
    printf("\n");    
}

void addEquipments(database *db, char *str)
{    
    char *token = strtok(str, " ");

    int strNum = 0;

    // le mais um para pular o id da msg
    token = strtok(NULL, " ");
    // itera pelas palavras da mensagem
    while (token != NULL)
    {        
        strNum = atoi(token);

        db->equipments[strNum-1] = strNum;
        printf(" %d", strNum);

        token = strtok(NULL, " ");
    }    
}

int newEquipment(database *db)
{
    if (db->equip_count >= MAXEQUIPMENTS) return -4;

    for (int i = 0; i < db->equip_count; i++)
    {
        if (db->equipments[i] == -1)
        {
            // adiciona equipamento
            db->equipments[i] = i+1;
            return i+1;
        }
    }
    // banco de dados cheio!! nao deve chegar aqui
    return -4; 
}

int removeEquipment(database *db, int id)
{
    // se o equipamento nao esta listado
    if (db->equip_count == 0 || db->equipments[id-1]==0)
        return -1;

    db->equipments[id-1] = 0;
    return id;
}

void handleMessage(char *msg, database *db, int selfId)
{
    char *token = strtok(msg, " ");

    int strNum = 0;

    // itera pelas palavras da mensagem  
    strNum = atoi(token);

    int aux = 0;
    switch (strNum)
    {
    case 1:
        aux = newEquipment(db);
        printf("ADDING EQUIPMENT %d\n", aux);
        break;
    case 2:
        // le o proximo valor
        token = strtok(NULL, " ");
        strNum = atoi(token);

        aux = removeEquipment(db, strNum);
        printf("REMOVING EQUIPMENT %d\n", aux);
        break;
    case 3:
        // le o proximo valor
        // token = strtok(NULL, " ");
        // strNum = atoi(token);

        printf("ADDED EQUIPMENTS ");
        addEquipments(db, msg);
        printf("\n");
        // eh assumido que nao havera erro pois eh apenas uma resposta do servidor
        aux = 1;
        break;
    case 4:
        printf("ADDED EQUIPMENTS ");
        addEquipments(db, msg);
        printf("\n");
        // eh assumido que nao havera erro pois eh apenas uma resposta do servidor
        aux = 1;
        break;
    case 5:
        int orig = 0;
        int dest = 0;

        // le o proximo valor
        token = strtok(NULL, " ");
        orig = atoi(token);
        // le o proximo valor
        token = strtok(NULL, " ");
        dest = atoi(token);

        if( !db->equipments[orig-1] ||
            !db->equipments[dest-1] ) aux = -1;

        // se for servidor
        if (selfId == 0)
        {
            printf("Sending request to %d", dest);
        }
        else if (selfId == dest)
        {
            printf("Requested Information");
            // gerar valor aleatorio
            // enviar resposta
        }

        break;
    case 6:
        int orig = 0;
        int dest = 0;
        double data = 0;

        // le o proximo valor
        token = strtok(NULL, " ");
        orig = atoi(token);
        // le o proximo valor
        token = strtok(NULL, " ");
        dest = atoi(token);

        if( !db->equipments[orig-1] ||
            !db->equipments[dest-1] ) aux = -1;

        // se for servidor
        if (selfId == 0)
        {
            printf("Sending information to %d", dest);
        }
        else if (selfId == dest)
        {
            printf("Received Information");
           
            token = strtok(NULL, " ");
            data = atoi(token);

            printf("Value from %d: %f", orig, data);
        }
        break;
    case 7:
        int code = 0;
        // le o proximo valor
        token = strtok(NULL, " ");
        code = atoi(token);

        switch (code)
        {
        case 1:
            printf("Equipment not found\n");
            break;
        case 2: 
            printf("Source equipment not found\n");
            break;
        case 3: 
            printf("Target equipment not found\n");
            break;
        case 4: 
            printf("Equipment limit exceeded\n");
            break;
        default:
            printf("Invalid Error Code\n");
            break;
        }
        break;
    case 8:
        int code = 0;
        // le o proximo valor
        token = strtok(NULL, " ");
        code = atoi(token);

        switch (code)
        {
        case 1:
            printf("Successful removal\n");
            break;
        default:
            printf("Invalid Error Code\n");
            break;
        }
        break;
    
    default:
        printf("Invalid Message ID");
        break;
    }
    
    if (aux > 0)
    {
        printf("ADDED EQUIPMENT %d\n", aux);
    }
    else
    {
        printf("ERROR %d\n", -aux);
    }
    
    return aux;
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
} */

int main(int argc, char **argv)
{
    printf("PROGRAMA INICIALIZADO\n");

    char buff[50]; // add, read or remove

    Command comm;
    Instruction inst;

    int selfId = 1;

    while (1)
    {
        fgets(buff, 50, stdin);
        buff[strcspn(buff, "\n")] = 0;
        if (strcmp(buff, "kill") == 0)
            break;

        reset_command(&comm);
        reset_instruction(&inst);

        int valid_comm = parseCommand(&comm, buff);

        printf("operation: %s\n", comm.operation);
        printf("equipment: %d\n", comm.equipment);
        
        int inst_aux = instruction_from_command(&inst, &comm, selfId);

        printf("message: %s\n", inst.message);

        if (valid_comm)
        {
            // printInd(industria);
            printf("is valid c inst status %d\n", inst_aux);
            // execute_operation(inst, &industria);
            // printInd(industria);
        }
        else
            printf("invalid input\n");
    }

    return 0;
}