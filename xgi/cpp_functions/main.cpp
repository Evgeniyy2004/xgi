#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include "algorithms/centrality.h"
#include <pybind11/stl.h>  // Required for std::vector conversion

namespace py = pybind11;

PYBIND11_MODULE(cpp_functions, m) {
		py::module_ m_algorithms = m.def_submodule("algorithms");
		py::module_ m_cetrality = m_algorithms.def_submodule("centrality");
    m_cetrality.def("compute_centralities", &compute_centralities,
					py::arg("nodes"),
					py::arg("edges"),
          py::arg("max_iter") = 100,
          py::arg("tol") = 1e-6,
          "Computes node and edge centralities from incidence matrix");
}
