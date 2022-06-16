#include <card.h>

// -------------- LIST CLASSES --------------
class cell
{
    private:
        card *item;
        cell *prox;
    public:
        cell();
    friend class lista;
};

cell::cell()
{
    item = NULL;
    prox = NULL;
}

class list
{
private:
    int lenght;
    cell* first;
    cell* last;
    cell* getPos(int pos, bool antes);

public:
    list();
    ~list();
    int getLen() { return lenght; };
    bool empty() { return lenght==0; };

    void insert();
};

list::list()
{
}

list::~list()
{
}
