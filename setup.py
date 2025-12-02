from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "coordinate_convert",
        ["src/pybind/coordinate_bindings.cpp"],
        include_dirs=["src"],
        extra_compile_args=["-std=c++17"],
        language="c++"
    )
]

setup(
    name="coordinate_convert",
    version="0.1",
    description="Bindings for CoordinateConvert C++ code",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    install_requires=["pybind11>=2.6.0"],
)
