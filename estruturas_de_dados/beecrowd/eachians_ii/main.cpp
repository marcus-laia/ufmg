#include <iostream>
#include <string.h>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[])
{
    int N = 0;

    string sentence;
    string word;
    string aux;

    int idx;
    bool found;

    cin >> N;
    cin.ignore();
    // cout << "lets read " << N << " words\n";
    for (int i=0; i<N; i++)
    {
        idx = 0;
        found = 0;
        
        getline(cin, sentence);
        cin >> word;
        cin.ignore();

        // cout << "the sentence " << i+1 << " is " << sentence << "\n";
        // cout << "the word " << i+1 << " is " << word << "\n";

        stringstream ss(sentence);

        while (ss >> aux)
        {
            // cout << "the aux word is " << aux << "\n";
            if (word.compare(aux) == 0)
            {
                if (found) cout << " ";
                cout << idx;
                found = true;
                idx = idx + aux.length() + 1;
            }
            else 
            {
                idx = idx + aux.length() + 1;
            }
        }
        if (!found) cout << "-1";
        cout << "\n";
    }
    
    return 0;
}
