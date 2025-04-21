#include "centrality.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <limits>
#include <algorithm>

namespace py = pybind11;

// Helper functions
double l1_norm(const std::vector<double>& vec) {
    double sum = 0.0;
    for (const auto& val : vec) {
        sum += std::abs(val);
    }
    return sum;
}

double vector_diff_norm(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw std::runtime_error("Vector sizes don't match");
    }
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        sum += std::abs(a[i] - b[i]);
    }
    return sum;
}

// Main function that works with numpy arrays
std::pair<std::vector<double>, std::vector<double>> 
compute_centralities(
    py::array_t<double> I_array,
    const std::function<double(double)>& f,
    const std::function<double(double)>& g,
    const std::function<double(double)>& phi,
    const std::function<double(double)>& psi,
    int max_iter,
    double tol
) {
    py::buffer_info I_buf = I_array.request();
    if (I_buf.ndim != 2) {
        throw std::runtime_error("Incidence matrix must be 2-dimensional");
    }

    size_t n = I_buf.shape[0]; // number of nodes
    size_t m = I_buf.shape[1]; // number of edges
    double* I_ptr = static_cast<double*>(I_buf.ptr);

    // Initialize vectors
    std::vector<double> x(n, 1.0 / n);
    std::vector<double> y(m, 1.0 / m);
    std::vector<double> new_x(n);
    std::vector<double> new_y(m);

    // Main iteration loop
    double check = std::numeric_limits<double>::infinity();
    int it = 0;

    for (; it < max_iter; ++it) {
        // Compute I @ f(y)
        std::vector<double> I_fy(n, 0.0);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                I_fy[i] += I_ptr[i * m + j] * f(y[j]);
            }
        }

        // Compute u = (x * g(I @ f(y))) ^ 0.5
        for (size_t i = 0; i < n; ++i) {
            new_x[i] = std::sqrt(x[i] * g(I_fy[i]));
        }

        // Normalize new_x with sign preservation
        double x_norm = l1_norm(new_x);
        double sign = new_x[0] >= 0 ? 1.0 : -1.0;
        for (auto& val : new_x) {
            val = sign * val / x_norm;
        }

        // Compute I.T @ phi(x)
        std::vector<double> IT_phi_x(m, 0.0);
        for (size_t j = 0; j < m; ++j) {
            for (size_t i = 0; i < n; ++i) {
                IT_phi_x[j] += I_ptr[i * m + j] * phi(x[i]);
            }
        }

        // Compute v = (y * psi(I.T @ phi(x))) ^ 0.5
        for (size_t j = 0; j < m; ++j) {
            new_y[j] = std::sqrt(y[j] * psi(IT_phi_x[j]));
        }

        // Normalize new_y with sign preservation
        double y_norm = l1_norm(new_y);
        sign = new_y[0] >= 0 ? 1.0 : -1.0;
        for (auto& val : new_y) {
            val = sign * val / y_norm;
        }

        // Check convergence
        check = vector_diff_norm(new_x, x) + vector_diff_norm(new_y, y);
        if (check < tol) {
            break;
        }

        // Update for next iteration
        x = new_x;
        y = new_y;
    }

    if (it == max_iter) {
        py::gil_scoped_acquire acquire;
        PyErr_WarnEx(PyExc_RuntimeWarning, "Iteration did not converge!", 1);
    }

    return {new_x, new_y};
}


