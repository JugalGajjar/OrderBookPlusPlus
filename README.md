# OrderBook++

**OrderBook++** is a high-performance, modular C++ order matching engine designed for simulating and analyzing financial trading systems. It supports market, limit, and cancel order types, tracks a complete trade history, and is built for both extensibility and speed. The project also includes Python bindings via `pybind11`, enabling interactive experimentation, scripting, and visualization.

---

## 📌 Features

- **Limit, Market, and Cancel Orders**
- **Real-Time Matching Engine** with bid/ask order book state
- **Trade History Tracking** for post-trade analytics
- **Comprehensive Testing** using `Catch2`
- **Python Bindings** via `pybind11` for rapid prototyping
- **Visualization Tools** in Python for order flow and trade behavior
- **Modular C++ Codebase** with a clean and extensible architecture

---

## 📁 Project Structure

```
OrderBookPlusPlus/
├── include/
│ ├── orderbook.h           # Order book class and methods
│ └── types.h               # Order types and data structures
├── src/
│ ├── orderbook.cpp         # Core matching engine logic
│ └── pybind.cpp            # pybind11 Python bindings
├── tests/
│ ├── test_orderbook.cpp    # Unit tests for core logic
│ └── test_performance.cpp  # Performance benchmarks
├── python/
│ ├── demo.py               # Sample usage from Python
│ └── visualization.py      # Visualization utilities
├── CMakeLists.txt          # CMake build configuration
├── README.md               # Project README file
└── LICENSE                 # Project LICENSE
```

---

## ⚙️ Build Instructions

### 📦 Dependencies

- **C++17** compiler (GCC, Clang, or MSVC)
- [CMake ≥ 3.15](https://cmake.org/)
- [pybind11](https://github.com/pybind/pybind11)
- [Catch2](https://github.com/catchorg/Catch2)
- **Python 3.7+** (for bindings and demo)

### Option 1: Build with `vcpkg`

```bash
# Install dependencies
vcpkg install catch2 pybind11

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake

# Build
make -j$(nproc) # OR make -j$(sysctl -n hw.logicalcpu)
```

### Option 2: Build with CMake FetchContent
Already configured in CMakeLists.txt to fetch Catch2 and pybind11 automatically.

```bash
mkdir build && cd build
cmake ..
make -j$(nproc) # OR make -j$(sysctl -n hw.logicalcpu)
```

---

## 🧪 Testing
Run all unit and performance tests using CTest:

```bash
cd build
ctest --output-on-failure
```

Tests cover:
- Order creation and cancellation
- Matching engine correctness
- Edge cases (e.g., partial matches, reordering)
- Performance under high load

---

## 🐍 Python Integration

### Using Python Bindings

```bash
cd build
python3
>>> import orderbook
>>> ob = orderbook.OrderBook()
>>> ob.add_limit_order(order_id=1, price=100, quantity=10, side='buy')
>>> print(ob)
```

Check python/demo.py for full usage example and python/visualization.py for charting order book state and trade history.

---

## 📊 Visualization
Use the Python module to visualize book snapshots, spreads, and trade volumes. Example:

```python
from visualization import visualize_order_book
visualize_order_book(ob)
```

---

## 🛠️ Future Enhancements

- Persistent logging of all trades and order book snapshots
- WebSocket-based live order flow simulation
- Multi-threaded matching engine
- RESTful API using FastAPI and C++ backend

---

## 📄 License

This project is licensed under the [MIT License](https://opensource.org/license/mit).
