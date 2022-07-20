#pragma once

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
    // inicializa o no com filhos nulos
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
    
    bool insertRec(node* &p, message email);
    bool removeRec(node* &p, int user, int key);
    message searchRec(node* &p, int key);
    void delRec(node *p);
    void predecessor(node *q, node *&r);
public:
    binaryTree();
    ~binaryTree();
    bool insert(message email);
    bool remove(int user, int key);
    message search(int key);
    void clear();
};

binaryTree::binaryTree()
{
    root = NULL;
}

binaryTree::~binaryTree()
{
}

bool binaryTree::insertRec(node* &p, message email)
{
    // se o no eh nulo
    if (p==NULL)
    {
        // cria um novo no
        p = new node();
        // guarda a mensagem
        p->msg = email;
        return true;
    }
    // se o no nao eh nulo
    else
    {
        // se a nova chave eh menor que a do no
        if (email.key < p->msg.key)
        {
            // insere a partir do no filho da esquerda
            return insertRec(p->esq, email);
        }
        // se a nova chave eh maior que a do no
        else if (email.key > p->msg.key)
        {
            // insere a partir do no filho da direita
            return insertRec(p->dir, email);
        }
        // se a nova chave eh igual a do no, retorna falha na insercao
        else return false;
    }     
}

bool binaryTree::insert(message email)
{
    // insere o email a partir da raiz
    return insertRec(root, email);
}

void binaryTree::predecessor(node *q, node *&r)
{
    // substitui o no q pelo no descendente de r mais a direita
    
    // se r tem no filho a direita
    if (r->dir != NULL)
    {
        // desce mais um nivel
        predecessor(q, r->dir);
    }
    // se r nao tem mais filhos a direita
    else
    {
        // substitui a mensagem de q pela de r
        q->msg = r->msg;
        // faz a variavel q apontar para o no r
        q = r;
        // o filho da esquerda substitui o antigo r
        r = r->esq;
        // libera da memoria q, o antigo r
        free(q);
    }
}

bool binaryTree::removeRec(node* &p, int user, int key)
{
    // se o no eh nulo
    if (p==NULL)
    {
        // nao encontrou o no desejado
        return false;
    }
    // se o no nao eh nulo
    else
    {
        // se a chave eh menor que a do no
        if (key < p->msg.key)
        {
            // desce para o filho da esquerda
            return removeRec(p->esq, user, key);
        }
        // se a chave eh maior que a do no
        else if (key > p->msg.key)
        {
            // desce para o filho da direita
            return removeRec(p->dir, user, key);
        }
        // se a chave eh igual a do no
        else
        {
            // se o identificador do ususario corresponde
            if (user == p->msg.user)
            {
                // cria um no auxiliar para liberar a memoria do removido
                node *aux;
                
                // se o filho da direita for nulo
                if (p->dir == NULL)
                {
                    aux = p;
                    // o no removido eh substituido pelo da esquerda (pode ser nulo)
                    p = p->esq;
                    free(aux);
                }
                // se o da direita nao eh nulo mas o da esquerda eh
                else if (p->esq == NULL)
                {
                    aux = p;
                    // substitui o no removido pelo da direita
                    p = p->dir;
                    free(aux);
                }
                // se o no tiver dois filhos, substitui ele pelo antecessor
                else predecessor(p, p->esq);

                return true;
            }
            // se o usuario nao corresponde, nao deve deletar o email
            else return false;
        }
    }     
}

bool binaryTree::remove(int user, int key)
{
    // remove o email com chave desejada a partir da raiz
    return removeRec(root, user, key);
}

message binaryTree::searchRec(node* &p, int key)
{
    // se o no eh nulo, nao encontrou a chave buscada
    if (p==NULL)
    {
        // retorna uma mensagem vazia
        message aux;
        aux.setKey(-1);
        return aux;
    }
    // se a chave eh menor que a do no
    if (key < p->msg.key)
    {
        // procura a partir do no da esquerda
        return searchRec(p->esq, key);
    }
    // se a chave eh maior que a do no
    else if (key > p->msg.key)
    {
        // procura a partir do no da direita
        return searchRec(p->dir, key);
    }
    // se a chave eh igual a do no
    else
    {
        // retorna a mensagem do no
        return p->msg;
    }    
}

message binaryTree::search(int key)
{
    // procura uma chave a partir da raiz
    return searchRec(root, key);
}

void binaryTree::delRec(node *p)
{
    // se o no eh nulo, apenas retorna
    if (p == NULL)
    {
        return;
    }
    else
    {
        // deleta recursivamente os filhos da esquerda
        delRec(p->esq);
        // os filhos da direita
        delRec(p->dir);
        // e por fim o proprio no
        delete p;
    }
}

void binaryTree::clear()
{
    // apaga da memoria todos os nos a partir da raiz
    delRec(root);
    // define a raiz como nulo
    root = NULL;
}