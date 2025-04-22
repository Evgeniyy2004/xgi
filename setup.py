from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "line_graph_cpp",
        ["bindings.cpp"],  # Укажите файлы с оберткой
        include_dirs=["."],  # Добавьте директорию с заголовочными файлами
    ),
]

setup(
    name="line_graph_cpp",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)