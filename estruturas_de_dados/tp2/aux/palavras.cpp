#include <iostream>
#include <string.h>

using namespace std;

class Palavra
{
private:
    int ocorrencias;
    string str;

public:
    Palavra();
    Palavra(string palav);
    string get_str() { return str; }
    
    int get_cod(int idx, int chaves[]);
    int compara(Palavra outra, int chaves[]);

friend class ConjuntoPalavras;
};

Palavra::Palavra() {}

Palavra::Palavra(string palav)
{
    str = palav;
    ocorrencias = 0;
}

int Palavra::get_cod(int idx, int chaves[])
{
    int cod = -1;

    // cod = ascii do caractere apontado pelo indice
    cod = (int)str[idx];

    // se for uma letra
    if (cod >= 97 && cod <= 122) 
        // se o caractere foi registrado na ordem dada, atribui o rank
        if (chaves[cod-97] != -1) cod = chaves[cod-97];

    return cod;
}

int Palavra::compara(Palavra outra, int chaves[])
{
    int tam = str.length();
    int tam_outra = outra.get_str().length();
    int menor = 0;

    if (tam <= tam_outra) menor = tam;
    else menor = tam_outra;

    for (int i = 0; i < menor; i++)
    {
        if (get_cod(i, chaves) == outra.get_cod(i, chaves)) continue;
        else return (get_cod(i, chaves) - outra.get_cod(i, chaves));
    }    
    // se nao for encontrada diferenca em toda a menor palavra, indica o menor rank pelo tamanho
    if (tam < tam_outra) return -1;
    else if (tam > tam_outra) return 1;
    // se forem iguais, retorna 0
    else return 0;

}

class ConjuntoPalavras
{
private:
    Palavra palavras[10];
    int num_palavras;
    int chaves[26];

    void ordena(int esq, int dir);
    void particao(int esq, int dir, int *i, int *j);
public:
    ConjuntoPalavras();
    ~ConjuntoPalavras();
    void setChaves(string ordem);

    void Adiciona(Palavra palav);
    int Busca(string palav);
    void Quicksort();
    void Print();
};

ConjuntoPalavras::ConjuntoPalavras()
{
    //palavras = Palavra[10];
    num_palavras = 0;
    for (int i = 0; i < 26; i++) chaves[i] = -1;
}

ConjuntoPalavras::~ConjuntoPalavras()
{
}

void ConjuntoPalavras::setChaves(string ordem)
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
            chaves[cod] = count;
            count ++;
        }
    }
}

void ConjuntoPalavras::Adiciona(Palavra palav)
{
    int idx = Busca(palav.str);
    // se a palavra nao existe no conjunto
    if (idx == -1)
    {
        // adiciona ao conjunto
        palav.ocorrencias++;
        palavras[num_palavras] = palav;
        num_palavras++;
    } else {
        // se existe incrementa o numero de ocorrencias
        palavras[idx].ocorrencias++;
    }
}

int ConjuntoPalavras::Busca(string palav)
{
    if (num_palavras == 0) return -1;

    int i; int esq = 0; int dir = num_palavras-1;

    do
    {
        i = (esq+dir)/2;
        if (palav.compare(palavras[i].str) > 0) esq = i+1;
        else if (palav.compare(palavras[i].str) < 0) dir = i-1;
        else return i;
    } while (esq <= dir);

    return -1;    
}

void ConjuntoPalavras::particao(int esq, int dir, int *i, int *j)
{
    Palavra pivo, aux_troca;
    *i = esq; *j = dir;
    pivo = palavras[(*i + *j)/2];

    do
    {
        // while (pivo.chave > palavras[*i].chave) (*i)++;
        // while (pivo.chave < palavras[*j].chave) (*j)--;
        while (pivo.compara(palavras[*i], chaves) > 0) (*i)++;
        while (pivo.compara(palavras[*j], chaves) < 0) (*j)--;
        if (*i <= *j)
        {
            aux_troca = palavras[*i]; palavras[*i] = palavras[*j]; palavras[*j] = aux_troca;
            (*i)++; (*j)--;
        }
    } while (*i <= *j);
}

void ConjuntoPalavras::ordena(int esq, int dir)
{
    int i; int j;
    particao(esq, dir, &i, &j);
    if (esq < j) ordena(esq, j);
    if (dir > i) ordena(i, dir);
}

void ConjuntoPalavras::Quicksort()
{
    ordena(0, num_palavras-1);
}

void ConjuntoPalavras::Print()
{
    for (int i = 0; i < num_palavras; i++)
    {
        cout << palavras[i].str << " ";
        cout << palavras[i].ocorrencias;
        cout << "\n";
    }
    cout << "#FIM\n";
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

void parse(string texto, ConjuntoPalavras *todas_palavras)
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
            // se o ultimo caractere for hifen
            if (palav.back() == '-')
            {
                // se nao tiver mais texto remove o hifen
                if (i == tam_texto-1) palav.pop_back();
                // caso contrario continua na mesma palavra
                else continue;
            }

            Palavra auxPalavra(palav);

            todas_palavras->Adiciona(auxPalavra);

            palav = "";
        }
    }
}

int main()
{
    string ordem = "Z Y X W V U T S R Q P O N M L K J I H G F E D C B A";
    string texto = "Era uma vez UMA gata xadrez.";

    ConjuntoPalavras todasPalavras;
    todasPalavras.setChaves(ordem);

    // get_keys(ordem, keys);
    parse(texto, &todasPalavras);

    todasPalavras.Quicksort();
    todasPalavras.Print();
    
    return 0;
}