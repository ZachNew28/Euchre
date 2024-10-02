// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Pack.h"
#include "unit_test_framework.h"
#include <fstream>
#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(Card::RANK_NINE, first.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, first.get_suit());
}

TEST(test_pack_input_ctor) {
    ifstream file;
    file.open("pack.in");
    assert(file.is_open());
    Pack pack;

    Pack pack2(file);

    Card first = pack2.deal_one();
    ASSERT_EQUAL(first, Card(Card::RANK_NINE, Card::SUIT_SPADES));
    file.close();
}

TEST(test_deal_one) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(Card::RANK_NINE, first.get_rank());
    ASSERT_EQUAL(Card::SUIT_SPADES, first.get_suit());
}

TEST(test_reset) {
    Pack pack;
    Card first = pack.deal_one();
    Card second = pack.deal_one();
    pack.reset();
    Card third = pack.deal_one();

    ASSERT_EQUAL(first, third);

    Card fourth = pack.deal_one();
    ASSERT_EQUAL(second, fourth);
}

TEST(test_shuffle) {
    //test one shuffle
    Pack pack;
    Pack pack_compare;
    Pack pack2;
    Card first = pack.deal_one();
    pack.reset();
    pack.shuffle();
    Card second = pack.deal_one();
    pack.reset();

    ASSERT_NOT_EQUAL(first, second);

    pack2.shuffle(); //Shuffle 20 times to make sure it is implemented right
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    pack2.shuffle();
    Card third = pack2.deal_one();
    Card fourth = pack_compare.deal_one();

    ASSERT_EQUAL(third, fourth);
}

TEST(test_empty) {
    Pack pack;

    assert(!pack.empty());

    Card first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    first = pack.deal_one();
    assert(pack.empty());
}

TEST_MAIN()
