#ifndef ORDERBOOK_TYPES_H
#define ORDERBOOK_TYPES_H

#include <cstdint>
#include <string>
#include <vector>

namespace OrderBook {

enum class OrderType { MARKET, LIMIT };
enum class Side { BUY, SELL };

struct Order {
    uint64_t order_id;
    std::string symbol;
    Side side;
    OrderType type;
    double price;            // Ignored for MARKET orders
    uint64_t quantity;
    uint64_t timestamp;

    // Filled quantity for partial fills
    uint64_t filled_quantity = 0;

    bool is_filled() const {
        return filled_quantity >= quantity;
    }
};

struct Trade {
    uint64_t trade_id;
    uint64_t buyer_order_id;
    uint64_t seller_order_id;
    std::string symbol;
    double price;
    uint64_t quantity;
    uint64_t timestamp;
};

using Price = double;
using Quantity = uint64_t;
using PriceLevel = std::pair<Price, Quantity>; // For get_bids()/get_asks()

} // namespace OrderBook

#endif // ORDERBOOK_TYPES_H
