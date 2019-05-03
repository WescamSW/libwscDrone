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

#ifdef GIL_HANDLER
PyBebop::PyBebop(int callSign)
: PyBebop::gil_guard(gil_guard::no_acquire)
#else
PyBebop::PyBebop(int callSign)
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

unsigned PyBebop::getBatteryLevel() {
    return m_drone->getBatteryLevel();
}

void PyBebop::takeoffDrone() {
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

void PyBebop::landDrone()
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

void PyBebop::killDrone() {
#ifdef GIL_HANDLER
    acquire();
    assert(!owns_gil());
#endif
    m_drone->getPilot()->CUT_THE_MOTORS();
#ifdef GIL_HANDLER
    restore();
#endif
}

void PyBebop::stopDrone()
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
    cout << "Done CAMERA start" << endl;
}

char * PyBebop::getFrameBuffer()
{
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

void PyBebop::moveDirection(MoveDirection dir)
{
    m_drone->getPilot()->moveDirection(dir);
}

void PyBebop::setHeading(float heading)
{
    m_drone->getPilot()->setHeading(heading);
}

//Camera Functions
void PyBebop::capturePhoto()
{
    m_drone->getCameraControl()->capturePhoto();
}

void PyBebop::setForward()
{
    m_drone->getCameraControl()->setForward();
}

void PyBebop::setTiltPan(float tilt, float pan)
{
    m_drone->getCameraControl()->setTiltPan(tilt, pan);
}

extern "C"
{
    PyBebop* CppPyIF(int callSign) { return new PyBebop(callSign); }
    void CppPyIF_takeoffDrone(PyBebop *PyBebop_IF) { PyBebop_IF->takeoffDrone(); }
    void CppPyIF_killDrone(PyBebop *PyBebop_IF) { PyBebop_IF->killDrone(); }
    void CppPyIF_moveRelativeMetres(PyBebop *PyIF, float x, float y) { 
        PyIF->moveRelativeMetres(x, y); 
    }
    void CppPyIF_landDrone(PyBebop *PyIF) { PyIF->landDrone(); } 
    // need to pass wscDrone::MoveDirection enum here
    // void CppPyIF_moveDirection(PyBebop *PyIF, MoveDirection dir) { PyIF->moveDirection(dir); }
    void CppPyIF_setHeading(PyBebop *PyIF, float heading) { PyIF->setHeading(heading); }
    //Camera commands
    void CppPyIF_capturePhoto(PyBebop *PyIF) { PyIF->capturePhoto(); }
    void CppPyIF_setForward(PyBebop *PyIF) { PyIF->setForward(); }
    void CppPyIF_setTiltPan(PyBebop *PyIF, float tilt, float pan) { PyIF->setTiltPan(tilt, pan); }
    char *CppPyIF_getBuff(PyBebop *PyIF) { return PyIF->getFrameBuffer(); }
    //Other commands
    int CppPyIF_getBatteryLevel(PyBebop *PyIF) { return PyIF->getBatteryLevel(); }
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