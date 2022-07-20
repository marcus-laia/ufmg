#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

#include "argParser.h"
#include "hash.h"
#include "binaryTree.h"
#include "message.h"

using namespace std;

int main(int argc, char **argv)
{
    // le os parametros passados por linha de comando
    argParser parser;
    parser.parse_args(argc, argv);

    // abre o arquivo de entrada
    ifstream inputFile(parser.input_path);

    // identificador da operacao
    string op;
    // tamanho da tabela hash
    int M = 0;
    // identificadores para o usuario e o email
    int U, E;
    int N = -1;

    // variavel auxiliar para ler o arquivo
    string text = "";

    // le o tamanho da tabela hash
    inputFile >> M;

    if (M==0)
    {
        cout << "ENTRADA INVALIDA\n";
        return 0;
    }

    // cria o objeto da tabela hash
    Hash hash(M);
    // abre o arquivo de saida
    hash.openLogFile(parser.output_path);

    // enquanto conseguir ler o arquivo (primeira palavra = operacao)
    while (inputFile >> op)
    {
        // le os identificadores de ususario e email
        inputFile >> U >> E;
        if (op == "ENTREGA")
        {
            // le o numero de palavras
            inputFile >> N;
            // le o texto da mensagem
            getline(inputFile, text);
            text.erase(0, 1);
            // cria uma nova mensagem
            message aux_msg(U, E, text);
            // entrega a mensagem (armazena no servidor simulado)
            hash.entrega(aux_msg);
        }
        else if (op == "CONSULTA")
        {
            hash.consulta(U, E);
        }
        else if (op == "APAGA")
        {
            hash.apaga(U, E);
        }
        else
        {
            cout << "OPERACAO INVALIDA\n";
        }
    }
    // fecha o arquivo de entrada
    inputFile.close();
    // fecha o arquivo de saida
    hash.closeLogFile();

    return 0;
}
