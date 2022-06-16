// N = number of people
// L = total
// Q = cuia

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main() 
{
    int N=0;
    float L=0, Q=0;
    
    scanf("%d %f %f", &N, &L, &Q);

    char pessoas[N][12];

    for (int i = 0; i < N; i++)
    {
        scanf("%12s", pessoas[i]);
    }

    int cuias = L/Q;
    float ultimo_vol = L - cuias*Q;

    int idx_pessoa = cuias%N;

    if (ultimo_vol==0) 
    {
        ultimo_vol = Q;
        idx_pessoa--;

        if (idx_pessoa < 0) idx_pessoa = N-1;
    }

    printf("%s %.1f\n", pessoas[idx_pessoa], ultimo_vol);
 
    return 0;
}