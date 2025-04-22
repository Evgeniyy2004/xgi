#include <vector>
#include <unordered_set>
#include <tuple>
#include <pybind11/pybind11.h>

namespace py = pybind11;

// Функция для вычисления пересечений и весов
std::vector<std::tuple<int, int, double>> compute_line_graph_edges(
    const std::vector<std::unordered_set<int>>& hyperedges,
    int s,
    const std::string& weights,
    const py::function& progress_callback) {

    std::vector<std::tuple<int, int, double>> edges;
    int n = hyperedges.size();

    int total_pairs = n * (n - 1) / 2; // Общее количество пар
    int processed_pairs = 0;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            std::unordered_set<int> intersection;
            for (const auto& elem : hyperedges[i]) {
                if (hyperedges[j].count(elem)) {
                    intersection.insert(elem);
                }
            }

            if (intersection.size() >= s) {
                double weight = 0.0;
                if (weights == "absolute") {
                    weight = intersection.size();
                } else if (weights == "normalized") {
                    weight = static_cast<double>(intersection.size()) /
                             std::min(hyperedges[i].size(), hyperedges[j].size());
                }
                edges.emplace_back(i, j, weight);
            }

            // Вызываем callback для обновления прогресса
            ++processed_pairs;
            if (i%1000 == 0) progress_callback(1000, processed_pairs, total_pairs);
        }
    }
    return edges;
}