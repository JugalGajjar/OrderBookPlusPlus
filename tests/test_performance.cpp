#include <gtest/gtest.h>
#include "orderbook.h"
#include <chrono>

using namespace OrderBook;

TEST(PerformanceTest, AddAndMatch10000Orders) {
    OrderBook::OrderBookEngine book("PERF");

    constexpr int num_orders = 10000;

    // Add 10,000 limit sell orders
    for (int i = 0; i < num_orders; ++i) {
        Order order{
            static_cast<uint64_t>(i), 
            "PERF", 
            Side::SELL, 
            OrderType::LIMIT, 
            100.0 + (i % 5), 
            100, 
            static_cast<uint64_t>(i)
        };
        book.add_order(order);
    }

    // Time how long it takes to match 10,000 market buy orders
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_orders; ++i) {
        Order order{
            static_cast<uint64_t>(100000 + i), 
            "PERF", 
            Side::BUY, 
            OrderType::MARKET, 
            0.0, 
            100, 
            static_cast<uint64_t>(i)
        };
        book.add_order(order);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Matched 10,000 market orders in " << diff.count() << " seconds.\n";

    EXPECT_EQ(book.get_trades().size(), num_orders);
}
