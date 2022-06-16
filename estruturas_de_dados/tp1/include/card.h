#include <iostream>
#include <string>

using namespace std;

class Card
{
private:
    int number;
    char suit;
public:
    Card();
    Card(string card_str);
    ~Card();
friend class Hand;
friend class Player;
};

Card::Card(string card_str)
{
    suit = card_str.back();
    card_str.pop_back();
    number = stoi(card_str);
}

Card::~Card(){}