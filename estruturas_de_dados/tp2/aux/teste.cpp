#include <iostream>
#include <cctype>
#include <sstream>
#include <string.h>

using namespace std;

int procura(string palav, string *palavras, int num_palavras)
{
    if (num_palavras == 0) return 0;

    int i; int esq = 0; int dir = num_palavras-1;

    do
    {
        i = (esq+dir)/2;        
        if (palav.compare(palavras[i]) > 0) esq = i+1;
        else if (palav.compare(palavras[i]) < 0) dir = i-1;
        else return i;
    } while ((esq <= dir));

    return -1;    
}

int main(int argc, char const *argv[])
{
    string palavras[3] = { "codigo", "meu", "testando"};
    
    cout << endl << procura("testando", palavras, 3) << endl;

    return 0;
}
