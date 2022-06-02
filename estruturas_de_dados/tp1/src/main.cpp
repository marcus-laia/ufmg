#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[])
{
    string filename = "./src/entrada.txt";
    ifstream fileHandle(filename);
    string content;

    // numero de rodadas
    int n = 0;
    // dinheiro inicial
    int di = 0;
    // numero de jogadores
    int j = 0;
    // valor do pingo
    int p = 0;


    while (getline(fileHandle, content))
    {
        cout << content;
    }    

    return 0;
}
