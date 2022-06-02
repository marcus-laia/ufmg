#include <string>

using namespace std;

// -------------- CARD CLASS --------------
class card
{
private:
    int number;
    char suit;
public:
    card(string card_rep);
    ~card();
};

card::card(string card_rep)
{
    suit = card_rep.back();
    card_rep.pop_back();
    number = stoi(card_rep);
}

card::~card(){}

// -------------- HAND CLASS --------------



// -------------- PLAYER CLASS --------------
class player
{
private:
    string name;
    double money;
public:
    player(string name, int initial_money);
    ~player();
};

player::player(string player_name, int initial_money)
{
    name = player_name;
    money = initial_money;
}

player::~player(){}

// FAZER METODOS PARA IDENTIFICAR AS SEQUENCIAS POSSIVEIS