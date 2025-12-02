#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../CoordinateConvert.cpp"  // include the converter (it includes Point.cpp)

namespace py = pybind11;

PYBIND11_MODULE(coordinate_convert, m) {
    m.doc() = "Coordinate conversion bindings using existing C++ code";

    py::class_<Point>(m, "Point")
        .def(py::init<>())
        .def(py::init<long double, long double, long double>())
        .def_readwrite("x", &Point::x)
        .def_readwrite("y", &Point::y)
        .def_readwrite("z", &Point::z)
        .def("getDist", (long double (Point::*)(const Point&)) &Point::getDist);

    py::class_<CoordinateConvert>(m, "CoordinateConvert")
        .def(py::init<>())
        .def("BLH2XYZ", (Point (CoordinateConvert::*)(const Point&)) &CoordinateConvert::BLH2XYZ)
        .def("BLH2NED", (Point (CoordinateConvert::*)(const Point&, const Point&)) &CoordinateConvert::BLH2NED)
        .def("BLH2NEDK", (Point (CoordinateConvert::*)(const Point&, const Point&)) &CoordinateConvert::BLH2NEDK)
        .def("BLH2XYZ_vec", (std::vector<Point> (CoordinateConvert::*)(const std::vector<Point>&)) &CoordinateConvert::BLH2XYZ)
        .def("BLH2NED_vec", (std::vector<Point> (CoordinateConvert::*)(const std::vector<Point>&, const Point&)) &CoordinateConvert::BLH2NED);
}
