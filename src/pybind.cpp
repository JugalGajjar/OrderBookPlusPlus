#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "orderbook.h"

namespace py = pybind11;

PYBIND11_MODULE(orderbook_py, m) {
    py::enum_<OrderBook::Side>(m, "Side")
        .value("BUY", OrderBook::Side::BUY)
        .value("SELL", OrderBook::Side::SELL)
        .export_values();

    py::enum_<OrderBook::OrderType>(m, "OrderType")
        .value("MARKET", OrderBook::OrderType::MARKET)
        .value("LIMIT", OrderBook::OrderType::LIMIT)
        .export_values();

    py::class_<OrderBook::Order>(m, "Order")
        .def(py::init<>())
        .def_readwrite("order_id", &OrderBook::Order::order_id)
        .def_readwrite("symbol", &OrderBook::Order::symbol)
        .def_readwrite("side", &OrderBook::Order::side)
        .def_readwrite("type", &OrderBook::Order::type)
        .def_readwrite("price", &OrderBook::Order::price)
        .def_readwrite("quantity", &OrderBook::Order::quantity)
        .def_readwrite("timestamp", &OrderBook::Order::timestamp)
        .def_readonly("filled_quantity", &OrderBook::Order::filled_quantity)
        .def("is_filled", &OrderBook::Order::is_filled);

    py::class_<OrderBook::Trade>(m, "Trade")
        .def_readonly("trade_id", &OrderBook::Trade::trade_id)
        .def_readonly("buyer_order_id", &OrderBook::Trade::buyer_order_id)
        .def_readonly("seller_order_id", &OrderBook::Trade::seller_order_id)
        .def_readonly("symbol", &OrderBook::Trade::symbol)
        .def_readonly("price", &OrderBook::Trade::price)
        .def_readonly("quantity", &OrderBook::Trade::quantity)
        .def_readonly("timestamp", &OrderBook::Trade::timestamp);

    py::class_<OrderBook::OrderBookEngine>(m, "OrderBook")
        .def(py::init<const std::string &>())
        .def("add_order", &OrderBook::OrderBookEngine::add_order)
        .def("cancel_order", &OrderBook::OrderBookEngine::cancel_order)
        .def("get_bids", &OrderBook::OrderBookEngine::get_bids)
        .def("get_asks", &OrderBook::OrderBookEngine::get_asks)
        .def("get_trades", &OrderBook::OrderBookEngine::get_trades)
        .def("get_order", &OrderBook::OrderBookEngine::get_order);
}
