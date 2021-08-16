#include <ldsCtrlEst_h/lds_gaussian_fit.h>
#include <ldsCtrlEst_h/lds_gaussian_sys.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <carma>

#include "ldsutils.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

// this allows for the shorthand "my_arg"_a instead of py::arg("my_arg")
using namespace pybind11::literals;
using namespace std;
using namespace lds;

namespace py = pybind11;
namespace glds = lds::gaussian;

PYBIND11_MODULE(gaussian, m) {
  m.doc() = "Gaussian classes from ldsCtrlEst";  // optional module docstring

  /*
  ---------------- Gaussian System class ---------------------
  */
  py::class_<glds::System, lds::System>(m, "System")
      .def(py::init<>())  // default constructor
      .def(py::init<size_t, size_t, size_t, data_t,
                    data_t, data_t, data_t>(),
           "n_u"_a, "n_x"_a, "n_y"_a, "dt"_a, "p0"_a = kDefaultP0,
           "q0"_a = kDefaultQ0, "r0"_a = kDefaultR0)

      // getters/setters not in base System
      .def_property("R", &glds::System::R, &glds::System::set_R)
      .def_property("Ke", &glds::System::Ke, &glds::System::set_Ke)
      .def_property("Ke_m", &glds::System::Ke_m, &glds::System::set_Ke_m)

      // other functions
      // NOTE: this might be unnecessary if functions were virtual,
      // but I'm not touching the base code right now
      .def("Simulate", &glds::System::Simulate)
      .def("Print", &glds::System::Print)
      .def("__str__", [](glds::System& system) {
        return ldsutils::capture_output([&system]() { system.Print(); });
      });

  /*
  ---------------- Gaussian Fit class ---------------------
  */
  py::class_<glds::Fit, lds::Fit>(m, "Fit")
      // constructors
      .def(py::init<>())
      .def(py::init<size_t, size_t, size_t, data_t>(), "n_u"_a, "n_x"_a,
           "n_y"_a, "dt"_a)
      ;
      // .def_property(
      //     "R", [](const glds::Fit& self) { return self.R(); },
      //     &glds::Fit::set_R, "measurement noise covariance");

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
