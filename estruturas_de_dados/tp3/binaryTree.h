#include <iostream>
#include "message.h"

using namespace std;

class node
{
private:
    message msg;
    node *esq;
    node *dir;
public:
    node();
    ~node();
friend class binaryTree;
};

node::node()
{
    esq = NULL;
    dir = NULL;
}

node::~node()
{
}

class binaryTree
{
private:
    node *root;
    void insertRec(node* &p, message email);
    void removeRec(node* &p, int key);
    void printRec(node* p);
    void delRec(node *p);
    void antecessor(node *q, node *&r);
public:
    binaryTree();
    ~binaryTree();
    void insert(message email);
    void remove(int key);
    void print();
    void clear();
};

binaryTree::binaryTree()
{
    root = NULL;
}

binaryTree::~binaryTree()
{
}

void binaryTree::insertRec(node* &p, message email)
{
    if (p==NULL)
    {
        p = new node();
        p->msg = email;
    }
    else
    {
        if (email.key < p->msg.key)
        {
            insertRec(p->esq, email);
        }
        else if (email.key > p->msg.key)
        {
            insertRec(p->dir, email);
        }
        else cout << "JA EXISTE\n";
    }     
}

void binaryTree::insert(message email)
{
    insertRec(root, email);
}

void binaryTree::antecessor(node *q, node *&r)
{
    if (r->dir != NULL)
    {
        antecessor(q, r->dir);
    }
    else
    {
        q->msg = r->msg;
        q = r;
        r = r->esq;
        free(q);
    }
}

void binaryTree::removeRec(node* &p, int key)
{
    if (p==NULL)
    {
        cout << "NAO EXISTE\n";
    }
    else
    {
        if (key < p->msg.key)
        {
            removeRec(p->esq, key);
        }
        else if (key > p->msg.key)
        {
            removeRec(p->dir, key);
        }
        else
        {
            node *aux;
            if (p->dir == NULL)
            {
                aux = p;
                p = p->esq;
                free(aux);
            }
            else if (p->esq == NULL)
            {
                aux = p;
                p = p->dir;
                free(aux);
            }
            else antecessor(p, p->esq);
        }
    }     
}

void binaryTree::remove(int key)
{
    removeRec(root, key);
}

void binaryTree::printRec(node *p)
{
    if (p==NULL)
    {
        return;
    }
    else
    {
        printRec(p->esq);
        cout << p->msg.key << " \n";
        printRec(p->dir);
    }
}

void binaryTree::print()
{
    printRec(root);
}

void binaryTree::delRec(node *p)
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

void binaryTree::clear()
{
    delRec(root);
    root = NULL;
}