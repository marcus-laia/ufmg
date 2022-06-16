#include "card.h"

class Hand
{
private:
    Card cards[5];
    int card_count;
    int same_num_counter[2];
    
    bool isFlush;
    bool isStraight;
    bool isRoyal;

    void getHandType();
    void sortCards();
public:
    Hand();
    ~Hand();
    void addCard(string card_str);
    int getScore();
    int getStronger();
friend class Player;
};

Hand::Hand()
{
    card_count = 0;
    same_num_counter[0] = 0;
    same_num_counter[1] = 0;

    isFlush = false;
    isStraight = false;
    isRoyal = false;
}

Hand::~Hand()
{
}

void Hand::addCard(string card_str)
{
    cards[card_count] = Card(card_str);
    card_count++;
}

void Hand::sortCards()
{
    // insertion algorithm
    for (int i=1; i<5; i++)
    {
        Card key = cards[i];
        int j = i - 1;

        while (key.number < cards[j].number && j>=0)
        {
            cards[j+1] = cards[j];
            j--;
        }
        cards[j+1] = key;        
    }
}

void Hand::getHandType()
{
    int sameNumCount = 1;
    int auxSameNum = 0;

    isFlush = true;
    isStraight = true;

    for (int i=1; i<5; i++)
    {
        if (cards[i].number == cards[i-1].number) sameNumCount++;
        else
        {
            if (sameNumCount>1) same_num_counter[auxSameNum] = sameNumCount;
            sameNumCount = 1;
            auxSameNum++;
        }
        if (sameNumCount>1) same_num_counter[auxSameNum] = sameNumCount;

        if (cards[i].suit != cards[i-1].suit) isFlush = false;

        if (i>1)
        {
            if (cards[i].number - cards[i-1].number != 1) isStraight = false;
        }
    }
    if (isStraight)
    {
        if (cards[0].number == 1 && cards[1].number == 10) isRoyal = true;
        else if (cards[0].number - cards[0].number != 1) isStraight = false;
    }
}

int Hand::getScore()
{

}

int Hand::getStronger()
{

}