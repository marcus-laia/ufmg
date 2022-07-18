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
    ~message();

    int getKey() { return key; };
    int getUser() { return user; };
friend class binaryTree;
};

message::message()
{
    key = -1;
    text = "";
}

message::~message()
{
}
