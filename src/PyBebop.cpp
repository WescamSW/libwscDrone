/*
 * api.cpp
 *
 * Create on March 27, 2019
 *
 */

// #include <cstdio>
#include <iostream>

#include "PyBebop.h"

using namespace std;
using namespace wscDrone;

const std::string BEBOP_IP_ADDRESS = "192.168.42.1";

using VideoFrameGeneric = PyFrame;

PyBebop::PyBebop(int callSign)
{
    m_frame = std::make_shared<VideoFrameGeneric>(wscDrone::BEBOP2_STREAM_HEIGHT, wscDrone::BEBOP2_STREAM_WIDTH);


    /* Assign IP Address */
    std::string ipAddress;
    switch (callSign) {
    case 0 :
        m_drone = std::make_shared<Bebop2>(Callsign::ALPHA, m_frame);
        m_flightAltitude = 1;
        break;
    case 1 :
        m_drone = std::make_shared<Bebop2>(Callsign::BRAVO, m_frame);
        m_flightAltitude = 1;
        break;
    case 2 :
        m_drone = std::make_shared<Bebop2>(Callsign::CHARLIE, m_frame);
        m_flightAltitude = 1;
        break;
    case 3 :
        m_drone = std::make_shared<Bebop2>(Callsign::LONE_WOLF, m_frame);
        m_flightAltitude = 1;
        break;
    default :
        cout << "Invalid call sign.. exiting.." << endl; /*TODO exit the program? */
        break;
    }

    startDrone();

    // count[0] = 0;
    // count[1] = 0;
}

// void PyBebop::changeCount(int idx, int value)
// {
//     count[idx]  = value;

//     printf("Changed %d: %d\n", idx, count[idx]);
// }

void PyBebop::testFoo()
{
    cout << "testFoo" << endl;
    wscDrone::waitSeconds(1);
}

void PyBebop::takeoffDrone() {
    m_drone->getPilot()->takeOff();
    // wscDrone::waitSeconds(3);
    // setFlightAltitude();
    // #error nice
}

void PyBebop::landDrone()
{
    m_drone->getCameraControl()->setForward();
    m_drone->getPilot()->land();
    // wscDrone::waitSeconds(5);
}

void PyBebop::stopDrone()
{
    landDrone();
    m_drone->getVideoDriver()->stop();
    m_drone->getDroneController()->stop();
}

// /*********************/
// /* PRIVATE FUCNTIONS */
// /*********************/

void PyBebop::startDrone()
{
    m_drone->getDroneController()->start();
    cout << "Done CONTROL START" << endl;
    wscDrone::waitSeconds(1);
    m_drone->getVideoDriver()->start();
    cout << "Done VIDEO START" << endl;
    m_drone->getCameraControl()->setForward();
    wscDrone::waitSeconds(1);
    cout << "Done drone start" << endl;
}

char * PyBebop::getFrameBuffer()
{
    // char test[5];
    // return test;
    return m_frame->getRawPointer();
}

void PyBebop::setFlightAltitude()
{
    wscDrone::waitSeconds(5);
    m_drone->getPilot()->moveRelativeMetres(0.0, 0.0, -m_flightAltitude);
    wscDrone::waitSeconds(7);
}

void PyBebop::moveRelativeMetres(float x, float y)
{
    m_drone->getPilot()->moveRelativeMetres(x, y, 0);
}


#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

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
        .def("testFoo",            &PyBebop::testFoo)
        // .def("getBatteryLevel", &PyBebop::getBatteryLevel)
        // .def("setBatteryLevel", &PyBebop::setBatteryLevel)
    ;
}


/********
* API
- Pilot
    - takeoff?
    - land()?

    - moveRelativeMetres()
    - moveDirection()?
    - setHeading()

- DroneController <- not directly used
    - start()
    - stop()

- CameraController
    - setTiltPan()
    - setForward()
    - capturePhoto()

-
*/