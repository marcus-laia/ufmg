#pragma once

#include <iostream>
#include <string.h>

using namespace std;

class message
{
private:
    // identificador da mensagem
    int key;
    // identificador do usuario
    int user;
    // texto da mensagem
    string text;
public:
    message();
    message(int U, int E, string MSG);
    ~message();

    // getters
    int getKey() { return key; };
    int getUser() { return user; };
    string getEmail() { return text; };
    // setters
    void setKey(int K) { key = K; };
// da permissoes a objetos da classe de arvore binaria
friend class binaryTree;
};

message::message()
{
    // inicializa uma mensagem vazia
    key = -1;
    text = "";
}

message::message(int U, int E, string MSG)
{
    // inicializa a mensagem
    user = U;
    key = E;
    text = MSG;
}

message::~message()
{
}
