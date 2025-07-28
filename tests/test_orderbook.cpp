#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "orderbook.h"

using namespace OrderBook;

TEST_CASE("Basic OrderBook functionality", "[orderbook]") {
    OrderBook ob("TEST");

    uint64_t ts = 123456789;

    // Add a limit buy order
    Order buy_order{1, "TEST", Side::BUY, OrderType::LIMIT, 100.0, 500, ts};
    ob.add_order(buy_order);

    // Add a limit sell order that matches partially
    Order sell_order{2, "TEST", Side::SELL, OrderType::LIMIT, 99.5, 300, ts};
    ob.add_order(sell_order);

    auto bids = ob.get_bids();
    auto asks = ob.get_asks();
    auto trades = ob.get_trades();

    REQUIRE(bids.size() == 1);
    REQUIRE(asks.size() == 1);
    REQUIRE(trades.size() == 1);

    REQUIRE(trades[0].quantity == 300);
    REQUIRE(trades[0].price == 100.0);

    // After partial fill, buy order should have 200 quantity left
    auto order_ptr = ob.get_order(1);
    REQUIRE(order_ptr != nullptr);
    REQUIRE(order_ptr->filled_quantity == 300);
    REQUIRE(order_ptr->quantity == 500);
    REQUIRE(order_ptr->is_filled() == false);

    // Cancel the remaining buy order
    ob.cancel_order(1);
    REQUIRE(ob.get_order(1) == nullptr);
}
