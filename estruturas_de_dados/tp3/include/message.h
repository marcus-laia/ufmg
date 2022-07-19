#pragma once

#include <iostream>
#include <string.h>

using namespace std;

class message
{
private:
    int key;
    int user;
    string text;
public:
    message();
    message(int U, int E, string MSG);
    ~message();

    int getKey() { return key; };
    int getUser() { return user; };
    string getEmail() { return text; };
    void setKey(int K) { key = K; };
friend class binaryTree;
};

message::message()
{
    key = -1;
    text = "";
}

message::message(int U, int E, string MSG)
{
    user = U;
    key = E;
    text = MSG;
}

message::~message()
{
}
