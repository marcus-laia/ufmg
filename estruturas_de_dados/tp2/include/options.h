#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "msgassert.h"
#include <iostream>
using namespace std;

class argParser
{
private:
public:
    char lognome[100];
    int m, s, regmem;
    char input_path[100], output_path[100], path_mat_res[100];
    
    argParser();

    void uso();
    void parse_args(int argc, char **argv);
};

argParser::argParser()
{
	input_path[0] = 0;
	output_path[0] = 0;
    lognome[0] = 0;
	m = 0;
	s = 0;
}

// variaveis globais para opcoes

void argParser::uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
	fprintf(stderr, "matop\n");
	fprintf(stderr, "\t-i <arq>\t(arquivo de entrada)\n");
	fprintf(stderr, "\t-m \t(numero m de elementos para mediana do quicksort) \n");
	fprintf(stderr, "\t-s \t(numero s de elementos por particao para mudar o metodo de ordenacao)\n");
	fprintf(stderr, "\t-o <arq>\t(arquivo de saida)\n");
	fprintf(stderr, "\t-p <arq>\t(arquivo de registro de acesso)\n");
	fprintf(stderr, "\t-l \t(registrar acessos a memoria)\n");
}

void argParser::parse_args(int argc, char **argv)
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc e argv
// Saida: optescolhida, optx, opty, regmem, lognome
{
	// variaveis externas do getopt
	extern char *optarg;
	// extern int optind;

	// variavel auxiliar
	int c;

	// getopt - letra indica a opcao, : junto a letra indica parametro
	// no caso de escolher mais de uma operacao, vale a ultima
	while ((c = getopt(argc, argv, "i:I:m:M:s:S:o:O:p:lh")) != EOF)
	{        
        // converte letra maiuscula para minuscula
        if (c >= 65 && c <= 90) {c += 32;}

		switch (c)
		{
		case 'i':
			strcpy(input_path, optarg);
			break;
		case 'm':
			m = stoi(optarg);
			break;
		case 's':
			s = stoi(optarg);
			break;
		case 'o':
			strcpy(output_path, optarg);
			break;
		case 'p':
			strcpy(lognome, optarg);
			break;
		case 'l':
			regmem = 1;
			break;
        case 'h':
			uso();
			exit(1);
		default:
            cout << "Flag Invalida" << "\n";
            break;
		}
	}
	// verificacao da consistencia das opcoes
	// erroAssert(strlen(lognome) > 0,
	// 		   "matop - nome de arquivo de registro de acesso tem que ser definido");
}
