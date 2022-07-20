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
    // para cada posicao da tabela
    for (int i=0; i<len; i++)
    {
        // libera os espacos alocados pelas arvores
        table[i].clear();
    }
    // libera o espaco alocado para a tabela
    delete [] table;
}

int Hash::getPos(int val)
{
    // aplica a funcao hash a um valor inteiro
    return val % len;
}

void Hash::entrega(message email)
{
    // obtem o indice de insercao a partir do valor de U
    int pos = getPos(email.getUser());
    // insere o email na arvore da posicao adequada na tabela
    int success = table[pos].insert(email);

    // variavel auxiliar para impressao
    ostringstream oss;

    // define a saida de acordo com o status da tarefa
    if (success)
        oss << "OK: MENSAGEM " << email.getKey() << " PARA "
        << email.getUser() << " ARMAZENADA EM " << pos << "\n";
    else oss << "ERRO: MENSAGEM NAO ENTREGUE\n";

    // imprime a saida para o usuario
    printLog(oss.str());
}

void Hash::apaga(int user, int emailKey)
{
    // obtem o indice de remocao a partir do valor de U
    int pos = getPos(user);
    // remove o email da arvore correspondente a posicao
    int success = table[pos].remove(user, emailKey);
    
    // variavel auxiliar para impressao
    ostringstream oss;

    // define a saida de acordo com o status da tarefa
    if (success)
        oss << "OK: MENSAGEM APAGADA\n";
    else oss << "ERRO: MENSAGEM INEXISTENTE\n";
    
    // imprime a saida para o usuario
    printLog(oss.str());
}

void Hash::consulta(int user, int emailKey)
{
    message result;
    // obtem o indice de consulta a partir do valor de U
    int pos = getPos(user);
    // procura na tabela de posicao correspondente
    result = table[pos].search(emailKey);

    // verifica se o usuario do email encontrado eh o buscado
    if (user != result.getUser()) result.setKey(-1);

    // variavel auxiliar para impressao
    ostringstream oss;

    // define a saida de acordo com o status da tarefa
    if (result.getKey() != -1)
        oss << "CONSULTA " << user << " " << emailKey << ": " << result.getEmail() << "\n";
    else
        oss << "CONSULTA " << user << " " << emailKey << ": MENSAGEM INEXISTENTE\n";
    
    // imprime a saida para o usuario
    printLog(oss.str());
}


void Hash::openLogFile(string path)
{
    // abre o arquivo de saida
    logFile = ofstream(path);
    // modifica a flag de impressao em arquivo
    log = true;
}


void Hash::closeLogFile()
{
    // fecha o arquivo de saida
    logFile.close();
    // modifica a flag de impressao em arquivo
    log = false;
}

void Hash::printLog(string text)
{
    // imprime no terminal
    cout << text;
    // se a flag estiver "ativada"
    if(log)
    {
        // imprime no arquivo
        logFile << text;
    }
}