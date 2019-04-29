#ifndef PY_BEBOP_H_
#define PY_BEBOP_H_

// #include <vector>


#include <memory> // for shared_ptr
#include <string>

#include "Semaphore.h"
#include "DroneDiscovery.h"
#include "DroneController.h"
#include "CameraControl.h"
#include "Pilot.h"
#include "PyFrame.h"
#include "Utils.h"
#include "VideoDecoder.h"
#include "VideoDriver.h"
#include "VideoFrame.h"

using VideoFrameGeneric = PyFrame; //IS THIS OK STEVE??

/// Provide symbolic names as "callsigns" to identify the drones.
enum class Callsign : unsigned {
    ALPHA      = 0, ///< Low altitude drone
    BRAVO      = 1, ///< Mid-altitude drone
    CHARLIE    = 2, ///< High-altitude drone
    LONE_WOLF  = 3  ///< stand alone drone creating it's own WIFI network
};

class PyBebop {
public:
    /* TODO what does setting = delete do? */

    /// Construct a number of Bebop 2 drone instances
    /// @param numDroneInstances number of drone instances to create (valid range 1-3)
    PyBebop(int callSign);

    /// Default destructor
    ~PyBebop() {}

    void changeCount(int idx, int value);

    /// Drone Movement Control commands
    void takeoffDrone();
    void landDrone();
    void stopDrone();

    char * getFrameBuffer();

    /// Get the current battery level
    /// @returns battery level 0 to 100.
    unsigned getBatteryLevel() { return m_batteryLevel; }

    /// Sets the battery level. This function is used by a callback within the library. Users should not call
    /// it directly.
    void setBatteryLevel(unsigned level) { m_batteryLevel = level; }



    int count[2];

private:
    // Callsign    m_callsign;         ///< callsign of the drone
    std::string m_ipAddress;        ///< ipAddress of the drone under control
    unsigned    m_batteryLevel = 0; ///< battery level, 0 to 100
    int         m_flightAltitude = 0;
    std::shared_ptr<wscDrone::DroneDiscovery>     m_droneDiscovery  = nullptr; ///< shared pointer to DroneDiscovery class
    std::shared_ptr<wscDrone::DroneController>    m_droneController = nullptr; ///< shared pointer to DroneController class
    std::shared_ptr<wscDrone::CameraControl>      m_camera          = nullptr; ///< shared pointer to CameraControl class
    std::shared_ptr<wscDrone::Pilot>              m_pilot           = nullptr; ///< shared pointer to Pilot class
    std::shared_ptr<wscDrone::VideoDriver>        m_video           = nullptr; ///< shared pointer to VideoDriver class
    std::shared_ptr<VideoFrameGeneric>            m_frame           = nullptr; ///< shared pointer to VideoDriver class

    /// Get a shared pointer to the DroneController class
    /// @returns shared pointer to drone controller class
    std::shared_ptr<wscDrone::DroneController> getDroneController() { return m_droneController; }

    /// Get a shared pointer to the CameraControl class
    /// @returns a shared pointer to the camera control class.
    std::shared_ptr<wscDrone::CameraControl>   getCameraControl()   { return m_camera; }

    /// Get a shared pointer to the Pilot class
    /// @returns a shared pointer to the piloting class.
    std::shared_ptr<wscDrone::Pilot>           getPilot()           { return m_pilot; }

    /// Get a shared pointer to the drone state semaphore
    /// @returns a shared pointer to sempahore
    std::shared_ptr<wscDrone::Semaphore>       getStateSemaphore()  { return m_droneController->getStateSemaphore(); }

    /// Get a shared pointer to the VideoDriver class
    /// @returns a shared pointer to the video driver class
    std::shared_ptr<wscDrone::VideoDriver>     getVideoDriver()     { return m_video; }

    void initDrone();
    void startDrone();
    void setFlightAltitude();

    static void m_onCommandReceivedDefault(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData);

    // vector<shared_ptr<Bebop2>>            g_drones;
    // vector<shared_ptr<VideoFrameGeneric>> g_frames;
};



#endif /* PY_BEBOP_H_ */