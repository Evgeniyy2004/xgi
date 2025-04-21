from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension

ext_modules = [
    Pybind11Extension(
        "cpp_functions",
        sorted(glob("xgi/cpp_functions/*.cpp")),  # Sort source files for reproducibility
    ),
]

setup(..., ext_modules=ext_modules)
