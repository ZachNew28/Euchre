// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <string>
#include "Card.h"
using namespace std;
// add any necessary #include or using directives here
static int determine_rank(const Card &card);
static int determine_suit(const Card &card);

// rank and suit names -- do not remove these
constexpr const char* const Card::RANK_TWO;
constexpr const char* const Card::RANK_THREE;
constexpr const char* const Card::RANK_FOUR;
constexpr const char* const Card::RANK_FIVE;
constexpr const char* const Card::RANK_SIX;
constexpr const char* const Card::RANK_SEVEN;
constexpr const char* const Card::RANK_EIGHT;
constexpr const char* const Card::RANK_NINE;
constexpr const char* const Card::RANK_TEN;
constexpr const char* const Card::RANK_JACK;
constexpr const char* const Card::RANK_QUEEN;
constexpr const char* const Card::RANK_KING;
constexpr const char* const Card::RANK_ACE;

constexpr const char* const Card::SUIT_SPADES;
constexpr const char* const Card::SUIT_HEARTS;
constexpr const char* const Card::SUIT_CLUBS;
constexpr const char* const Card::SUIT_DIAMONDS;

Card::Card() {
  rank = RANK_TWO;
  suit = SUIT_SPADES;
}

Card::Card(const string &rank_in, const string &suit_in) {
  rank = rank_in;
  suit = suit_in;
}

string Card::get_rank() const {
    return rank;
}

string Card::get_suit() const {
    return suit;
}

string Card::get_suit(const string &trump) const {
    if ((is_left_bower(trump)) && (get_suit() == SUIT_CLUBS)) {
        return SUIT_SPADES;
    }
    else if ((is_left_bower(trump)) && (get_suit() == SUIT_SPADES)) {
        return SUIT_CLUBS;
    }
    else if ((is_left_bower(trump)) && (get_suit() == SUIT_DIAMONDS)) {
        return SUIT_HEARTS;
    }
    else if ((is_left_bower(trump)) && (get_suit() == SUIT_HEARTS)) {
        return SUIT_DIAMONDS;
    }
    else {
        return get_suit();
    }
}

bool Card::is_face_or_ace() const {
    if (get_rank() == RANK_ACE) {
        return true;
    }
    else if (get_rank() == RANK_KING) {
        return true;
    }
    else if (get_rank() == RANK_QUEEN) {
        return true;
    }
    else if (get_rank() == RANK_JACK) {
        return true;
    }
    else {
        return false;
    }
}

bool Card::is_right_bower(const string &trump) const {
    if (get_suit() == trump) {
        if (get_rank() == RANK_JACK) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool Card::is_left_bower(const string &trump) const {
    if (get_rank() == RANK_JACK) {
        if (get_suit() == trump) {
            return false;
        }
        else if (trump == SUIT_CLUBS && get_suit() == SUIT_SPADES) {
            return true;
        }
        else if (trump == SUIT_SPADES && get_suit() == SUIT_CLUBS) {
            return true;
        }
        else if (trump == SUIT_DIAMONDS && get_suit() == SUIT_HEARTS) {
            return true;
        }
        else if (trump == SUIT_HEARTS && get_suit() == SUIT_DIAMONDS) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool Card::is_trump(const string &trump) const {
    if (get_suit() == trump) {
        return true;
    }
    else if (is_left_bower(trump)) {
        return true;
    }
    else {
        return false;
    }
}

bool operator<(const Card &lhs, const Card &rhs) {
    int left_rank_value = determine_rank(lhs);
    int left_suit_value = determine_suit(lhs);
    int right_rank_value = determine_rank(rhs);
    int right_suit_value = determine_suit(rhs);

    if (left_rank_value == right_rank_value) {
        if (left_suit_value < right_suit_value) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (left_rank_value < right_rank_value) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool operator<=(const Card &lhs, const Card &rhs) {
    return lhs < rhs || lhs == rhs;
}

bool operator>(const Card &lhs, const Card &rhs) {
    return !(lhs <= rhs);
}

bool operator>=(const Card &lhs, const Card &rhs) {
    return !(lhs < rhs);
}

bool operator==(const Card &lhs, const Card &rhs) {
    return ((lhs.get_rank() == rhs.get_rank()) && (lhs.get_suit() == rhs.get_suit()));
}

bool operator!=(const Card &lhs, const Card &rhs) {
    return !(lhs == rhs);
}

string Suit_next(const string &suit) {
    if (suit == Card::SUIT_CLUBS) {
        return Card::SUIT_SPADES;
    }
    else if (suit == Card::SUIT_SPADES) {
        return Card::SUIT_CLUBS;
    }
    else if (suit == Card::SUIT_HEARTS) {
        return Card::SUIT_DIAMONDS;
    }
    else if (suit == Card::SUIT_DIAMONDS) {
        return Card::SUIT_HEARTS;
    }
    else {
        return "Error";
    }
}

ostream & operator<<(ostream &os, const Card &card) {
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

bool Card_less(const Card &a, const Card &b, const string &trump) {
    if (!a.is_trump(trump) && !a.is_right_bower(trump)) {
        if (a < b) {
            return true;
        }
        else if (b.is_trump(trump) || b.is_right_bower(trump)) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool Card_less(const Card &a, const Card &b, const Card &led_card, const string &trump) {
    if (a == b) {
        return false;
    }
    if ((a.get_suit() == b.get_suit()) 
        && (!b.is_right_bower(trump) && !b.is_left_bower(trump))) {
        if (a > b) {
            return false;
        }
    }
    if (a.is_right_bower(trump)) {
        return false;
    }
    else if (a.is_left_bower(trump) && !b.is_right_bower(trump)) {
        return false;
    }
    else if (a.is_trump(trump) && !b.is_trump(trump)) {
        return false;
    }
    else if ((a.get_suit() == led_card.get_suit()) && !b.is_trump(trump) 
            && b.get_suit() != led_card.get_suit()) {
        return false;
    }
    else if (!a.is_trump(trump) && a.get_suit() != led_card.get_suit()
            && b.get_suit() == led_card.get_suit() && !led_card.is_left_bower(trump)) {
                return true;
            }
    else if (a > b && !b.is_trump(trump)) {
        return false;
    }
    else {
        return true;
    }
}

//Helper functions
static int determine_rank(const Card &card) {
    int rank_rank = 0;

    for (int i = 0; i < NUM_RANKS; i++) {
        if (RANK_NAMES_BY_WEIGHT[rank_rank] == card.get_rank()) {
            break;
        }
        else {
            rank_rank++;
        }
    }

    return rank_rank;
}

static int determine_suit(const Card &card) {
    int suit_rank = 0;

    for (int i = 0; i < NUM_SUITS; i++) {
        if (SUIT_NAMES_BY_WEIGHT[suit_rank] == card.get_suit()) {
            break;
        }
        else {
            suit_rank++;
        }
    }

    return suit_rank;
}

// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
