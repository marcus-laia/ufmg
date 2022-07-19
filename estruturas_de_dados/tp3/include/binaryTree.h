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
    int insertRec(node* &p, message email);
    int removeRec(node* &p, int key);
    message searchRec(node* &p, int key);
    void printRec(node* p);
    void delRec(node *p);
    void antecessor(node *q, node *&r);
public:
    binaryTree();
    ~binaryTree();
    int insert(message email);
    int remove(int key);
    message search(int key);
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

int binaryTree::insertRec(node* &p, message email)
{
    if (p==NULL)
    {
        p = new node();
        p->msg = email;
        return 1;
    }
    else
    {
        if (email.key < p->msg.key)
        {
            return insertRec(p->esq, email);
        }
        else if (email.key > p->msg.key)
        {
            return insertRec(p->dir, email);
        }
        else return 0;
    }     
}

int binaryTree::insert(message email)
{
    return insertRec(root, email);
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

int binaryTree::removeRec(node* &p, int key)
{
    if (p==NULL)
    {
        return 0;
    }
    else
    {
        if (key < p->msg.key)
        {
            return removeRec(p->esq, key);
        }
        else if (key > p->msg.key)
        {
            return removeRec(p->dir, key);
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

            return 1;
        }
    }     
}

int binaryTree::remove(int key)
{
    return removeRec(root, key);
}

message binaryTree::searchRec(node* &p, int key)
{
    if (p==NULL)
    {
        message aux;
        aux.setKey(-1);
        return aux;
    }
    if (key < p->msg.key)
    {
        return searchRec(p->esq, key);
    }
    else if (key > p->msg.key)
    {
        return searchRec(p->dir, key);
    }
    else
    {
        return p->msg;
    }    
}

message binaryTree::search(int key)
{
    return searchRec(root, key);
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