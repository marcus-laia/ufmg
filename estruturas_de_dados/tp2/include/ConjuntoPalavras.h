#pragma once
#include "Palavra.h"
#include <fstream>

class ConjuntoPalavras
{
private:
    Palavra *palavras;
    int num_palavras;
    int chaves[26];

    void ordena(int esq, int dir, int m, int s);
    void particao(int esq, int dir, int *i, int *j, int m, int s);

public:
    ConjuntoPalavras();
    ~ConjuntoPalavras();
    void setChaves(string ordem);

    void Adiciona(Palavra palav);
    void AdicionaTexto(string texto);
    int Busca(string palav);
    void Quicksort(int m, int s);
    void Selecao(int esq, int dir);
    void Insercao(int esq, int dir);
    void Print();
    void PrintFile(string path);
};

ConjuntoPalavras::ConjuntoPalavras()
{
    // palavras = Palavra[10];
    palavras = new Palavra[1000];
    num_palavras = 0;
    for (int i = 0; i < 26; i++)
        chaves[i] = -1;
}

ConjuntoPalavras::~ConjuntoPalavras()
{
    delete [] palavras;
}

void ConjuntoPalavras::setChaves(string ordem)
{
    int tam_ordem = ordem.length();
    int count = 0;

    for (int i = 0; i < tam_ordem; i++)
    {
        if (ordem[i] != ' ')
        {
            // obtem o codigo ascii da letra e converte para indice da lista de chaves
            int cod = (int)ordem[i];
            // se for letra maiuscula 
            if (cod >= 65 && cod <= 90)
                cod -= 65;
            // se for minuscula
            else if (cod >= 97 && cod <= 122)
                cod -= 97;
            else 
            {
                cout << "Caractere fora do conjunto [A-Za-z] encontrado em ORDEM, ignorando...\n";
                continue;
            }

            // a chave na posicao determinada pela operacao com o codigo ascii
            // sera igual ao indice da letra na nova ordem lexicografica
            chaves[cod] = count+97;
            count++;
        }
    }
    // se nao foram ranqueadas todas as letras do alfabeto
    if (count < 26)
    {
        // para cada posicao da lista de chaves (posicoes correspondem a ordem alfabetica)
        for (int i=0; i<26; i++)
        {
            // se a letra daquela posicao nao foi ranqueada
            if (chaves[i] == -1)
            {
                // adiciona ao fim do ranqueamento
                chaves[i] = count+97;
                count++;
            }
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
        palavras[num_palavras] = palav;
        num_palavras++;
    }
    else
    {
        // se existe incrementa o numero de ocorrencias
        palavras[idx].ocorrencias++;
    }
}

void ConjuntoPalavras::AdicionaTexto(string texto)
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
            // se a palavra tem pelo menos um caractere, adiciona
            if (palav.length()) Adiciona(Palavra(palav));

            palav = "";
        }
    }
}

int ConjuntoPalavras::Busca(string palav)
{
    if (num_palavras == 0) return -1;

    for (int i = 0; i < num_palavras; i++)
    {
        if (palav.compare(palavras[i].str) == 0) return i;
    }
    
    return -1;
}

void ConjuntoPalavras::particao(int esq, int dir, int *i, int *j, int m, int s)
{    
    if ((dir-esq) < s)
    {
        // cout << "saindo do quicksort para insercao\n";
        Insercao(esq, dir);
        *i = dir;
        *j = esq;
        return;
    }

    Palavra pivo, aux_troca;
    *i = esq;
    *j = dir;

    if (m > 0 && m <= num_palavras)
    {
        // ordena os primeiros m elementos
        Insercao(esq, esq+m-1);
        // seleciona a mediana como pivo
        pivo = palavras[esq+m/2];
    }
    else 
        pivo = palavras[(*i + *j) / 2];

    do
    {
        while (pivo.compara(palavras[*i], chaves) > 0)
            (*i)++;
        // cout << "i parou\n";
        while (pivo.compara(palavras[*j], chaves) < 0)
            (*j)--;
        // cout << "j parou e com (i, j) = (" << *i << ", " << *j << ")\n";
        if (*i <= *j)
        {
            aux_troca = palavras[*i];
            palavras[*i] = palavras[*j];
            palavras[*j] = aux_troca;
            (*i)++;
            (*j)--;
        }
    } while (*i <= *j);
}

void ConjuntoPalavras::ordena(int esq, int dir, int m, int s)
{
    int i;
    int j;
    particao(esq, dir, &i, &j, m, s);
    // cout << "ordena para " << esq << " " << dir;
    // cout << " teve i: " << i << " e j: " << j << endl;
    if (esq < j)
        ordena(esq, j, m, s);
    if (dir > i)
        ordena(i, dir, m, s);
}

void ConjuntoPalavras::Quicksort(int m, int s)
{
    if (num_palavras > 1) ordena(0, num_palavras - 1, m, s);
}

void ConjuntoPalavras::Selecao(int esq, int dir)
{
    int i, j, Min;
    for (i = esq; i < dir - 1; i++)
    {
        Min = i;
        for (j = i + 1; j < dir; j++)
        {
            if (palavras[j].compara(palavras[Min], chaves) < 0)
                Min = j;
        }
        Palavra aux = palavras[i];
        palavras[i] = palavras[Min];
        palavras[Min] = aux;
    }
}

void ConjuntoPalavras::Insercao(int esq, int dir)
{
    int i, j;
    Palavra aux;
    for (i = esq+1; i < dir+1; i++)
    {
        aux = palavras[i];
        j = i - 1;
        while ((j >= 0) && (aux.compara(palavras[j], chaves) < 0))
        {
            palavras[j + 1] = palavras[j];
            j--;
        }
        palavras[j + 1] = aux;
    }
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

void ConjuntoPalavras::PrintFile(string path)
{
    ofstream File(path);
    for (int i = 0; i < num_palavras; i++)
    {
        File << palavras[i].str << " ";
        File << palavras[i].ocorrencias;
        File << "\n";
    }
    File << "#FIM\n";
    File.close();
}