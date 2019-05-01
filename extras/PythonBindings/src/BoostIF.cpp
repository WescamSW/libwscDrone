
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include "PyBebop.h"
// #include "gilHandler.hpp"

PyObject* getFrameBuffer_wrap(PyBebop& self)
{
    PyObject* pymemview;
    char* export_data;

    export_data = self.getFrameBuffer();

    int frameSize = wscDrone::BEBOP2_STREAM_HEIGHT * wscDrone::BEBOP2_STREAM_WIDTH * 3;
    pymemview = PyMemoryView_FromMemory((char*) export_data, frameSize, PyBUF_READ);
    // return PyBytes_FromObject(pymemview);
    return pymemview;
}

using namespace boost::python;

BOOST_PYTHON_MODULE(libwscDrone)
{
    class_<PyBebop>("PyBebop", init<int>())
        // .def("changeCount", &PyBebop::PyBebop)
        // .def("changeCount", &PyBebop::changeCount)
        .def("takeoffDrone",       &PyBebop::takeoffDrone)
        .def("moveRelativeMetres", &PyBebop::moveRelativeMetres)
        .def("landDrone",          &PyBebop::landDrone)
        .def("stopDrone",          &PyBebop::stopDrone)
        .def("getFrameBuffer",     &getFrameBuffer_wrap)
        .def("killDrone",          &PyBebop::killDrone)
        // .def("getBatteryLevel", &PyBebop::getBatteryLevel)
        // .def("setBatteryLevel", &PyBebop::setBatteryLevel)
    ;
}