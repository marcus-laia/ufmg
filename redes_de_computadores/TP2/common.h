#pragma once

#include <stdlib.h>

#include <arpa/inet.h>

// NETWORK

void logexit(const char *msg);

int addrparse(const char *addrstr, const char *portstr,
              struct sockaddr_storage *storage);

void addrtostr(const struct sockaddr *addr, char *str, size_t strsize);

int server_sockaddr_init(const char *proto, const char *portstr, 
                         struct sockaddr_storage *storage);

// INDUSTRY SYSTEM

typedef struct command
{
    char operation[25];
    int equipment;
} Command;

typedef struct instruction
{
    int idMsg, idOrig, idDest;
    char payload[20];
    char message[50];
} Instruction;

typedef struct database
{
    int equipments[15];
    int selfId;
    int equip_count;
} Database;

void reset_command(Command *comm);

int parseCommand(Command *co, char *str);

void reset_instruction(Instruction *inst);

void msg_from_instruction(Instruction *inst);

int instruction_from_command(Instruction *inst, Command *comm, int selfId);

void initializeDB(Database *db);

void listEquipments(Database *db);

void addEquipment(Database *db, int id);

int newEquipment(Database *db);

int removeEquipment(Database *db, int id);

int handleMessage(char *msg, Database *db, int selfId);

void printDB(Database *db);