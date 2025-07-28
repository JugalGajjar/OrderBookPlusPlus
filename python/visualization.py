import matplotlib.pyplot as plt
import numpy as np

def visualize_order_book(orderbook, top_n=10):
    bids = orderbook.get_bids()[:top_n]
    asks = orderbook.get_asks()[:top_n]

    # Extract prices and quantities
    bid_prices = [price for price, qty in bids]
    bid_qtys = [qty for price, qty in bids]

    ask_prices = [price for price, qty in asks]
    ask_qtys = [qty for price, qty in asks]

    fig, ax = plt.subplots(figsize=(10, 6))

    # Plot bids - descending prices from left to right
    y_pos_bids = np.arange(len(bid_prices))
    ax.barh(y_pos_bids, bid_qtys, color='green', alpha=0.6, label='Bids')
    ax.set_yticks(y_pos_bids)
    ax.set_yticklabels([f"{p:.2f}" for p in bid_prices])
    ax.invert_yaxis()  # Highest bid on top

    # Plot asks - descending prices from left to right, offset to right
    y_pos_asks = np.arange(len(ask_prices))
    ax.barh(y_pos_asks + len(bid_prices) + 1, ask_qtys, color='red', alpha=0.6, label='Asks')
    ax.set_yticks(list(y_pos_bids) + list(y_pos_asks + len(bid_prices) + 1))
    ax.set_yticklabels(
        [f"{p:.2f}" for p in bid_prices] +
        [f"{p:.2f}" for p in ask_prices]
    )

    ax.set_xlabel('Quantity')
    ax.set_title(f'Order Book Depth for {orderbook.symbol}')
    ax.legend()

    plt.tight_layout()
    plt.show()
