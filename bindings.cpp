#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "line_graph.cpp"

namespace py = pybind11;

PYBIND11_MODULE(line_graph_cpp, m) {
    m.def("compute_line_graph_edges", &compute_line_graph_edges,
        "Compute line graph edges in C++",
        py::arg("hyperedges"),
        py::arg("s"),
        py::arg("weights"),
        py::arg("progress_callback"));
}