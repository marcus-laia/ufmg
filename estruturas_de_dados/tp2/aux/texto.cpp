#include <iostream>
#include <cctype>
#include <sstream>
#include <string.h>

using namespace std;

void parse(string texto)
{
    int tam_texto = texto.length();
    
    string palav = "";

    int cod = -1;
    for (int i = 0; i < tam_texto; i++)
    {
        cod = (int)texto[i];
        
        // se o ultimo caractere nao for espaco ou pontuacao
        if (! ( cod == 32 || cod == 33 ||
                cod == 44 || cod == 46 ||
                cod == 58 || cod == 59 || cod == 63 ) )
        {
            // converte letra maiuscula para minuscula
            if (cod >= 65 && cod <= 90) cod += 32;
            // adiciona o caractere a palavra
            palav += (char) cod;
        }
        
        if (texto[i] == ' ' || i == tam_texto-1)
        {
            // cod = ultimo caractere
            // cod = (int)palav.back();
            // enquanto o ultimo caractere for espaco ou pontuacao
            // while ( cod == 32 || cod == 33 ||
            //         cod == 44 || cod == 46 ||
            //         cod == 58 || cod == 59 || cod == 63 )
            // {
            //     // remove o caractere
            //     palav.pop_back();
            //     cod = (int)palav.back();
            // }
            // se o ultimo caractere for hifen e nao for o fim do texto, continua na mesma palavra
            
            // se o ultimo caractere for hifen
            if (palav.back() == '-')
            {
                // se nao tiver mais texto remove o hifen
                if (i == tam_texto-1) palav.pop_back();
                // caso contrario continua na mesma palavra
                else continue;
            }

            // cod = primeiro caractere
            cod = (int)palav[0];
            // se for uma letra
            if (cod >= 97 && cod <= 122) 
                // se o caractere foi registrado na ordem dada
                if (true) cod = cod-97; //cod-97 vai indexar a lista de chaves

            cout << "adiciona palavra " << palav;
            cout << " com codigo " << cod << "\n";

            palav = "";
        }
    }
}

int main()
{
    string texto = "Era- uma-vez U.M;A gata xadrez.-";

    parse(texto);
    
    return 0;
}