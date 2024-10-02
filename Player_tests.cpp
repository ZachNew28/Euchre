// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    //Test bot
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;

    //Test Human
    /*Player *me = Player_factory("Me :)", "Human");
    ASSERT_EQUAL("Me :)", me->get_name());

    delete me;*/
}

TEST(test_player_factory) {
    /*Player *player = Player_factory("Geroge", "Human");
    ASSERT_EQUAL("George", player->get_name());
    
    delete player;*/

    Player *player2 = Player_factory("Geoffrey", "Simple");
    ASSERT_EQUAL("Geoffrey", player2->get_name());
    
    delete player2;
}

TEST(test_add_card) {
    Player *Gary = Player_factory("Gary", "Simple");
    Card c;
    Gary->add_card(c);

    ASSERT_EQUAL(Gary->lead_card(Card::SUIT_SPADES), c);

    Card c2(Card::RANK_ACE, Card::SUIT_SPADES);

    Gary->add_card(c);
    Gary->add_card(c2);

    ASSERT_EQUAL(Gary->lead_card(Card::SUIT_SPADES), c2);
    
    delete Gary;
}

TEST(test_make_trump) {
    Player *Gimlee = Player_factory("Gimlee", "Simple");
    Card upcard(Card::RANK_JACK, Card::SUIT_SPADES);
    Card c;
    Card c2(Card::RANK_ACE, Card::SUIT_SPADES);
    Card c3(Card::RANK_KING, Card::SUIT_HEARTS);
    Card c4(Card::RANK_QUEEN, Card::SUIT_SPADES);
    Card c5(Card::RANK_THREE, Card::SUIT_DIAMONDS);
    string upsuit = Card::SUIT_HEARTS;

    Gimlee->add_card(c);
    Gimlee->add_card(c2);
    Gimlee->add_card(c3);
    Gimlee->add_card(c4);
    Gimlee->add_card(c5);

    ASSERT_TRUE(Gimlee->make_trump(upcard, false, 1, upsuit));
    ASSERT_EQUAL(upcard.get_suit(), upsuit);
    upsuit = Card::SUIT_HEARTS;
    ASSERT_TRUE(Gimlee->make_trump(upcard, true, 1, upsuit));
    ASSERT_EQUAL(upcard.get_suit(), upsuit);
    upsuit = Card::SUIT_CLUBS;
    ASSERT_TRUE(Gimlee->make_trump(upcard, true, 2, upsuit));
    ASSERT_FALSE(Gimlee->make_trump(upcard, false, 2, upsuit));

    delete Gimlee;

    /*Player *Gandalf = Player_factory("Gandalf", "Human");

    ASSERT_TRUE(Gandalf->make_trump(c, false, 1, upsuit));
    ASSERT_TRUE(Gandalf->make_trump(c, false, 2, upsuit));
    ASSERT_TRUE(Gandalf->make_trump(c, true, 2, upsuit));
    upsuit = Card::SUIT_SPADES;
    ASSERT_FALSE(Gandalf->make_trump(c, false, 2, upsuit));

    delete Gandalf;*/
}

TEST(test_add_and_discard) {
    Player *Genley = Player_factory("Genley", "Simple");
    Card upcard;
    Card card_fill;
    Card card_fill2;
    Card card_fill3;
    Card card_fill4;
    Card c(Card::RANK_ACE, Card::SUIT_CLUBS);
    Genley->add_card(c);
    Genley->add_card(card_fill);
    Genley->add_card(card_fill2);
    Genley->add_card(card_fill3);
    Genley->add_card(card_fill4);

    Genley->add_and_discard(upcard); //Replaces c because upcard suit(spades) is now trump

    ASSERT_EQUAL(Genley->lead_card("Spades"), upcard);

    delete Genley;

    Player *Ganondorf = Player_factory("Ganondorf", "Simple");
    Card hearts(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card diamonds(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    Card upcard2(Card::RANK_ACE, Card::SUIT_HEARTS);
    Ganondorf->add_card(c);
    Ganondorf->add_card(card_fill);
    Ganondorf->add_card(card_fill2);
    Ganondorf->add_card(hearts);
    Ganondorf->add_card(diamonds);

    Ganondorf->add_and_discard(upcard2);

    ASSERT_EQUAL(Ganondorf->lead_card(Card::SUIT_HEARTS), diamonds);

    delete Ganondorf;
}

TEST(test_lead_card) {
    Player *Greggory = Player_factory("Greggory", "Simple");
    Card c;
    Card c2(Card::RANK_ACE, Card::SUIT_SPADES);
    Card c3(Card::RANK_KING, Card::SUIT_HEARTS);
    Greggory->add_card(c);
    Greggory->add_card(c2);
    Greggory->add_card(c3);

    ASSERT_EQUAL(Greggory->lead_card("Spades"), c3);
    ASSERT_EQUAL(Greggory->lead_card("Hearts"), c2);
    Greggory->lead_card("Spades");

    Card c4(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card c5(Card::RANK_ACE, Card::SUIT_HEARTS);
    Greggory->add_card(c4);
    Greggory->add_card(c5);

    ASSERT_EQUAL(Greggory->lead_card("Diamonds"), c5);
    Greggory->add_card(c5);
    ASSERT_EQUAL(Greggory->lead_card("Hearts"), c4);
    
    delete Greggory;
}

TEST(test_play_card) {
    Player *Gilgamesh = Player_factory("Gilgamesh", "Simple");
    Card c;
    Card c2(Card::RANK_ACE, Card::SUIT_SPADES);
    Card c3(Card::RANK_KING, Card::SUIT_HEARTS);
    Card c4(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    Gilgamesh->add_card(c);
    Gilgamesh->add_card(c2);
    Gilgamesh->add_card(c3);

    ASSERT_EQUAL(Gilgamesh->play_card(c, "Spades"), c2);
    Gilgamesh->add_card(c2);

    ASSERT_EQUAL(Gilgamesh->play_card(c4, "Diamonds"), c);

    Card c5(Card::RANK_JACK, Card::SUIT_HEARTS);
    Card c6(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    Card upcard(Card::RANK_ACE, Card::SUIT_CLUBS);
    Gilgamesh->add_card(c5);
    Gilgamesh->add_card(c6);

    ASSERT_EQUAL(Gilgamesh->play_card(c4, "Clubs"), c6);
    Gilgamesh->add_card(c5);
    ASSERT_EQUAL(Gilgamesh->play_card(c3, "Hearts"), c5);
    Gilgamesh->add_card(c5);
    ASSERT_EQUAL(Gilgamesh->play_card(c6, "Diamonds"), c5);

    delete Gilgamesh;
}

TEST_MAIN()
