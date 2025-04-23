#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
namespace py = pybind11;

void normalize(std::vector<double>& vec) {
    double sum = 0.0;
    for (const auto& val : vec) {
        sum += std::abs(val);
    }
    if (sum == 0.0) return;
    
    const double scale = 1.0 / sum;
    for (auto& val : vec) {
        val *= scale;
    }
}

std::pair<std::vector<double>, std::vector<double>> 
compute_centralities(
    py::array_t<double> I_array,
		const py::dict& dict_nodes,
		const py::dict& dict_edges,
    int max_iter,
    double tol
) {
    py::buffer_info I_buf = I_array.request();
    if (I_buf.ndim != 2) {
        throw std::runtime_error("Incidence matrix must be 2-dimensional");
  }

    const size_t n = I_buf.shape[0];
    const size_t m = I_buf.shape[1];
    const double* I = static_cast<double*>(I_buf.ptr);

    std::vector<double> x(n, 1.0/n);
    std::vector<double> y(m, 1.0/m);
    std::vector<double> new_x(n);
    std::vector<double> new_y(m);
    std::vector<double> buffer_n(n);
    std::vector<double> buffer_m(m);

    for (int iter = 0; iter < max_iter; ++iter) {
        // Вычисляем I @ (y^2)
        std::fill(buffer_n.begin(), buffer_n.end(), 0.0);
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                buffer_n[i] += I[i*m + j] * y[j] * y[j];
            }
        }

        // Вычисляем new_x = sqrt(x * sqrt(I @ y^2))
        for (size_t i = 0; i < n; ++i) {
            new_x[i] = std::sqrt(x[i] * std::sqrt(buffer_n[i]));
        }
        normalize(new_x);

        // Вычисляем I.T @ (x^2)
        std::fill(buffer_m.begin(), buffer_m.end(), 0.0);
        for (size_t j = 0; j < m; ++j) {
            for (size_t i = 0; i < n; ++i) {
                buffer_m[j] += I[i*m + j] * x[i] * x[i];
            }
        }
        // Вычисляем new_y = sqrt(y * sqrt(I.T @ x^2))
        for (size_t j = 0; j < m; ++j) {
            new_y[j] = std::sqrt(y[j] * std::sqrt(buffer_m[j]));
        }
        normalize(new_y);

        // Проверка сходимости
        double diff = 0.0;
        for (size_t i = 0; i < n; ++i) {
            diff += std::abs(new_x[i] - x[i]);
        }
        for (size_t j = 0; j < m; ++j) {
            diff += std::abs(new_y[j] - y[j]);
       }

       if (diff < tol) {
            return {new_x, new_y};
       }
       x = new_x;
       y = new_y;
    }
    return {x, y};
}


