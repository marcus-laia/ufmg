#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[])
{
    string filename = "./src/entrada.txt";
    ifstream fileHandle(filename);
    string line;

    // numero de rodadas
    int n = 0;
    // dinheiro inicial
    int di = 0;
    // numero de jogadores
    int j = 0;
    // valor do pingo
    int p = 0;

    string aux;


    while (getline(fileHandle, line))
    {
        istringstream ss(line);
        while ((ss >> aux))
        {
            cout << aux << endl;
        }
    }    

    return 0;
}
