#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <vector>
#include <functional>

namespace py = pybind11;

std::pair<std::vector<double>, std::vector<double>> 
compute_centralities(
    py::array_t<double> I_array,
//    const std::function<double(double)>& f,
//    const std::function<double(double)>& g,
//    const std::function<double(double)>& phi,
//    const std::function<double(double)>& psi,
		const py::dict& dict_nodes,
		const py::dict& dict_edges,
    int max_iter,
    double tol

);
