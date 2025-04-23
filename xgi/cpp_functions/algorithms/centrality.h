#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <vector>
#include <functional>

namespace py = pybind11;

std::tuple<std::vector<double>, std::vector<double>, std::vector<int>, std::vector<int>> 
compute_centralities(
    const std::unordered_map<int, std::unordered_set<int>>& nodes,
    const std::unordered_map<int, std::unordered_set<int>>& edges,
    int max_iter,
    double tol
);
