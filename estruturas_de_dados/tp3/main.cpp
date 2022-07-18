#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include "argParser.h"

using namespace std;

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

    string texto = "";

    inputFile >> M;
    cout << M << endl;

    while (inputFile >> op)
    {
        inputFile >> U >> E;        
        cout << op << " / " << U << " / " << E << " / " << N << endl;
        if (op == "ENTREGA")
        {
            inputFile >> N;
            getline(inputFile, texto);
            texto.erase(0, 1);
            cout << texto << "\n";
        }        
    }

    inputFile.close();  

    return 0;
}
