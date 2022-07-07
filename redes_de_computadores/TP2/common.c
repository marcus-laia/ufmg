#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include <arpa/inet.h>

#include <time.h>
#include <unistd.h>

#include "common.h"

#define MAXEQUIPMENTS 15

// NETWORK

void logexit(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int addrparse(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage)
{
    if (addrstr == NULL || portstr == NULL)
    {
        return -1;
    }

    uint16_t port = (uint16_t)atoi(portstr); // unsigned short
    if (port == 0)
    {
        return -1;
    }
    // host to network short, o oposto eh network to host short (ntos)
    port = htons(port);

    struct in_addr inaddr4; // 32-bit IP address
    if (inet_pton(AF_INET, addrstr, &inaddr4))
    {
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_port = port;
        addr4->sin_addr = inaddr4;
        return 0;
    }

    return -1;
}

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize)
{
    int version;
    char addrstr[INET6_ADDRSTRLEN + 1] = "";
    uint16_t port;

    if (addr->sa_family == AF_INET)
    {
        version = 4;
        struct sockaddr_in *addr4 = (struct sockaddr_in *)addr;
        // ntop = network to presentation, pton = presentation to network
        if (!inet_ntop(AF_INET, &(addr4->sin_addr), addrstr, INET6_ADDRSTRLEN + 1))
        {
            logexit("ntop");
        }
        port = ntohs(addr4->sin_port); // network to host short
    }
    else
    {
        logexit("unknown protocol family");
    }
    if (str){
        snprintf(str, strsize, "IPV%d %s %hu", version, addrstr, port);
    }
}

int server_sockaddr_init(const char *proto, const char *portstr, 
                         struct sockaddr_storage *storage) {

    uint16_t port = (uint16_t)atoi(portstr); // unsigned short
    if (port == 0)
    {
        return -1;
    }
    port = htons(port);

    memset(storage, 0, sizeof(*storage));
    if (strcmp(proto, "v4") == 0) {        
        struct sockaddr_in *addr4 = (struct sockaddr_in *)storage;
        addr4->sin_family = AF_INET;
        addr4->sin_addr.s_addr = INADDR_ANY;
        addr4->sin_port = port;
        return 0;
    } else {
        return -1;
    }
}

// INDUSTRY SYSTEM

void reset_command(Command *comm)
{
    memset(comm->operation, 0, 25);
    comm->equipment = 0;
}

int parseCommand(Command *co, char *str)
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

void reset_instruction(Instruction *inst)
{
    memset(inst->payload, 0, 20);
    inst->idMsg = 0;
    inst->idOrig = 0;
    inst->idDest = 0;
    memset(inst->message, 0, 50);
}

void msg_from_instruction(Instruction *inst)
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

int instruction_from_command(Instruction *inst, Command *comm, int selfId)
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

void initializeDB(Database *db)
{
    // inicializa um vetor nulo para os equipamentos
    for (int equipment = 0; equipment < MAXEQUIPMENTS; equipment++)
    {
        db->equipments[equipment] = 0;
    }
    db->equip_count = 0;
    db->selfId = 0;
}

void listEquipments(Database *db)
{
    for (int i = 0; i < db->equip_count; i++)
    {
        printf("0%d ", db->equipments[i]);
    }
    printf("\n");    
}

void addEquipment(Database *db, int id)
{
    db->equipments[id-1] = id;
}

int newEquipment(Database *db)
{
    if (db->equip_count >= MAXEQUIPMENTS) return -4;
    else if (db->equip_count == 0)
    {
        db->equipments[0] = 1;
        return 1;        
    }    

    for (int i = 0; i < db->equip_count; i++)
    {
        if (db->equipments[i] == 0)
        {
            // adiciona equipamento
            db->equipments[i] = i+1;
            return i+1;
        }
    }
    // banco de dados cheio!! nao deve chegar aqui
    return -4; 
}

int removeEquipment(Database *db, int id)
{
    // se o equipamento nao esta listado
    if (db->equip_count == 0 || db->equipments[id-1]==0)
        return -1;

    db->equipments[id-1] = 0;
    return id;
}

int handleMessage(char *msg, Database *db, int selfId)
{
    printf("\n[message = %s]\n", msg);
    
    printf("B: ");
    printDB(db);
    printf("\n");     

    char *token = strtok(msg, " ");

    int strNum = 0;
    int orig = 0;
    int dest = 0;
    double data = 0;
    int code = 0;

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
        token = strtok(NULL, " ");
        code = atoi(token);
          
        addEquipment(db, code);
        db->selfId = code;

        if (code < 10) printf("New ID: 0%d\n", code);
        else printf("New ID: %d\n", code);
        // eh assumido que nao havera erro pois eh apenas uma resposta do servidor
        aux = 1;
        break;
    case 4:

        token = strtok(NULL, " ");
        while (token != NULL)
        {        
            code = atoi(token);
            
            addEquipment(db, code);
            
            token = strtok(NULL, " ");
        }
        
        // eh assumido que nao havera erro pois eh apenas uma resposta do servidor
        aux = 1;
        break;
    case 5:
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
    
    printf("A: ");
    printDB(db);
    printf("\n");
    
    if (aux > 0)
    {
        printf("OK %d\n", aux);
    }
    else
    {
        printf("ERROR %d\n", -aux);
    }
    
    return aux;
}

void printDB(Database *db)
{
    for (int i=0; i<15; i++) printf("%d ", db->equipments[i]);
}