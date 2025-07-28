cmake_minimum_required(VERSION 3.14)
project(OrderBookPlusPlus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent for Catch2 and pybind11
include(FetchContent)

# -------------------------------
# Fetch Catch2
# -------------------------------
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2 # or latest stable
)
FetchContent_MakeAvailable(catch2)

# -------------------------------
# Fetch pybind11
# -------------------------------
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.11.1 # or latest stable
)
FetchContent_MakeAvailable(pybind11)

# -------------------------------
# OrderBook core library
# -------------------------------
file(GLOB_RECURSE ORDERBOOK_HEADERS include/*.h)
file(GLOB_RECURSE ORDERBOOK_SOURCES src/*.cpp)

add_library(orderbook_core STATIC ${ORDERBOOK_SOURCES} ${ORDERBOOK_HEADERS})
target_include_directories(orderbook_core PUBLIC include)

# -------------------------------
# Test: test_orderbook
# -------------------------------
add_executable(test_orderbook tests/test_orderbook.cpp)
target_link_libraries(test_orderbook PRIVATE orderbook_core Catch2::Catch2WithMain)
target_include_directories(test_orderbook PRIVATE include)

# -------------------------------
# Test: test_performance
# -------------------------------
add_executable(test_performance tests/test_performance.cpp)
target_link_libraries(test_performance PRIVATE orderbook_core Catch2::Catch2WithMain)
target_include_directories(test_performance PRIVATE include)

# -------------------------------
# Python Bindings
# -------------------------------
pybind11_add_module(orderbook_py src/pybind.cpp)
target_link_libraries(orderbook_py PRIVATE orderbook_core)
target_include_directories(orderbook_py PRIVATE include)

# -------------------------------
# Enable testing
# -------------------------------
include(CTest)
include(Catch)
catch_discover_tests(test_orderbook)
catch_discover_tests(test_performance)
