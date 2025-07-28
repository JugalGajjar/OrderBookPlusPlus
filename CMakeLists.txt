cmake_minimum_required(VERSION 3.15)
project(OrderBookPlusPlus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python and pybind11 packages
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

# Find GoogleTest from Miniconda environment
find_package(GTest REQUIRED)

# Include directories for all
include_directories(
    include
    ${Python_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}   # <-- Add gtest include dirs here
)

# Source files and headers
file(GLOB_RECURSE ORDERBOOK_HEADERS include/*.h)
file(GLOB_RECURSE ORDERBOOK_SOURCES src/*.cpp)

# Core static library
add_library(orderbook_core STATIC ${ORDERBOOK_SOURCES} ${ORDERBOOK_HEADERS})
target_include_directories(orderbook_core PUBLIC
    include
    ${Python_INCLUDE_DIRS}
    ${pybind11_INCLUDE_DIRS}
)

# Test executable: test_orderbook
add_executable(test_orderbook tests/test_orderbook.cpp)
target_link_libraries(test_orderbook PRIVATE orderbook_core GTest::GTest GTest::Main)  # Use imported targets
target_include_directories(test_orderbook PRIVATE include)

# Test executable: test_performance
add_executable(test_performance tests/test_performance.cpp)
target_link_libraries(test_performance PRIVATE orderbook_core GTest::GTest GTest::Main)
target_include_directories(test_performance PRIVATE include)

# Enable testing
enable_testing()
add_test(NAME test_orderbook COMMAND test_orderbook)
add_test(NAME test_performance COMMAND test_performance)

# Python bindings module
add_library(orderbook_py MODULE src/pybind.cpp)
target_include_directories(orderbook_py PRIVATE
    include
    ${pybind11_INCLUDE_DIRS}
    ${Python_INCLUDE_DIRS}
)
target_link_libraries(orderbook_py PRIVATE
    orderbook_core
    pybind11::module
    Python::Python
)

# macOS specific flags to avoid "module has no main" error for Python extensions
set_target_properties(orderbook_py PROPERTIES
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/python"
)
