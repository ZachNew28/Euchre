#include <iostream>
#include <fstream>
#include <string>
#include "Player.h"
#include "Pack.h"
#include "Card.h"
#include "unit_test_framework.h"
#include "euchre.cpp"
using namespace std;

TEST(test_player_ptrs) {
    Player* Kelly = Player_factory("Kelly", "Simple");
    Player* Key = Player_factory("Key", "Simple");
    Player* Kyle = Player_factory("Kyle", "Simple");
    Player* Kilmer = Player_factory("Kilmer", "Simple");
    vector<Player*> players_in;
    players_in.push_back(Kelly);
    players_in.push_back(Key);
    players_in.push_back(Kyle);
    players_in.push_back(Kilmer);
    ifstream pack_file;
    pack_file.open("pack.in");

    Game game(players_in, 100, "noshuffle", pack_file);
    game.play();

}



