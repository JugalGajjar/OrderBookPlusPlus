from orderbook_py import OrderBook, Side, OrderType, Order
from visualization import visualize_order_book
import time

def main():
    ob = OrderBook("AAPL")
    current_time = int(time.time())

    # Add bids
    for price in [99.0, 99.5, 100.0, 100.5]:
        ob.add_order(Order(
            order_id=1000 + int(price * 10),
            symbol="AAPL",
            side=Side.BUY,
            type=OrderType.LIMIT,
            price=price,
            quantity=1000,
            timestamp=current_time
        ))

    # Add asks
    for price in [101.0, 101.5, 102.0, 102.5]:
        ob.add_order(Order(
            order_id=2000 + int(price * 10),
            symbol="AAPL",
            side=Side.SELL,
            type=OrderType.LIMIT,
            price=price,
            quantity=800,
            timestamp=current_time
        ))

    # Add a market buy order to consume some asks
    ob.add_order(Order(
        order_id=3000,
        symbol="AAPL",
        side=Side.BUY,
        type=OrderType.MARKET,
        price=0.0,  # Ignored for MARKET
        quantity=1500,
        timestamp=current_time
    ))

    # Print current bids and asks
    print("Bids:")
    for price, qty in ob.get_bids():
        print(f"Price: {price}, Quantity: {qty}")

    print("\nAsks:")
    for price, qty in ob.get_asks():
        print(f"Price: {price}, Quantity: {qty}")

    print("\nTrades:")
    for trade in ob.get_trades():
        print(f"Trade ID: {trade.trade_id}, Price: {trade.price}, Quantity: {trade.quantity}")
    
    visualize_order_book(ob)

if __name__ == "__main__":
    main()
