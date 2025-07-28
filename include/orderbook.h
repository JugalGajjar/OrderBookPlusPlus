#ifndef ORDERBOOK_ORDERBOOK_H
#define ORDERBOOK_ORDERBOOK_H

#include "types.h"
#include <map>
#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace OrderBook {

class OrderBook {
public:
    explicit OrderBook(std::string symbol);

    // Core API
    void add_order(const Order& order);
    void cancel_order(uint64_t order_id);

    std::vector<PriceLevel> get_bids() const;
    std::vector<PriceLevel> get_asks() const;
    std::vector<Trade> get_trades() const;

    std::shared_ptr<Order> get_order(uint64_t order_id) const;

private:
    struct OrderNode {
        Order order;
        // Doubly linked list pointers inside a price level
        OrderNode* prev = nullptr;
        OrderNode* next = nullptr;

        explicit OrderNode(const Order& ord) : order(ord) {}
    };

    struct PriceLevelNodeList {
        uint64_t total_quantity = 0;
        OrderNode* head = nullptr;
        OrderNode* tail = nullptr;

        void append(OrderNode* node);
        void remove(OrderNode* node);
        bool empty() const { return head == nullptr; }
    };

    using OrderList = std::list<OrderNode*>;

    std::string symbol_;

    // Bids sorted descending (highest price first)
    std::map<Price, PriceLevelNodeList, std::greater<Price>> bids_;

    // Asks sorted ascending (lowest price first)
    std::map<Price, PriceLevelNodeList> asks_;

    // Map order_id to OrderNode for quick lookup
    std::unordered_map<uint64_t, OrderNode*> order_map_;

    // Executed trades
    std::vector<Trade> trades_;
    uint64_t next_trade_id_ = 1;

    // Internal helpers
    void match_order(OrderNode* order_node);
    void add_to_book(OrderNode* order_node);
    void execute_trade(OrderNode* taker, OrderNode* maker, Price price, Quantity qty);
};

} // namespace OrderBook

#endif // ORDERBOOK_ORDERBOOK_H
