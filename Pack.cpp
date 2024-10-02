#include "Pack.h"
#include <cassert>
#include <iostream>
using namespace std;

// Project UID 1d9f47bfc76643019cfbf037641defe1

Pack::Pack() {
    next = 0;
    int iterator = 0;

    for (int i = 0; i < NUM_SUITS; i++) {
        for (int j = 0; j < 6; j++) {
            cards[iterator] = Card(RANK_NAMES_BY_WEIGHT[j + 7], SUIT_NAMES_BY_WEIGHT[i]);
            iterator++;
        }
    }
}

Pack::Pack(istream& pack_input) {
    next = 0;
    int iterator = 0;
    string rank;
    string of; //junk variable
    string suit;

    while (pack_input.good()) {
        pack_input >> rank >> of >> suit;
        cards[iterator] = Card(rank, suit);
        iterator++;
        if (iterator == 24) {
            break;
        }
    }
}

Card Pack::deal_one() {
    Card card = cards[next];
    next++;
    return card;
}

void Pack::reset() {
   next = 0;
}

void Pack::shuffle() {
    array<Card, PACK_SIZE / 2> top_half;
    array<Card, PACK_SIZE / 2> bottom_half;
    int iterator;

    for (int j = 0; j < 7; j++) {
        iterator = 0;
        for (int i = 0; i < PACK_SIZE; i++) {
            if (i <= 11) {
                top_half[i] = cards[i];
            }
            else {
                bottom_half[iterator] = cards[i];
                iterator++;
            }
        }  
        for (int i = 0; i < (PACK_SIZE / 2); i++) {
            cards[2 * i] = bottom_half[i];
            cards[(2 * i) + 1] = top_half[i];
        }
    }
}

bool Pack::empty() const {
    if (next > 23) {
        return true;
    }
    else {
        return false;
    }
}