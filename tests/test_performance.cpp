#include <chrono>
#include <iostream>
#include "orderbook.h"

using namespace OrderBook;

int main() {
    OrderBook ob("PERF");
    constexpr int NUM_ORDERS = 100000;
    uint64_t ts = 123456789;

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Add a bunch of limit buy orders (bids)
    for (int i = 0; i < NUM_ORDERS; ++i) {
        Order buy_order{
            static_cast<uint64_t>(i),
            "PERF",
            Side::BUY,
            OrderType::LIMIT,
            100.0 + (i % 100) * 0.01,  // prices from 100.0 to ~101.0
            10,
            ts + i
        };
        ob.add_order(buy_order);
    }

    // Add a bunch of limit sell orders (asks)
    for (int i = 0; i < NUM_ORDERS; ++i) {
        Order sell_order{
            static_cast<uint64_t>(NUM_ORDERS + i),
            "PERF",
            Side::SELL,
            OrderType::LIMIT,
            100.5 + (i % 100) * 0.01, // prices from 100.5 to ~101.5
            10,
            ts + i
        };
        ob.add_order(sell_order);
    }

    // Add some market orders to trigger matching
    for (int i = 0; i < 1000; ++i) {
        Order market_order{
            static_cast<uint64_t>(2 * NUM_ORDERS + i),
            "PERF",
            Side::BUY,
            OrderType::MARKET,
            0.0,
            100,
            ts + i
        };
        ob.add_order(market_order);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Processed " << (2 * NUM_ORDERS + 1000) << " orders in "
              << duration_ms << " ms." << std::endl;

    // Optionally: print number of trades executed
    std::cout << "Number of trades executed: " << ob.get_trades().size() << std::endl;

    return 0;
}
