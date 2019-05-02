/*
 * api.cpp
 *
 * Create on March 27, 2019
 *
 */

// #include <cstdio>
#include <iostream>

#include "Bebop2CtrlIF.h"


using namespace std;
using namespace wscDrone;

const std::string BEBOP_IP_ADDRESS = "192.168.42.1";

using VideoFrameGeneric = Bebop2FrameIF;

#ifdef GIL_HANDLER
Bebop2CtrlIF::Bebop2CtrlIF(int callSign)
: Bebop2CtrlIF::gil_guard(gil_guard::no_acquire)
#else
Bebop2CtrlIF::Bebop2CtrlIF(int callSign)
#endif
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

}

unsigned Bebop2CtrlIF::getBatteryLevel() {
    return m_drone->getBatteryLevel();
}

void Bebop2CtrlIF::takeoffDrone() {
#ifdef GIL_HANDLER
    acquire();
    assert(!owns_gil());
#endif
    cout << "Taking off" << endl;
    m_drone->getPilot()->takeOff();
#ifdef GIL_HANDLER
    restore();
#endif
}

void Bebop2CtrlIF::landDrone()
{
#ifdef GIL_HANDLER
    acquire();
    assert(!owns_gil());
#endif

    m_drone->getCameraControl()->setForward();
    m_drone->getPilot()->land();
#ifdef GIL_HANDLER
    restore();
#endif
}

void Bebop2CtrlIF::killDrone() {
#ifdef GIL_HANDLER
    acquire();
    assert(!owns_gil());
#endif
    m_drone->getPilot()->CUT_THE_MOTORS();
#ifdef GIL_HANDLER
    restore();
#endif
}

void Bebop2CtrlIF::stopDrone()
{
#ifdef GIL_HANDLER
    acquire();
    assert(!owns_gil());
#endif
    landDrone();
    m_drone->getVideoDriver()->stop();
    m_drone->getDroneController()->stop();
#ifdef GIL_HANDLER
    restore();
#endif
}

void Bebop2CtrlIF::moveRelativeMetres(float x, float y)
{
    m_drone->getPilot()->moveRelativeMetres(x, y, 0);
}


void Bebop2CtrlIF::moveDirection(MoveDirection dir)
{
    m_drone->getPilot()->moveDirection(dir);
}

void Bebop2CtrlIF::setHeading(float heading)
{
    m_drone->getPilot()->setHeading(heading);
}

//Camera Functions
void Bebop2CtrlIF::capturePhoto()
{
    m_drone->getCameraControl()->capturePhoto();
}

void Bebop2CtrlIF::setForward()
{
    m_drone->getCameraControl()->setForward();
}

void Bebop2CtrlIF::setTiltPan(float tilt, float pan)
{
    m_drone->getCameraControl()->setTiltPan(tilt, pan);
}

// /*********************/
// /* PRIVATE FUCNTIONS */
// /*********************/

void Bebop2CtrlIF::startDrone()
{
    m_drone->getDroneController()->start();
    cout << "Done CONTROL START" << endl;
    wscDrone::waitSeconds(1);
    m_drone->getVideoDriver()->start();
    cout << "Done VIDEO START" << endl;
    m_drone->getCameraControl()->setForward();
    wscDrone::waitSeconds(1);
    cout << "Done CAMERA start" << endl;
}

char * Bebop2CtrlIF::getFrameBuffer()
{
    return m_frame->getRawPointer();
}

void Bebop2CtrlIF::setFlightAltitude()
{
    wscDrone::waitSeconds(5);
    m_drone->getPilot()->moveRelativeMetres(0.0, 0.0, -m_flightAltitude);
    wscDrone::waitSeconds(7);
}


#define USE_CTYPES

#ifndef USE_CTYPES

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
// #include "Bebop2CtrlIF.h"
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
        .def("getBatteryLevel", &Bebop2CtrlIF::getBatteryLevel)
        // .def("setBatteryLevel", &Bebop2CtrlIF::setBatteryLevel)
    ;
}

#else

extern "C"
{
    Bebop2CtrlIF* CppPyIF(int callSign) { return new Bebop2CtrlIF(callSign); }
    void CppPyIF_takeoffDrone(Bebop2CtrlIF *Bebop2CtrlIF_IF) { Bebop2CtrlIF_IF->takeoffDrone(); }
    void CppPyIF_killDrone(Bebop2CtrlIF *Bebop2CtrlIF_IF) { Bebop2CtrlIF_IF->killDrone(); }
    void CppPyIF_moveRelativeMetres(Bebop2CtrlIF *PyIF, float x, float y) { 
        PyIF->moveRelativeMetres(x, y); 
    }
    void CppPyIF_landDrone(Bebop2CtrlIF *PyIF) { PyIF->landDrone(); } 
    // need to pass wscDrone::MoveDirection enum here
    // void CppPyIF_moveDirection(Bebop2CtrlIF *PyIF, MoveDirection dir) { PyIF->moveDirection(dir); }
    void CppPyIF_setHeading(Bebop2CtrlIF *PyIF, float heading) { PyIF->setHeading(heading); }
    //Camera commands
    void CppPyIF_capturePhoto(Bebop2CtrlIF *PyIF) { PyIF->capturePhoto(); }
    void CppPyIF_setForward(Bebop2CtrlIF *PyIF) { PyIF->setForward(); }
    void CppPyIF_setTiltPan(Bebop2CtrlIF *PyIF, float tilt, float pan) { PyIF->setTiltPan(tilt, pan); }
    char *CppPyIF_getBuff(Bebop2CtrlIF *PyIF) { return PyIF->getFrameBuffer(); }
    //Other commands
    int CppPyIF_getBatteryLevel(Bebop2CtrlIF *PyIF) { return PyIF->getBatteryLevel(); }
}

#endif

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