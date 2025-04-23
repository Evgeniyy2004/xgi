from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "xgi.cpp_functions",
        sources=[
            "xgi/cpp_functions/main.cpp",
            "xgi/cpp_functions/algorithms/centrality.cpp",
            "xgi/cpp_functions/algorithms/connected.cpp"
        ],
        include_dirs=["extern/pybind11/include"],
        extra_compile_args=["-O3", "-Wall"],
        define_macros=[("PYBIND11", None)],
        cxx_std=11,
    )
]

setup(
    ext_modules=ext_modules,
)