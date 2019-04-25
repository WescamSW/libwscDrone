/****************************************************************************//**
 * @file
 * @brief This file contains the DroneController class for accesing basic
 * device information on the Bebop 2 drone.
 * @ingroup wscDrone wscDrone
 * @author Steve Lascos
 * @date Feb. 3, 2019
 * @copyright CONFIDENTIAL and PROPRIETARY to L3 Technologies Wescam. This
 * source code is copyrighted. The source code may not be copied, reproduced,
 * translated, or reduced to any electronic medium or machine-readable form
 * without the prior written consent of L-3 Wescam. This source code is
 * confidential and proprietary to L-3 Wescam and may not be reproduced,
 * published, or disclosed to others without company authorization.
 ******************************************************************************/
#ifndef DRONECONTROLLER_H_
#define DRONECONTROLLER_H_

#include <unistd.h>
#include <functional>
#include <memory>


#ifdef __cplusplus
extern "C" {
#endif

#include <libARDiscovery/ARDiscovery.h>
#include <libARController/ARController.h>

#ifdef __cplusplus
}
#endif


#include "Semaphore.h"
#include "DroneDiscovery.h"

namespace wscDrone {

/// alias for ARSDK3 ARCONTROLLER_Device_StateChangedCallback_t
using StateChangeCallback     = ARCONTROLLER_Device_StateChangedCallback_t;

/// alias for ARSDK3 ARCONTROLLER_DICTIONARY_CALLBACK_t
using CommandReceivedCallback = ARCONTROLLER_DICTIONARY_CALLBACK_t;

/// alias for ARSDK3 ARCONTROLLER_Stream_DecoderConfigCallback_t
using VideoDecoderConfigCallback = ARCONTROLLER_Stream_DecoderConfigCallback_t;

/// alias for ARSDK3 ARCONTROLLER_Stream_DidReceiveFrameCallback_t
using VideoFrameReceivedCallback = ARCONTROLLER_Stream_DidReceiveFrameCallback_t;

/// The DroneController class wraps a DroneDiscovery class for mapping IP addresses
/// to particular drone istances and permits callback registration.
class DroneController {
public:
    DroneController() = delete;
    /// Constructor requires an instance of DroneDiscovery
    /// @param droneDiscovery smart pointer to an instance of DroneDiscovery
    DroneController(std::shared_ptr<DroneDiscovery> droneDiscovery);
    ~DroneController();

    /// get a raw ARSD3 pointer to the associated device controller
    /// @returns raw pointer to ARSD3 device controller
    ARCONTROLLER_Device_t *getDeviceController() { return m_deviceController; }

    /// Register the user callback for processing received commands
    /// @param callback the function name of the callback
    void registerCommandReceivedCallback(const CommandReceivedCallback &callback);

    /// Register the user callback for processing received state changes
    /// @param callback the function name of the callback
    void registerStateChangeCallback(const StateChangeCallback &callback);

    /// Register the user callback for processing received H264 video frames. Both decoder and video
    /// callbacks must be registered at the same time.
    /// @param decoderCallback the function name of the decoder callback
    /// @param videoCallback the function name of the video callback
    //void registerVideoCallbacks(const VideoDecoderConfigCallback &decoderCallback, const VideoFrameReceivedCallback &videoCallback);
    void registerVideoCallback(unsigned callbackId, const VideoFrameReceivedCallback &videoCallback);

    /// Get a smart pointer to the drone's state semaphore.
    /// @returns smart pointer to a Semaphore
    std::shared_ptr<Semaphore> getStateSemaphore() { return m_stateSemaphore; }

    /// Blocking call to busy wait until a state change occurs
    void waitForStateChange();

    /// Get the last received state event of the drone
    /// @returns the last state received
    eARCONTROLLER_DEVICE_STATE getLastState() { return m_lastState; }

    /// Start the drones device control interface
    void start();

    /// Stop the drones device control interface
    void stop();

private:
    std::shared_ptr<Semaphore> m_stateSemaphore   = nullptr; ///< smart pointer to the state semaphore
    ARCONTROLLER_Device_t *    m_deviceController = nullptr; ///< smart pointer to the device controller
    eARCONTROLLER_DEVICE_STATE m_lastState;                  ///< smart pointer to the last state update
};

} // wscDrone

#endif /* DRONECONTROLLER_H_ */
