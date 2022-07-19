#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>

#include "argParser.h"
#include "hash.h"
#include "binaryTree.h"
#include "message.h"

using namespace std;

// int main(int argc, char const *argv[])
// {
//     /* code */
//     return 0;
// }

int main(int argc, char **argv)
{
    argParser parser;
    parser.parse_args(argc, argv);

    ifstream inputFile(parser.input_path);
    string fileLine;
    string fileWord;

    // identificador da operacao
    string op;
    // tamanho da tabela hash
    int M = 0;
    // identificadores para o usuario e o email
    int U, E;
    int N = -1;

    string text = "";

    inputFile >> M;
    cout << M << endl;

    Hash hash(M);
    hash.openLogFile(parser.output_path);

    while (inputFile >> op)
    {
        inputFile >> U >> E;        
        // cout << "\nTRACK: " << op << " / " << U << " / " << E << " / " << N << endl;
        if (op == "ENTREGA")
        {
            inputFile >> N;
            getline(inputFile, text);
            text.erase(0, 1);
            // cout << "\nTRACK: " << text << "\n";

            message aux_msg(U, E, text);
            // cout << "\nTRACK: " << "CRIOU\n";
            hash.entrega(aux_msg);
            // cout << "\nTRACK: " << "ENTREGOU\n";
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

    inputFile.close();
    hash.closeLogFile();

    return 0;
}
