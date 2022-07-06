#include <iostream>
#include <string.h>

using namespace std;

class node
{
private:
    string value;
    node *esq;
    node *dir;
public:
    node(string val);
friend class tree;
};

node::node(string val)
{
    value = val;
    esq = NULL;
    dir = NULL;
}


class tree
{
private:
    node *root;
    void insertRec(node* &p, string val);
    int procuraRec(node* &p, string val);
    void printInOrder(node* p);
    void printPreOrder(node* p);
    void printPosOrder(node* p);
    void delRec(node *p);
public:
    tree();
    ~tree();
    void insert(string val);
    int procura(string val);
    void print(int type);
    void clear();
};

tree::tree()
{
    root = NULL;
}

tree::~tree()
{
}

void tree::insertRec(node* &p, string val)
{
    if (p==NULL)
    {
        p = new node(val);
    }
    else
    {
        if (val.compare(p->value) < 0)
        {
            insertRec(p->esq, val);
        }
        else if (val.compare(p->value) > 0)
        {
            insertRec(p->dir, val);
        }
    }     
}

int tree::procuraRec(node* &p, string val)
{
    if (p==NULL)
    {
        return 0;
    }
    else
    {
        if (val.compare(p->value) < 0)
        {
            return procuraRec(p->esq, val);
        }
        else if (val.compare(p->value) > 0)
        {
            return procuraRec(p->dir, val);
        }
        else
        {
            return 1;
        }
    }     
}

void tree::insert(string val)
{
    insertRec(root, val);
}

int tree::procura(string val)
{
    return procuraRec(root, val);
}

void tree::printInOrder(node *p)
{
    if (p==NULL)
    {
        return;
    }
    else
    {
        printInOrder(p->esq);
        cout << p->value << " ";
        printInOrder(p->dir);
    }
}

void tree::printPreOrder(node *p)
{
    if (p==NULL)
    {
        return;
    }
    else
    {
        cout << p->value << " ";
        printPreOrder(p->esq);
        printPreOrder(p->dir);
    }
}

void tree::printPosOrder(node *p)
{
    if (p==NULL)
    {
        return;
    }
    else
    {
        printPosOrder(p->esq);
        printPosOrder(p->dir);
        cout << p->value << " ";
    }
}

void tree::print(int type)
{
    if (type==1) printPreOrder(root);
    if (type==2) printInOrder(root);
    if (type==3) printPosOrder(root);
    cout << "\n";
}

void tree::delRec(node *p)
{
    if (p == NULL)
    {
        return;
    }
    else
    {
        delRec(p->esq);
        delRec(p->dir);
        delete p;
    }
}

void tree::clear()
{
    delRec(root);
    root = NULL;
}

int main()
{
    tree arvore;
    string entrada;
    string valor;

    while (cin >> entrada)
    {
        if (entrada.compare("I")==0)
        {
            cin >> valor;
            arvore.insert(valor);
        }
        else if (entrada.compare("P")==0)
        {
            cin >> valor;
            if(arvore.procura(valor)) cout << valor << " existe" << "\n";
            else cout << valor << " nao existe" << "\n";
        }
        else if (entrada.compare("PREFIXA")==0)
        {
            arvore.print(1);
        }
        else if (entrada.compare("INFIXA")==0)
        {
            arvore.print(2);
        }
        else if (entrada.compare("POSFIXA")==0)
        {
            arvore.print(3);
        }
    }

    arvore.clear();
    
    return 0;
}