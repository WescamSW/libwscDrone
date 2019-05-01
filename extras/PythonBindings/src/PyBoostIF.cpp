
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include "Bebop2CtrlIF.h"
// #include "gilHandler.hpp"

PyObject* getFrameBuffer_wrap(Bebop2CtrlIF& self)
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
    class_<Bebop2CtrlIF>("Bebop2CtrlIF", init<int>())
        // .def("changeCount", &Bebop2CtrlIF::Bebop2CtrlIF)
        // .def("changeCount", &Bebop2CtrlIF::changeCount)
        .def("takeoffDrone",       &Bebop2CtrlIF::takeoffDrone)
        .def("moveRelativeMetres", &Bebop2CtrlIF::moveRelativeMetres)
        .def("landDrone",          &Bebop2CtrlIF::landDrone)
        .def("stopDrone",          &Bebop2CtrlIF::stopDrone)
        .def("getFrameBuffer",     &getFrameBuffer_wrap)
        .def("killDrone",          &Bebop2CtrlIF::killDrone)
        // .def("getBatteryLevel", &Bebop2CtrlIF::getBatteryLevel)
        // .def("setBatteryLevel", &Bebop2CtrlIF::setBatteryLevel)
    ;
}