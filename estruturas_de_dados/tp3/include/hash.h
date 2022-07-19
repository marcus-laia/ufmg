#pragma once

#include <fstream>
#include <sstream>
#include "binaryTree.h"
#include "message.h"

class Hash
{
private:
    int len;
    bool log;
    ofstream logFile;
    binaryTree *table;
public:
    Hash(int M);
    ~Hash();
    int getPos(int val);
    void entrega(message email);
    void apaga(int user, int emailKey);
    void consulta(int user, int emailKey);
    void openLogFile(string path);
    void closeLogFile();
    void printLog(string text);
};

Hash::Hash(int M)
{
    len = M;
    table = new binaryTree[M];
    log = false;
}

Hash::~Hash()
{
    for (int i=0; i<len; i++)
    {
        table[i].clear();
    }
    // cout << "\nTRACK: cleaned trees\n";
    delete [] table;
    // cout << "\nTRACK: cleaned table\n";
}

int Hash::getPos(int val)
{
    return val % len;
}

void Hash::entrega(message email)
{
    // cout << "\nTRACK: " << "ENTREGANDO:\n";
    // obtem o indice de insercao a partir do valor de U
    int pos = getPos(email.getUser());
    // cout << "\nTRACK: "  << "POSITION: " << pos << endl;
    int success = table[pos].insert(email);
    // cout << "\nTRACK: "  << "SUCCESS: " << success << endl;

    ostringstream oss;

    if (success)
        oss << "OK: MENSAGEM " << email.getKey() << " PARA "
        << email.getUser() << " ARMAZENADA EM " << pos << "\n";
    else oss << "ERRO: MENSAGEM NAO ENTREGUE\n";

    printLog(oss.str());
}

void Hash::apaga(int user, int emailKey)
{
    // obtem o indice de remocao a partir do valor de U
    int pos = getPos(user);
    int success = table[pos].remove(emailKey);
    
    ostringstream oss;

    if (success)
        oss << "OK: MENSAGEM APAGADA\n";
    else oss << "ERRO: MENSAGEM INEXISTENTE\n";
    
    printLog(oss.str());
}

void Hash::consulta(int user, int emailKey)
{
    message result;
    // obtem o indice de consulta a partir do valor de U
    int pos = getPos(user);
    result = table[pos].search(emailKey);

    if (user != result.getUser()) result.setKey(-1);

    ostringstream oss;

    if (result.getKey() != -1)
        oss << "CONSULTA " << user << " " << emailKey << ": " << result.getEmail() << "\n";
    else
        oss << "CONSULTA " << user << " " << emailKey << ": MENSAGEM INEXISTENTE\n";
    
    printLog(oss.str());
}


void Hash::openLogFile(string path)
{
    logFile = ofstream(path);
    log = true;
}


void Hash::closeLogFile()
{
    logFile.close();
    log = false;
}

void Hash::printLog(string text)
{
    cout << text;

    if(log)
    {
        logFile << text;
    }
}