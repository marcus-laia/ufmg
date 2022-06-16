#include <iostream>

using namespace std;

class node
{
private:
    int value;
    int count;
    node *esq;
    node *dir;
public:
    node(int val);
    ~node();
friend class tree;
};

node::node(int val)
{
    value = val;
    count = 1;
    esq = NULL;
    dir = NULL;
}

node::~node()
{
}

class tree
{
private:
    node *root;
    void insertRec(node* &p, int val);
    void printRec(node* p);
    void delRec(node *p);
public:
    tree();
    ~tree();
    void insert(int val);
    void print();
    void clear();
};

tree::tree()
{
    root = NULL;
}

tree::~tree()
{
}

void tree::insertRec(node* &p, int val)
{
    if (p==NULL)
    {
        p = new node(val);
    }
    else
    {
        if (val < p->value)
        {
            insertRec(p->esq, val);
        }
        else if (val > p->value)
        {
            insertRec(p->dir, val);
        } 
        else 
        {
            p->count++;
        }   
    }     
}

void tree::insert(int val)
{
    insertRec(root, val);
}

void tree::printRec(node *p)
{
    if (p==NULL)
    {
        return;
    }
    else
    {
        printRec(p->esq);
        cout << p->value << " aparece " << p->count << " vez(es)" << endl;
        printRec(p->dir);
    }
}

void tree::print()
{
    printRec(root);
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

    tree aux;
    int n;
    int nval;

    cin >> n;

    for (int i=0; i<n; i++)
    {
        cin >> nval;
        aux.insert(nval);
    }

    aux.print();

    aux.clear();
    
    return 0;
}