#include "orderbook.h"
#include <algorithm>  // std::min, std::max

namespace OrderBook {

// PriceLevelNodeList methods

void OrderBook::PriceLevelNodeList::append(OrderNode* node) {
    node->prev = tail;
    node->next = nullptr;
    if (tail) {
        tail->next = node;
    }
    tail = node;
    if (!head) head = node;
    total_quantity += (node->order.quantity - node->order.filled_quantity);
}

void OrderBook::PriceLevelNodeList::remove(OrderNode* node) {
    total_quantity -= (node->order.quantity - node->order.filled_quantity);

    if (node->prev) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }

    node->prev = nullptr;
    node->next = nullptr;
}

// OrderBook implementation

OrderBook::OrderBook(std::string symbol) : symbol_(std::move(symbol)) {}

void OrderBook::add_order(const Order& order) {
    auto* node = new OrderNode(order);
    order_map_[order.order_id] = node;

    if (order.type == OrderType::MARKET) {
        match_order(node);
        // MARKET orders don't rest on book, delete node after matching
        if (!node->order.is_filled()) {
            // MARKET order partially filled but book empty (no liquidity)
            // discard remaining since no resting
        }
        order_map_.erase(order.order_id);
        delete node;
    } else {
        match_order(node);
        if (!node->order.is_filled()) {
            add_to_book(node);
        } else {
            order_map_.erase(order.order_id);
            delete node;
        }
    }
}

void OrderBook::cancel_order(uint64_t order_id) {
    auto it = order_map_.find(order_id);
    if (it == order_map_.end()) return;

    OrderNode* node = it->second;
    
    if (node->order.side == Side::BUY) {
        auto& book = bids_;

        auto level_it = book.find(node->order.price);
        if (level_it == book.end()) return;

        auto& level = level_it->second;
        level.remove(node);

        if (level.empty()) {
            book.erase(level_it);
        }
    } else {
        auto& book = asks_;

        auto level_it = book.find(node->order.price);
        if (level_it == book.end()) return;

        auto& level = level_it->second;
        level.remove(node);

        if (level.empty()) {
            book.erase(level_it);
        }
    }

    order_map_.erase(it);
    delete node;
}

std::vector<PriceLevel> OrderBook::get_bids() const {
    std::vector<PriceLevel> result;
    for (const auto& [price, level] : bids_) {
        result.emplace_back(price, level.total_quantity);
    }
    return result;
}

std::vector<PriceLevel> OrderBook::get_asks() const {
    std::vector<PriceLevel> result;
    for (const auto& [price, level] : asks_) {
        result.emplace_back(price, level.total_quantity);
    }
    return result;
}

std::vector<Trade> OrderBook::get_trades() const {
    return trades_;
}

std::shared_ptr<Order> OrderBook::get_order(uint64_t order_id) const {
    auto it = order_map_.find(order_id);
    if (it != order_map_.end()) {
        return std::make_shared<Order>(it->second->order);
    }
    return nullptr;
}

void OrderBook::match_order(OrderNode* taker) {
    // For BUY taker, match with asks ascending (lowest ask)
    // For SELL taker, match with bids descending (highest bid)

    if(taker->order.side == Side::BUY) {
        auto& opposite_book = asks_;

        auto it = opposite_book.begin();

        while (it != opposite_book.end() && !taker->order.is_filled()) {
            Price book_price = it->first;

            // Check if prices cross
            bool price_match = false;
            if (taker->order.type == OrderType::MARKET) {
                price_match = true; // Market order matches any price
            } else if (taker->order.side == Side::BUY) {
                price_match = taker->order.price >= book_price;
            } else {
                price_match = taker->order.price <= book_price;
            }

            if (!price_match) break;

            auto& level = it->second;
            OrderNode* maker = level.head;

            while (maker && !taker->order.is_filled()) {
                Quantity trade_qty = std::min(
                    taker->order.quantity - taker->order.filled_quantity,
                    maker->order.quantity - maker->order.filled_quantity
                );

                Price trade_price = maker->order.price;

                execute_trade(taker, maker, trade_price, trade_qty);

                if (maker->order.is_filled()) {
                    OrderNode* to_delete = maker;
                    maker = maker->next;
                    level.remove(to_delete);
                    order_map_.erase(to_delete->order.order_id);
                    delete to_delete;
                } else {
                    maker = maker->next;
                }
            }

            if (level.empty()) {
                it = opposite_book.erase(it);
            } else {
                ++it;
            }
        }
    } else {
        auto& opposite_book = bids_;

        auto it = opposite_book.begin();

        while (it != opposite_book.end() && !taker->order.is_filled()) {
            Price book_price = it->first;

            // Check if prices cross
            bool price_match = false;
            if (taker->order.type == OrderType::MARKET) {
                price_match = true; // Market order matches any price
            } else if (taker->order.side == Side::BUY) {
                price_match = taker->order.price >= book_price;
            } else {
                price_match = taker->order.price <= book_price;
            }

            if (!price_match) break;

            auto& level = it->second;
            OrderNode* maker = level.head;

            while (maker && !taker->order.is_filled()) {
                Quantity trade_qty = std::min(
                    taker->order.quantity - taker->order.filled_quantity,
                    maker->order.quantity - maker->order.filled_quantity
                );

                Price trade_price = maker->order.price;

                execute_trade(taker, maker, trade_price, trade_qty);

                if (maker->order.is_filled()) {
                    OrderNode* to_delete = maker;
                    maker = maker->next;
                    level.remove(to_delete);
                    order_map_.erase(to_delete->order.order_id);
                    delete to_delete;
                } else {
                    maker = maker->next;
                }
            }

            if (level.empty()) {
                it = opposite_book.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void OrderBook::execute_trade(OrderNode* taker, OrderNode* maker, Price price, Quantity qty) {
    taker->order.filled_quantity += qty;
    maker->order.filled_quantity += qty;

    trades_.push_back({
        next_trade_id_++,
        (taker->order.side == Side::BUY) ? taker->order.order_id : maker->order.order_id,
        (taker->order.side == Side::SELL) ? taker->order.order_id : maker->order.order_id,
        taker->order.symbol,
        price,
        qty,
        std::max(taker->order.timestamp, maker->order.timestamp)
    });
}

void OrderBook::add_to_book(OrderNode* node) {
    if (node->order.side == Side::BUY) {
        auto& book = bids_;
        auto& level = book[node->order.price];
        level.append(node);
    } else {
        auto& book = asks_;
        auto& level = book[node->order.price];
        level.append(node);
    }
    
}

} // namespace OrderBook
