#include <iostream>
#include <string>
#include <cassert>
#include "Player.h"
#include <algorithm>
using namespace std;

// Project UID 1d9f47bfc76643019cfbf037641defe1

static string get_same_color_suit(string current_suit);

class Human : public Player {
  public:
    Human(string name_in);

    const std::string & get_name() const {
      return name;
    };

    void add_card(const Card &c) {
      hand.push_back(c);
    };

    bool make_trump(const Card &upcard, bool is_dealer, int round, 
                    std::string &order_up_suit) const {
                      string suit_in;
                      //Sort will need to get called in the driver before its printed
                      print_hand();
                      cout << "Human player " << name 
                      << ", please enter a suit, or \"pass\":\n";

                      cin >> suit_in;

                      if (round == 1) {
                        if (suit_in == upcard.get_suit()) {
                          order_up_suit = suit_in;
                          return true;
                        }
                        else {
                          return false;
                        }
                      }
                      else if (round == 2 && !is_dealer) {
                        if (suit_in != upcard.get_suit() && suit_in != "pass") {
                          order_up_suit = suit_in;
                          return true;
                        }
                        else {
                          return false;
                        }
                      }
                      else {
                        order_up_suit = suit_in;
                        return true;
                      }
                    };

    void add_and_discard(const Card &upcard) {
      sort(hand.begin(), hand.end());
      int discard_index;
      print_hand();
      cout << "Discard upcard: [-1]\n";
      cout << "Human player " << name << ", please select a card to discard:\n";

      cin >> discard_index;

      if (discard_index == -1) {
        return;
      }
      else {
        hand.at(discard_index) = upcard;
      }
    };

    Card lead_card(const std::string &trump) {
      int card_index;
      Card leadCard;
      sort(hand.begin(), hand.end());
      print_hand();
      cout << "Human player " << name << ", please select a card:\n";

      cin >> card_index;

      leadCard = hand.at(card_index);
      hand.erase(hand.begin() + card_index);
      return leadCard;
    };

    Card play_card(const Card &led_card, const std::string &trump) {
      int card_index;
      Card playCard;
      sort(hand.begin(), hand.end());
      print_hand();
      cout << "Human player " << name << ", please select a card:\n";

      cin >> card_index;

      playCard = hand.at(card_index);
      hand.erase(hand.begin() + card_index);
      return playCard;
    };

  private:
    string name;

    void print_hand() const {
      vector<Card> copy = hand;
      sort(copy.begin(), copy.end());

      for (size_t i=0; i < hand.size(); ++i) {
        cout << "Human player " << name << "'s hand: "
            << "[" << i << "] " << copy[i] << "\n";
      }
    }

    vector<Card> hand;
};

class Simple : public Player {
public:
  Simple(string name_in);

  const std::string & get_name() const {
    return name;
  };

  void add_card(const Card &c) {
    hand.push_back(c);
  };

  bool make_trump(const Card &upcard, bool is_dealer, int round, 
      std::string &order_up_suit) const {
    int num_face_or_ace = 0;

    if (round == 1) {
      //Calculate num of trump ace or face cards the player will get
      for (int i = 0; i < hand.size(); i++) {
        if ((hand.at(i).is_face_or_ace()
            && (hand.at(i).get_suit() == upcard.get_suit()))
            || hand.at(i).is_left_bower(upcard.get_suit())) {
              num_face_or_ace++;
        }
      }
      //If they get 2 or more ace or face cards for making it trump,
      //make it trump
      if (num_face_or_ace >= 2) {
        order_up_suit = upcard.get_suit();
        return true;
      }
      else {
        return false;
      }
    }
    else { //round 2
      if (!is_dealer) {
        //Calculate num of trump ace or face cards the player will get
        for (int i = 0; i < hand.size(); i++) {
          if ((hand.at(i).is_face_or_ace()
              && (hand.at(i).get_suit() == 
              get_same_color_suit(upcard.get_suit())))
              || hand.at(i).is_left_bower(get_same_color_suit
              (upcard.get_suit()))) {
                num_face_or_ace++;
          }
        }
        if (num_face_or_ace >= 1) {
          order_up_suit = get_same_color_suit(upcard.get_suit());
          return true;
        }
        else {
          return false;
        }
      }
      else { //Screw the dealer
        order_up_suit = get_same_color_suit(upcard.get_suit());
        return true;
      }
    }
  };

  void add_and_discard(const Card &upcard) {
    sort(hand.begin(), hand.end());
    int index = 0;

    if (Card_less(upcard, hand.at(0), upcard.get_suit())) {
      return;
    }
    else {
      Card new_card = upcard;
      //Replace the smallest card
      for (int i = 0; i < 5; i++) {
          //Find the index with the lowest card
          if (Card_less(hand.at(i), hand.at(index), upcard.get_suit())) {
            index = i;
        }
      }
      swap(new_card, hand.at(index));
      return;
    }
  };

  Card lead_card(const std::string &trump) {
    sort(hand.begin(), hand.end());
    Card leadCard;
    //Play the highest value card that is not trump
    for (int i = hand.size() - 1; i >= 0; i--) {
      if (hand.at(i).is_trump(trump)) {
        continue;
      }
      else {
        leadCard = hand.at(i);
        hand.erase(hand.begin() + i);
        return leadCard;
      }
    }

    //If they are all trump, return the highest value card in the hand
    for (int i = 0; i < hand.size(); i++) { //Play bower if present
      if (hand.at(i).is_right_bower(trump)) {
        leadCard = hand.at(i);
        hand.erase(hand.begin() + i);
        return leadCard;
      }
    }
    for (int i = 0; i < hand.size(); i++) { //Play bower if present
      if (hand.at(i).is_left_bower(trump)) {
        leadCard = hand.at(i);
        hand.erase(hand.begin() + i);
        return leadCard;
      }
    }

    leadCard = hand.at(hand.size() - 1);
    hand.erase(hand.end() - 1);
    return leadCard;
  };

  Card play_card(const Card &led_card, const std::string &trump) {
    sort(hand.begin(), hand.end());
    Card playCard;
    int index = 0;

    //Play the bowers first if present
    if (led_card.get_suit() == trump) {
      for (int i = 0; i < hand.size(); i++) {
        if (hand.at(i).is_right_bower(trump)) {
          playCard = hand.at(i);
          hand.erase(hand.begin() + i);
          return playCard;
        }
      }
      for (int i = 0; i < hand.size(); i++) {
        if (hand.at(i).is_left_bower(trump)) {
          playCard = hand.at(i);
          hand.erase(hand.begin() + i);
          return playCard;
        }
      }
    }

    //Play the highest card of the same suit
    for (int i = hand.size() - 1; i >= 0; i--) {
      if (hand.at(i).get_suit(trump) == led_card.get_suit(trump)) {
        playCard = hand.at(i);
        hand.erase(hand.begin() + i);
        return playCard;
      }
    }

    //If not, play the lowest card in the hand
    playCard = hand.at(0);
    for (int i = 0; i < hand.size(); i++) {
      if (Card_less(hand.at(i), playCard, led_card, trump)) {
        playCard = hand.at(i);
        index = i;
      }
    }
    hand.erase(hand.begin() + index);
    return playCard;
  };

  private:
    string name;
    vector<Card> hand;
};

Human::Human(string name_in) {
  name = name_in;
}

Simple::Simple(string name_in) {
  name = name_in;
}

Player * Player_factory(const string &name, const string &strategy) {
  if (strategy == "Human") {
    return new Human(name);
  }
  else {
    return new Simple(name);
  }

  assert(false);
  return nullptr;
}

ostream & operator<<(ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}

//Helper functions
static string get_same_color_suit(string current_suit) {
    if (current_suit == Card::SUIT_CLUBS) {
        return Card::SUIT_SPADES;
    }
    else if (current_suit == Card::SUIT_SPADES) {
        return Card::SUIT_CLUBS;
    }
    else if (current_suit == Card::SUIT_HEARTS) {
        return Card::SUIT_DIAMONDS;
    }
    else if (current_suit == Card::SUIT_DIAMONDS) {
        return Card::SUIT_HEARTS;
    }
    else {
        return "Error";
    }
}