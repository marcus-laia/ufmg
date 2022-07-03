#include <iostream>
#include <cctype>
#include <sstream>
#include <string.h>

using namespace std;

class Palavra
{
private:
    int ocorrencias;
    double chave;
    string str;
public:
    Palavra();
    Palavra(string palav, double key);

friend class ConjuntoPalavras;
};

Palavra::Palavra() {}

Palavra::Palavra(string palav, double key)
{
    str = palav;
    chave = key;
    ocorrencias = 0;
}

class ConjuntoPalavras
{
private:
    Palavra palavras[10];
    int num_palavras;
public:
    ConjuntoPalavras();
    ~ConjuntoPalavras();
    void Adiciona(Palavra palav);
    void Ordena();
    void Print();
};

ConjuntoPalavras::ConjuntoPalavras()
{
    //palavras = Palavra[10];
    num_palavras = 0;
}

ConjuntoPalavras::~ConjuntoPalavras()
{
}

void ConjuntoPalavras::Adiciona(Palavra palav)
{
    // se a palavra nao existe no conjunto adiciona
    palav.ocorrencias++;
    palavras[num_palavras] = palav;
    num_palavras++;
}

void ConjuntoPalavras::Ordena() {};

void ConjuntoPalavras::Print()
{
    for (int i = 0; i < num_palavras; i++)
    {
        cout << "Palavra: " << palavras[i].str;
        cout << " Rank: " << palavras[i].chave;
        cout << " Count: " << palavras[i].ocorrencias;
        cout << "\n";
    }
}


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

void parse(string texto, int keys[], ConjuntoPalavras *todas_palavras)
{
    int tam_texto = texto.length();
    
    string palav = "";
    // chave decimal tira o problema de lidar com palavras de diferentes tamanhos
    string chave = "0.";

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
            
            // se for uma letra
            if (cod >= 97 && cod <= 122) 
                // se a letra for dada na nova ordem, atribui o valor da chave
                if (keys[cod-97] != -1) cod = keys[cod-97];
                
            // formata as chaves para terem 3 digitos
            if (cod <= 9) chave += "00";
            else if (cod <= 99) chave += "0";
            chave += to_string(cod);
        }
        
        if (texto[i] == ' ' || i == tam_texto-1)
        {
            // se o ultimo caractere for hifen
            if (palav.back() == '-')
            {
                // se nao tiver mais texto remove o hifen
                if (i == tam_texto-1) palav.pop_back();
                // caso contrario continua na mesma palavra
                else continue;
            }

            // cod = primeiro caractere
            // cod = (int)palav[0];
            // se for uma letra
            // if (cod >= 97 && cod <= 122) 
                // se o caractere foi registrado na ordem dada
                // if (keys[cod-97] != -1) cod = keys[cod-97]; //cod-97 vai indexar a lista de chaves

            Palavra auxPalavra(palav, stod(chave));

            todas_palavras->Adiciona(auxPalavra);

            // cout << "adiciona palavra " << palav;
            // cout << " com codigo " << cod << "\n";

            palav = "";
            chave = "0.";
        }
    }
}

int main()
{
    string ordem = "Z Y X W V U T S R Q P O N M L K J I H G F E D C B A";
    string texto = "Era uma vez UMA gata xadrez.";

    int keys[26] = {-1};

    ConjuntoPalavras todasPalavras;

    get_keys(ordem, keys);
    parse(texto, keys, &todasPalavras);

    todasPalavras.Print();
    
    return 0;
}