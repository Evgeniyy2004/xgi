#include <pybind11/pybind11.h>
#include "algorithms/centrality.h"

namespace py = pybind11;

PYBIND11_MODULE(cpp_functions, m) {
		py::module_ m_algorithms = m.def_submodule("algorithms");
		py::module_ m_cetrality = m_algorithms.def_submodule("centrality");
    m_cetrality.def("compute_centralities", &compute_centralities,
          py::arg("I"),
          py::arg("f") = py::cpp_function([](double x) { return x * x; }),
          py::arg("g") = py::cpp_function([](double x) { return std::sqrt(x); }),
          py::arg("phi") = py::cpp_function([](double x) { return x * x; }),
          py::arg("psi") = py::cpp_function([](double x) { return std::sqrt(x); }),
          py::arg("max_iter") = 100,
          py::arg("tol") = 1e-6,
          "Computes node and edge centralities from incidence matrix");

    m.doc() = "pybind11 example plugin"; // optional module docstring
}
