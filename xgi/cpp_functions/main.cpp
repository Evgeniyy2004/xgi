#include <pybind11/pybind11.h>
#include "algorithms/centrality.h"

namespace py = pybind11;

PYBIND11_MODULE(cpp_functions, m) {
		py::module_ m_algorithms = m.def_submodule("algorithms");
		py::module_ m_cetrality = m_algorithms.def_submodule("centrality");
    m_cetrality.def("add", &add, "A function that adds two numbers");

    m.doc() = "pybind11 example plugin"; // optional module docstring
}
