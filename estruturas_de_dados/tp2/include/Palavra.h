#pragma once

#include <iostream>
#include <string.h>

using namespace std;

class Palavra
{
private:
    string str;
    int ocorrencias;

public:
    Palavra(){};
    Palavra(string palav);
    string get_str() { return str; }

    int get_cod(int idx, int chaves[]);
    int compara(Palavra outra, int chaves[]);
    void incrementa() { ocorrencias++; };

    friend class ConjuntoPalavras;
};

Palavra::Palavra(string palav)
{
    str = palav;
    ocorrencias = 1;
}

int Palavra::get_cod(int idx, int chaves[])
{
    int cod = -1;

    // cod = ascii do caractere apontado pelo indice
    cod = (int)str[idx];

    // se for uma letra
    if (cod >= 97 && cod <= 122)
        // se o caractere foi registrado na ordem dada, atribui o rank
        if (chaves[cod - 97] != -1)
            cod = chaves[cod - 97];

    return cod;
}

int Palavra::compara(Palavra outra, int chaves[])
{
    int tam = str.length();
    int tam_outra = outra.get_str().length();
    int menor = 0;

    if (tam <= tam_outra)
        menor = tam;
    else
        menor = tam_outra;

    for (int i = 0; i < menor; i++)
    {
        // cout << "comparando " << str << " (" << get_cod(i, chaves) << ") com ";
        // cout << outra.str << " (" << outra.get_cod(i, chaves) << ") temos ";
        // cout << (get_cod(i, chaves) - outra.get_cod(i, chaves)) << endl;
        if (get_cod(i, chaves) == outra.get_cod(i, chaves))
            continue;
        else
            return (get_cod(i, chaves) - outra.get_cod(i, chaves));
    }
    // se nao for encontrada diferenca em toda a menor palavra, indica o menor rank pelo tamanho
    if (tam < tam_outra)
        return -1;
    else if (tam > tam_outra)
        return 1;
    // se forem iguais, retorna 0
    else
        return 0;
}
