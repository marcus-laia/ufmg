#include <iostream>
#include <string>
#include "hand.h"

using namespace std;

class Player
{
private:
    string name;
    double money;
    Hand hand;
public:
    Player(string name, int initial_money);
    ~Player();

    void printHand();
};

Player::Player(string Player_name, int initial_money)
{
    name = Player_name;
    money = initial_money;
}

Player::~Player(){}

void Player::printHand()
{
    for (int i=0; i<5; i++)
    {
        cout << hand.cards[i].number << hand.cards[i].suit << " ";
    }
    cout << "\n";
}