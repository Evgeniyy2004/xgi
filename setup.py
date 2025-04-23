from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "xgi.cpp_functions",
        sources=[
            "xgi/cpp_functions/main.cpp",
            "xgi/cpp_functions/algorithms/centrality.cpp",
            "xgi/cpp_functions/algorithms/connected.cpp",
            "xgi/cpp_functions/convert/line_graph.cpp"
        ],
        include_dirs=["extern/pybind11/include"],
        extra_compile_args=["-O3", "-Wall", "-fopenmp"],
        define_macros=[("PYBIND11", None)],
        extra_link_args=["-fopenmp"],
        cxx_std=17,
    )
]

setup(
    ext_modules=ext_modules,
)
