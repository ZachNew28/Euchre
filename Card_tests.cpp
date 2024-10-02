// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    //Test complex constructor
    Card c(Card::RANK_ACE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c.get_suit());

    //Test basic constructor
    Card c2;
    ASSERT_EQUAL(Card::RANK_TWO, c2.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, c2.get_suit());
}

TEST(test_get_rank) {
    Card c(Card::RANK_ACE, Card::SUIT_CLUBS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());

    Card c2(Card::RANK_TWO, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(Card::RANK_TWO, c2.get_rank());
}

TEST(test_get_suit_basic) {
    Card c(Card::RANK_ACE, Card::SUIT_CLUBS);
    ASSERT_EQUAL(Card::SUIT_CLUBS, c.get_suit());

    Card c2(Card::RANK_TWO, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(Card::SUIT_DIAMONDS, c2.get_suit());
}

TEST(test_get_suit_complicated) {
    //Test all suits
    string trump = Card::SUIT_SPADES;

    Card c(Card::RANK_JACK, Card::SUIT_CLUBS);
    ASSERT_EQUAL(Card::SUIT_SPADES, c.get_suit(trump))

    trump = Card::SUIT_CLUBS;

    Card c2(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_EQUAL(Card::SUIT_CLUBS, c2.get_suit(trump))

    trump = Card::SUIT_DIAMONDS;
    
    Card c3(Card::RANK_JACK, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::SUIT_DIAMONDS, c3.get_suit(trump))

    trump = Card::SUIT_HEARTS;

    Card c4(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(Card::SUIT_HEARTS, c4.get_suit(trump))

    //Test when not bower
    Card c5(Card::RANK_ACE, Card::SUIT_CLUBS);
    ASSERT_EQUAL(Card::SUIT_CLUBS, c5.get_suit(trump))
}

TEST(test_is_left_bower) {
    //test all suits
    string trump = Card::SUIT_CLUBS;

    Card c(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_TRUE(c.is_left_bower(trump));

    trump = Card::SUIT_SPADES;

    Card c2(Card::RANK_JACK, Card::SUIT_CLUBS);
    ASSERT_TRUE(c2.is_left_bower(trump));

    trump = Card::SUIT_DIAMONDS;

    Card c3(Card::RANK_JACK, Card::SUIT_HEARTS);
    ASSERT_TRUE(c3.is_left_bower(trump));
    
    trump = Card::SUIT_HEARTS;

    Card c4(Card::RANK_JACK, Card::SUIT_DIAMONDS);
    ASSERT_TRUE(c4.is_left_bower(trump));

    //Test non bower
    Card c5(Card::RANK_KING, Card::SUIT_DIAMONDS); //Not jack rank
    ASSERT_FALSE(c5.is_left_bower(trump));

    Card c6(Card::RANK_JACK, Card::SUIT_SPADES); //Not bower suit
    ASSERT_FALSE(c6.is_left_bower(trump));
}

TEST(test_is_trump) {
    string trump = Card::SUIT_CLUBS;

    Card c(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_TRUE(c.is_trump(trump));

    Card c2(Card::RANK_JACK, Card::SUIT_CLUBS);
    ASSERT_TRUE(c2.is_trump(trump));

    Card c3(Card::RANK_ACE, Card::SUIT_CLUBS);
    ASSERT_TRUE(c3.is_trump(trump));

    Card c4(Card::RANK_ACE, Card::SUIT_SPADES);
    ASSERT_FALSE(c4.is_trump(trump));
}

TEST(test_is_face_or_ace) {
    Card c(Card::RANK_JACK, Card::SUIT_SPADES);
    ASSERT_TRUE(c.is_face_or_ace());

    Card c2(Card::RANK_QUEEN, Card::SUIT_CLUBS);
    ASSERT_TRUE(c2.is_face_or_ace());

    Card c3(Card::RANK_KING, Card::SUIT_CLUBS);
    ASSERT_TRUE(c3.is_face_or_ace());

    Card c4(Card::RANK_ACE, Card::SUIT_SPADES);
    ASSERT_TRUE(c4.is_face_or_ace());

    Card c5(Card::RANK_TEN, Card::SUIT_SPADES);
    ASSERT_FALSE(c5.is_face_or_ace());
}

TEST(test_is_right_bower) {
    string trump = Card::SUIT_CLUBS;

    Card c(Card::RANK_JACK, Card::SUIT_CLUBS);
    ASSERT_TRUE(c.is_right_bower(trump));

    Card c2(Card::RANK_JACK, Card::SUIT_SPADES); //diff suit
    ASSERT_FALSE(c2.is_right_bower(trump));

    Card c3(Card::RANK_KING, Card::SUIT_CLUBS); //diff rank
    ASSERT_FALSE(c3.is_right_bower(trump));
}

TEST(test_ostream_operator) {
    //<< operator
    Card c(Card::RANK_JACK, Card::SUIT_CLUBS);
    cout << c << endl;
}
TEST(test_less_than_operator) {
    //< operator
    Card c(Card::RANK_JACK, Card::SUIT_CLUBS);
    
    Card c2(Card::RANK_KING, Card::SUIT_DIAMONDS); //diff rank and suit
    ASSERT_TRUE(c < c2);

    Card c3(Card::RANK_ACE, Card::SUIT_DIAMONDS); //diff rank
    ASSERT_TRUE(c2 < c3);

    Card c4(Card::RANK_KING, Card::SUIT_CLUBS); //diff suit
    ASSERT_TRUE(c4 < c2);
}
TEST(test_equals_operator) {
    //== operator
    Card c6;
    Card c5;
    ASSERT_TRUE(c6 == c5);
}
TEST(test_other_operators) {
    Card c(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card c2(Card::RANK_KING, Card::SUIT_DIAMONDS);
    Card c3(Card::RANK_ACE, Card::SUIT_DIAMONDS);
    Card c4(Card::RANK_KING, Card::SUIT_CLUBS);
    Card c6;
    Card c5;

    //<= operator
    ASSERT_TRUE(c <= c2); //c < c2
    ASSERT_TRUE(c5 <= c6); //c5 == c6

    //>operator
    ASSERT_TRUE(c2 > c);
    ASSERT_TRUE(c3 > c2);
    ASSERT_TRUE(c2 > c4);

    //>= operator
    ASSERT_TRUE(c3 >= c2); //c3 > c2
    ASSERT_TRUE(c5 >= c6); //c5 == c6

    //!= operator
    ASSERT_TRUE(c3 != c2);
}

TEST(test_suit_next) {
    ASSERT_TRUE(Suit_next(Card::SUIT_CLUBS) == Card::SUIT_SPADES);
    ASSERT_TRUE(Suit_next(Card::SUIT_SPADES) == Card::SUIT_CLUBS);
    ASSERT_TRUE(Suit_next(Card::SUIT_HEARTS) == Card::SUIT_DIAMONDS);
    ASSERT_TRUE(Suit_next(Card::SUIT_DIAMONDS) == Card::SUIT_HEARTS);
}

TEST(test_card_less) {
    //without led suit consideration
    Card c(Card::RANK_JACK, Card::SUIT_CLUBS);
    Card c2(Card::RANK_KING, Card::SUIT_DIAMONDS);
    string trump = Card::SUIT_CLUBS;

    ASSERT_TRUE(Card_less(c2, c, trump));

    trump = Card::SUIT_SPADES;
    ASSERT_TRUE(Card_less(c2, c, trump));

    //with led suit consideration
    Card c3(Card::RANK_ACE, Card::SUIT_HEARTS);

    ASSERT_TRUE(Card_less(c3, c, c3, trump));
    ASSERT_TRUE(Card_less(c2, c3, c3, trump));

    trump = Card::SUIT_HEARTS;
    ASSERT_TRUE(Card_less(c, c3, c3, trump));
}

TEST_MAIN()
