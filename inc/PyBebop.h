#ifndef PY_BEBOP_H_
#define PY_BEBOP_H_

// #include <vector>


#include <memory> // for shared_ptr
#include <string>

#include "Bebop2.h"
#include "DroneDiscovery.h"
#include "DroneController.h"
#include "CameraControl.h"
#include "Pilot.h"
#include "PyFrame.h"
#include "Semaphore.h"
#include "Utils.h"
#include "VideoDecoder.h"
#include "VideoDriver.h"
#include "VideoFrame.h"
// #include "wscDrone.h"
// #include "../inc/gilHandler.hpp"

using VideoFrameGeneric = PyFrame; //IS THIS OK STEVE??

#ifdef GIL_HANDLER
class PyBebop : public gil_guard 
#else
class PyBebop 
#endif
{
public:
    /* TODO what does setting = delete do? */

    /// Construct a number of Bebop 2 drone instances
    /// @param numDroneInstances number of drone instances to create (valid range 1-3)
    PyBebop(int callSign);

    /// Default destructor
    ~PyBebop() {}

    /// Thread Mgmt by GIL
    // gil_guard gil(gil_guard::no_acquire_t);
    /// Drone Movement Control commands
    void takeoffDrone();
    void moveRelativeMetres(float x, float y);
    void landDrone();
    void stopDrone();
    void killDrone();
    unsigned getBatteryLevel();

    char * getFrameBuffer();

    void testFoo();
private:
    // Callsign    m_callsign;         ///< callsign of the drone
    std::string m_ipAddress;        ///< ipAddress of the drone under control
    unsigned    m_batteryLevel = 0; ///< battery level, 0 to 100
    int         m_flightAltitude = 0;
    // std::shared_ptr<wscDrone::DroneDiscovery>     m_droneDiscovery  = nullptr; ///< shared pointer to DroneDiscovery class
    // std::shared_ptr<wscDrone::DroneController>    m_droneController = nullptr; ///< shared pointer to DroneController class
    // std::shared_ptr<wscDrone::CameraControl>      m_camera          = nullptr; ///< shared pointer to CameraControl class
    // std::shared_ptr<wscDrone::Pilot>              m_pilot           = nullptr; ///< shared pointer to Pilot class
    // std::shared_ptr<wscDrone::VideoDriver>        m_video           = nullptr; ///< shared pointer to VideoDriver class
    std::shared_ptr<VideoFrameGeneric> m_frame = nullptr; ///< shared pointer to VideoDriver class
    std::shared_ptr<wscDrone::Bebop2>  m_drone = nullptr; ///< shared pointer to Bebop2 class


    void initDrone();
    void startDrone();
    void setFlightAltitude();
};



#endif /* PY_BEBOP_H_ */