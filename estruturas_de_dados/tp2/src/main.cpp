// #pragma once

#include "Palavra.h"
#include "ConjuntoPalavras.h"
#include "options.h"
#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
    argParser parser;
    parser.parse_args(argc, argv);

    // cout << "input_path: " << parser.input_path << endl;
    // cout << "m: " << parser.m << endl;
    // cout << "s: " << parser.s << endl;
    // cout << "output_path: " << parser.output_path << endl;

    // string input_path = "./input/1.txt";

    ifstream entrada(parser.input_path);
    string linhaArq;

    string ordem;
    string texto = "";

    bool get_ordem = false;
    bool get_texto = false;
    while (getline(entrada, linhaArq))
    {
        if (get_ordem)
        {
            ordem = linhaArq;
            get_ordem = false;
        }
        if (get_texto)
        {
            texto += " ";
            texto += linhaArq;
        }
        if (linhaArq.compare("#ORDEM") == 0) get_ordem = true;   
        if (linhaArq.compare("#TEXTO") == 0) get_texto = true;
    }

    entrada.close();  

    if (ordem.length() == 0) cout << "Nova ordem nao encontrada, considerando a ordem alfabetica\n";
    if (texto.length() == 0) cout << "Nao ha texto para analisar\n";
 
    // ordem = "Z Y X W V U T S R Q P O N M L K J I H G F E D C B A";
    // texto = "Era uma vez UMA gata xadrez.";
    
    // ordem = "Q A Z W S X E D C R F V T G B Y H N M U J I K O L P";
    // texto = "Sofisticado, o Tenis Adidas Top Top  e o tipo de  que faz uma no look da .em  , sua parte um   por  . Alem disso, tem  em  para   em  .  os  de sua , va de e faca efeito em.";

    ConjuntoPalavras todasPalavras;
    todasPalavras.setChaves(ordem);

    todasPalavras.AdicionaTexto(texto);
    // for (int i=0; i<26; i++) cout << todasPalavras.chaves[i] << "    ";
    // cout << endl;
    // for (int i=0; i<26; i++) cout << (char)(i+65) << "     ";
    // cout << endl;
    todasPalavras.Quicksort(parser.m, parser.s);

    todasPalavras.PrintFile(parser.output_path);
    
    return 0;
}