#include <stdio.h>
#include <stdlib.h>

class celula
{
private:
    celula *prox;

public:
    celula();

    friend class pilha;
};

celula::celula()
{
    this->prox = NULL;
}

class pilha
{
private:
    celula *topo;
    int tam;

public:
    pilha();
    ~pilha();
    
    void empilha();
    void desempilha();
    void limpa();
    int getTam();
};

pilha::pilha()
{
    this->tam = 0;
    this->topo = NULL;
}

pilha::~pilha()
{
    this->limpa();
};

void pilha::empilha()
{
    celula *nova = new celula();
    // nova->item = 0;
    nova->prox = this->topo;
    this->topo = nova;
    this->tam++;
}

void pilha::desempilha()
{
    if (this->topo == NULL)
    {
        return;
    }
    celula *aux_del = this->topo;
    this->topo = this->topo->prox;
    delete aux_del;
    this->tam--;
}

void pilha::limpa()
{
    if (this->topo == NULL)
    {
        return;
    }
    while(this->topo->prox != NULL)
    {
        this->desempilha();
    }
    this->desempilha();
}

int pilha::getTam()
{
    return this->tam;
}

int main()
{

    char inp;
    // int remain = 0;
    pilha assuntos = pilha();

    // assuntos->tam = 0;

    // scanf("%s", inp);

    do
    {
        inp = getchar();

        if (inp == '(')
        {
            assuntos.empilha();
            // remain++;
        } else if (inp == ')')
        {
            assuntos.desempilha();
            // remain--;
        }
    } while(inp!='\n');

    int remain = assuntos.getTam();
    
    if (remain > 0){
        printf("Ainda temos %d assunto(s) pendente(s)!\n", remain);
    } else {
        printf("Partiu RU!\n");
    }

    // destroiPilha(assuntos);
    assuntos.limpa();

    // delete assuntos;

    return 0;
}