#include "binaryTree.h"

class Hash
{
private:
    int len;
    binaryTree *table;
public:
    Hash(int M);
    ~Hash();
    int getPos(int val);
    void insert(message email);
    void remove(int user, int emailKey);
};

Hash::Hash(int M)
{
    len = M;
    binaryTree aux[M];
    table = aux;
}

Hash::~Hash()
{
}

int Hash::getPos(int val)
{
    return val % len;
}

void Hash::insert(message email)
{
    // obtem o indice de insercao a partir do valor de U
    int pos = getPos(email.getUser());
    table[pos].insert(email);
}

void Hash::remove(int user, int emailKey)
{
    // obtem o indice de insercao a partir do valor de U
    int pos = getPos(user);
    table[pos].remove(emailKey);
}