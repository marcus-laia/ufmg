#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

void compareFiles(string path1, string path2)
{
    bool leu = false;

    ifstream f1(path1);
    ifstream f2(path2);

    string linha1, linha2;

    while (getline(f1, linha1))
    {
        leu = true;
        getline(f2, linha2);
        if (linha1.compare(linha2) != 0) 
        {
            cout << "Line not ok\n";
            return;
        }
    }
    if(leu) cout << "All ok" << endl;
    else cout << "Error reading the file" << endl;
}

int main(int argc, char const *argv[])
{
    char outname[35];    
    char myout[30];

    sprintf(outname, "./tests/expected/expected_%s.txt",argv[1]);
    sprintf(myout, "./tests/output/output_%s.txt",argv[1]);

    compareFiles(myout, outname);
    return 0;
}
