#include <gtest/gtest.h>
#include "orderbook.h"

using namespace OrderBook;

class OrderBookTest : public ::testing::Test {
protected:
    OrderBook::OrderBookEngine book{"AAPL"};
};

TEST_F(OrderBookTest, AddLimitBuyOrder) {
    Order order{1, "AAPL", Side::BUY, OrderType::LIMIT, 100.0, 10, 1};
    book.add_order(order);

    auto bids = book.get_bids();
    ASSERT_EQ(bids.size(), 1);
    EXPECT_EQ(bids[0].first, 100.0);
    EXPECT_EQ(bids[0].second, 10);
}

TEST_F(OrderBookTest, AddLimitSellOrder) {
    Order order{2, "AAPL", Side::SELL, OrderType::LIMIT, 101.0, 5, 2};
    book.add_order(order);

    auto asks = book.get_asks();
    ASSERT_EQ(asks.size(), 1);
    EXPECT_EQ(asks[0].first, 101.0);
    EXPECT_EQ(asks[0].second, 5);
}

TEST_F(OrderBookTest, MatchLimitOrders) {
    Order buy{1, "AAPL", Side::BUY, OrderType::LIMIT, 100.0, 10, 1};
    Order sell{2, "AAPL", Side::SELL, OrderType::LIMIT, 100.0, 10, 2};

    book.add_order(buy);
    book.add_order(sell);

    auto trades = book.get_trades();
    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].price, 100.0);
    EXPECT_EQ(trades[0].quantity, 10);

    EXPECT_TRUE(book.get_bids().empty());
    EXPECT_TRUE(book.get_asks().empty());
}

TEST_F(OrderBookTest, PartialFillLimitOrders) {
    Order buy{1, "AAPL", Side::BUY, OrderType::LIMIT, 100.0, 5, 1};
    Order sell{2, "AAPL", Side::SELL, OrderType::LIMIT, 100.0, 10, 2};

    book.add_order(buy);
    book.add_order(sell);

    auto trades = book.get_trades();
    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 5);

    auto asks = book.get_asks();
    ASSERT_EQ(asks.size(), 1);
    EXPECT_EQ(asks[0].second, 5);  // Remaining ask
}

TEST_F(OrderBookTest, MarketOrderMatchesBestPrice) {
    book.add_order(Order{1, "AAPL", Side::SELL, OrderType::LIMIT, 101.0, 10, 1});
    book.add_order(Order{2, "AAPL", Side::SELL, OrderType::LIMIT, 100.0, 10, 2});
    book.add_order(Order{3, "AAPL", Side::BUY, OrderType::MARKET, 0.0, 10, 3});

    auto trades = book.get_trades();
    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].price, 100.0);
    EXPECT_EQ(trades[0].quantity, 10);
}

TEST_F(OrderBookTest, CancelOrderRemovesFromBook) {
    Order order{1, "AAPL", Side::BUY, OrderType::LIMIT, 99.0, 20, 1};
    book.add_order(order);
    book.cancel_order(1);

    auto bids = book.get_bids();
    EXPECT_TRUE(bids.empty());
}

TEST_F(OrderBookTest, GetOrderReturnsCorrectData) {
    Order order{123, "AAPL", Side::SELL, OrderType::LIMIT, 105.0, 50, 1234};
    book.add_order(order);

    auto result = book.get_order(123);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->price, 105.0);
    EXPECT_EQ(result->quantity, 50);
}
