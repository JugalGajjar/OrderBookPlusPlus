#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(minimal, m) {
    m.def("hello", []() {
        return "Hello, World!";
    });
}
