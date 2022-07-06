
class Item
{
private:
public:
    Item();
    int chave;
};

Item::Item()
{
    chave = -1;
}

void particao(int esq, int dir, int *i, int *j, Item *vec)
{
    Item pivo, aux_troca;
    *i = esq; *j = dir;
    pivo = vec[(*i + *j)/2];

    do
    {
        while (pivo.chave > vec[*i].chave) (*i)++;
        while (pivo.chave < vec[*j].chave) (*j)--;
        if (*i <= *j)
        {
            aux_troca = vec[*i]; vec[*i] = vec[*j]; vec[*j] = aux_troca;
            (*i)++; (*j)--;
        }
    } while (*i <= *j);
}

void ordena(int esq, int dir, Item *vec)
{
    int i; int j;
    particao(esq, dir, &i, &j, vec);
    if (esq < j) ordena(esq, j, vec);
    if (dir > i) ordena(i, dir, vec);
}

void quicksort(Item *vec, int n)
{
    ordena(0, n-1, vec);
}