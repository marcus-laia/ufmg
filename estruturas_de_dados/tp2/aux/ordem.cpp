#include <iostream>
#include <cctype>
#include <sstream>
#include <string.h>

using namespace std;

void get_keys(string ordem, int keys[])
{    
    int tam_ordem = ordem.length();
    int count = 0;

    for (int i = 0; i< tam_ordem; i++)
    {
        if (ordem[i] != ' '){
            // obtem o codigo ascii da letra
            int cod = (int)ordem[i];
            // assumindo entrada correta, se for letra maiuscula
            if (cod < 91) cod -= 65; 
            // se for minuscula
            else cod -= 97;

            // a chave na posicao determinada pela operacao com o codigo ascii
            // sera igual ao indice da letra na nova ordem lexicografica
            keys[cod] = count;
            count ++;
        }
    }
}

int main()
{
    string ordem = "Z Y X W V U T S R Q P O N M L K J I H G F E D C B A";

    int keys[26] = {-1};

    get_keys(ordem, keys);

    for (int i = 0; i < 26; i++)
    {
        cout << (keys[i]+65) << ", ";
    }
    cout << "\n";   

    return 0;
}
