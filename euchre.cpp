// Project UID 1d9f47bfc76643019cfbf037641defe1
#include <iostream>
#include <fstream>
#include <string>
#include "Player.h"
#include "Pack.h"
#include "Card.h"
using namespace std;

static string get_same_color_suit(string current_suit);
static Card find_trick_winner(vector<Card> cards, Card led_card, string trump);

class Game {
 public:
  Game(vector<Player*> players_in, int pts, string shuffle_in, ifstream &pack_file);
  void play();

 private:
  vector<Player*> players;
  Pack pack;
  int pts2win;
  bool shuffleBool;
  int t1_pts;
  int t2_pts;
  string game_trump_suit;
  bool team1ChangeTrump;
  
  void shuffle();
  void deal(int dealer_index);
  void make_trump(int round, int dealer_index, Card upcard, bool &trumpChange);
  void play_hand(int dealer_index, string trump_suit);
  vector<Player*> player_ptrs(int dealer_index);
  int get_dealer_index(Player* trick_winner);
  bool teamone_or_teamtwo(Player* trick_winner);
};

int main(int argc, char *argv[]) {
    //Check for errors in command line
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }
    else if (atoi(argv[3]) < 1 || atoi(argv[3]) > 100) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }
    else if (static_cast<string>(argv[2]) != "shuffle" 
            && static_cast<string>(argv[2]) != "noshuffle") {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }
    else if ((static_cast<string>(argv[5]) != "Human" 
            && static_cast<string>(argv[5]) != "Simple") || 
            (static_cast<string>(argv[7]) != "Human" 
            && static_cast<string>(argv[7]) != "Simple") || 
            (static_cast<string>(argv[9]) != "Human" 
            && static_cast<string>(argv[9]) != "Simple") || 
            (static_cast<string>(argv[11]) != "Human" 
            && static_cast<string>(argv[11]) != "Simple")) {
                cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                    << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                    << "NAME4 TYPE4" << endl;
                return 1;
    }
    
    //Make sure file can open properly
    string filename = argv[1];
    ifstream pack_file;
    pack_file.open(filename);
    if (!pack_file.is_open()) {
        cout << "Error opening " << filename << endl;
        return 1;
    }

    //Create the players
    vector<Player*> players_in;
    for (int i = 4; i < 12; i += 2) {
        players_in.push_back(Player_factory(argv[i], argv[i + 1]));
    }

    //Create other variable from command line
    int pts = atoi(argv[3]);
    string shuffle_in = argv[2];

    //Print command line arguments
    for (int i = 0; i < argc; i++) {
        string command;
        command = static_cast<string>(argv[i]);

        cout << command << " ";
    }
    cout << endl;

    //Create the game and play it
    Game Game(players_in, pts, shuffle_in, pack_file);
    pack_file.close();
    Game.play();
}

Game::Game(vector<Player*> players_in, int pts, string shuffle_in, ifstream &pack_file) {
    players = players_in;
    Pack current_pack(pack_file);
    pack = current_pack;
    pts2win = pts;
    team1ChangeTrump = false;
    t1_pts = 0;
    t2_pts = 0;

    if (shuffle_in == "shuffle") {
        shuffleBool = true;
    }
    else {
        shuffleBool = false;
    }
}

void Game::play() {
    int hand = 0;
    int dealer_index = 0;
    int round;
    Card upcard;
    bool trumpChange;
    //Iterate until a team gets enough points to win
    while (t1_pts < pts2win && t2_pts < pts2win) {
        //Reset neccessary variables
        trumpChange = false;
        team1ChangeTrump = false;
        if (dealer_index == 4) {
            dealer_index = 0;
        }
        round = 1;

        //Setup table
        if (shuffleBool) {
            shuffle();
        }
        else {
            pack.reset();
        }
        
        cout << "Hand " << hand << endl;
        cout << *players.at(dealer_index) << " deals" << endl;
        deal(dealer_index);
        upcard = pack.deal_one();
        cout << upcard << " turned up" << endl;

        //Making
        make_trump(round, dealer_index, upcard, trumpChange);
        //Proceed to round two if necessary 
        if (!trumpChange) {
            round++;
            make_trump(round, dealer_index, upcard, trumpChange);
        }
        cout << endl;

        //Tricks
        play_hand(dealer_index, game_trump_suit);
        cout << *players.at(0) << " and " << *players.at(2) << " have " << t1_pts 
            << " points" << endl;
        cout << *players.at(1) << " and " << *players.at(3) << " have " << t2_pts 
            << " points" << endl;
        cout << endl;

        hand++;
        dealer_index++;
    }

    if (t1_pts > t2_pts) {
        cout << *players.at(0) << " and " << *players.at(2) << " win!" << endl;
    }
    else {
        cout << *players.at(1) << " and " << *players.at(3) << " win!" << endl;
    }

    //Delete the players
    for (int i = 0; i < players.size(); i++) {
        delete players.at(i);
    }
}

void Game::play_hand(int dealer_index, string trump_suit) {
    int trickNum;
    int team1 = 0;
    int team2 = 0;
    
    Card card1;
    Card card2;
    Card card3;
    Card card4;
    Card winner;
    vector<Card> cards;

    for (trickNum = 1; trickNum <= 5; trickNum++) {
        vector<Player*> player_vec = player_ptrs(dealer_index);

        //Players play their cards
        card1 = player_vec.at(1)->lead_card(trump_suit);
        cout << card1 << " led by " << *player_vec.at(1) << endl;

        card2 = player_vec.at(2)->play_card(card1, trump_suit);
        cout << card2 << " played by " << *player_vec.at(2) << endl;

        card3 = player_vec.at(3)->play_card(card1, trump_suit);
        cout << card3 << " played by " << *player_vec.at(3) << endl;

        card4 = player_vec.at(0)->play_card(card1, trump_suit);
        cout << card4 << " played by " << *player_vec.at(0) << endl;

        //Find winning card
        vector<Card> cards = {card4, card1, card2, card3};
        winner = find_trick_winner(cards, card1, trump_suit);

        //Determine who won the trick and which team gets the "points" for it
        for (int i = 0; i < cards.size(); i++) {
            if (winner == cards.at(i)) {
                cout << *player_vec.at(i) << " takes the trick" << endl;
                dealer_index = get_dealer_index(player_vec.at(i));
                if (teamone_or_teamtwo(player_vec.at(i))) {team1++;}
                else {team2++;}
            }
        }
        cout << endl;
    }

    //Print hand winners
    if (team1 > team2) {
        cout << *players.at(0) << " and " << *players.at(2) << " win the hand" << endl;
        if (team1ChangeTrump) {
            t1_pts++;
            if (team1 == 5) {
                t1_pts++;
                cout << "march!" << endl;
            }
        }
        else {
            t1_pts += 2;
            cout << "euchred!" << endl;
        }
    }
    else {
        cout << *players.at(1) << " and " << *players.at(3) << " win the hand" << endl;
        if (!team1ChangeTrump) {
            t2_pts++;
            if (team2 == 5) {
                t2_pts++;
                cout << "march!" << endl;
            }
        }
        else {
            t2_pts += 2;
            cout << "euchred!" << endl;    
        }
    }
}

void Game::shuffle() {
    pack.reset();
    pack.shuffle();
}

void Game::make_trump(int round, int dealer_index, Card upcard, bool &trumpChange) {
    string upsuit = "null";
    vector<Player*> player_vec = player_ptrs(dealer_index);

    for (int i = 1; i <= player_vec.size(); i++) {
        if (i == player_vec.size()) {
            if (players.at(dealer_index)->make_trump(upcard, true, round, upsuit)) {
                trumpChange = true;
                if (round == 1) {
                    cout << *players.at(dealer_index) << " orders up " 
                    << upcard.get_suit() << endl;
                    players.at(dealer_index)->add_and_discard(upcard);
                }
                game_trump_suit = upcard.get_suit();

                if (player_vec.at(0) == players.at(0) || 
                    player_vec.at(0) == players.at(2)) {
                    team1ChangeTrump = true;
                }
                return;
            }
            else {
                cout << *players.at(dealer_index) << " passes" << endl;
                return;
            }
        }

        if (player_vec.at(i)->make_trump(upcard, false, round, upsuit)) {
            trumpChange = true;
            //Give dealer option to pick up upcard
            if (round == 1) {
                cout << *player_vec.at(i) << " orders up " << upcard.get_suit()
                    << endl; //print decision
                game_trump_suit = upcard.get_suit();
                players.at(dealer_index)->add_and_discard(upcard);
            }
            else {
                cout << *player_vec.at(i) << " orders up "
                    << get_same_color_suit(upcard.get_suit()) << endl;
                game_trump_suit = get_same_color_suit(upcard.get_suit());
            }

            //Take note of which team ordered up (If they are team1, take note of that)
            if (player_vec.at(i) == players.at(0) || player_vec.at(i) == players.at(2)) {
                team1ChangeTrump = true;
            }
            return;
        }
        else {
            cout << *player_vec.at(i) << " passes" << endl;
        }
     }
}

void Game::deal(int dealer_index) {
    vector<Player*> player_vec = player_ptrs(dealer_index);

    player_vec.at(1)->add_card(pack.deal_one());
    player_vec.at(1)->add_card(pack.deal_one());
    player_vec.at(1)->add_card(pack.deal_one());

    player_vec.at(2)->add_card(pack.deal_one());
    player_vec.at(2)->add_card(pack.deal_one());

    player_vec.at(3)->add_card(pack.deal_one());
    player_vec.at(3)->add_card(pack.deal_one());
    player_vec.at(3)->add_card(pack.deal_one());

    player_vec.at(0)->add_card(pack.deal_one());
    player_vec.at(0)->add_card(pack.deal_one());

    player_vec.at(1)->add_card(pack.deal_one());
    player_vec.at(1)->add_card(pack.deal_one());

    player_vec.at(2)->add_card(pack.deal_one());
    player_vec.at(2)->add_card(pack.deal_one());
    player_vec.at(2)->add_card(pack.deal_one());

    player_vec.at(3)->add_card(pack.deal_one());
    player_vec.at(3)->add_card(pack.deal_one());

    player_vec.at(0)->add_card(pack.deal_one());
    player_vec.at(0)->add_card(pack.deal_one());
    player_vec.at(0)->add_card(pack.deal_one());
    
}

//Returns the order of the players with the dealer at index 0
vector<Player*> Game::player_ptrs(int dealer_index) {
    vector<Player*> ptr_vec(4);
    if (dealer_index == 0) {
        for (int i = 0; i < 4; i++) {
            ptr_vec.at(i) = players.at(i);
        }
    }
    else if (dealer_index == 1) {
        ptr_vec.at(0) = players.at(1);
        ptr_vec.at(1) = players.at(2);
        ptr_vec.at(2) = players.at(3);
        ptr_vec.at(3) = players.at(0);
    }
    else if (dealer_index == 2) {
        ptr_vec.at(0) = players.at(2);
        ptr_vec.at(1) = players.at(3);
        ptr_vec.at(2) = players.at(0);
        ptr_vec.at(3) = players.at(1);
    }
    else {
        ptr_vec.at(0) = players.at(3);
        ptr_vec.at(1) = players.at(0);
        ptr_vec.at(2) = players.at(1);
        ptr_vec.at(3) = players.at(2);
    }

    return ptr_vec;
}

//Returns the index of the player to the right of the trick_winner, on the players vector
int Game::get_dealer_index(Player* trick_winner) {
    if (trick_winner == players.at(0)) {
        return 3;
    }

    for (int i = 0; i < 4; i++) {
        if (trick_winner == players.at(i)) {
            return i - 1;
        }
    }

    cout << "Error getting dealer index" << endl;
    return 0;
}

//Returns true for team one, false for team two or error
bool Game::teamone_or_teamtwo(Player* trick_winner) {
    if (trick_winner == players.at(0)) {
        return true;
    }
    else if (trick_winner == players.at(1)) {
        return false;
    }
    else if (trick_winner == players.at(2)) {
        return true;
    }
    else if (trick_winner == players.at(3)) {
        return false;
    }
    else {
        cout << "Error computing team winner" << endl;
        return false;
    }
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

static Card find_trick_winner(vector<Card> cards, Card led_card, string trump) {
    Card max = cards.at(0);

    for (int i = 0; i < 4; i++) {
        if (Card_less(max, cards.at(i), led_card, trump)) {
            max = cards.at(i);
        }
    }

    return max;
}