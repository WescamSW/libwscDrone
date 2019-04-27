/*
 * DroneController.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <stdexcept>
#include <iostream>
#include <thread>
#include <functional>

#include "DroneController.h"

using namespace std;
using namespace std::placeholders; // for _1, _2, etc.

namespace wscDrone {

DroneController::DroneController(std::shared_ptr<DroneDiscovery> droneDiscovery)
{
    m_stateSemaphore = make_shared<Semaphore>();

    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    m_deviceController = ARCONTROLLER_Device_New(droneDiscovery->getDiscoveryDevice(), &error);

    if ((error != ARCONTROLLER_OK) || (!m_deviceController)) {
        if (m_deviceController) {
            ARCONTROLLER_Device_Delete(&m_deviceController);
        }
        throw runtime_error("DroneController(...): failed to create device controller");
    }

    // Set modes to INDOOR (=0)
    m_deviceController->common->sendWifiSettingsOutdoorSetting(m_deviceController->common, 0);

    registerStateChangeCallback(m_onStateChangedDefault, this);

    cout << "DroneController(): Successfully created" << endl;
}

DroneController::~DroneController()
{
    if (m_deviceController) {
        stop();
        ARCONTROLLER_Device_Delete(&m_deviceController);
    }
}

void DroneController::waitForStateChange()
{
    m_stateSemaphore->wait();
}

void DroneController::notifyStateChange()
{
	m_stateSemaphore->notify();
}

void DroneController::start()
{
    printf("DroneController::start(): starting up drone control\n");
    if (!m_deviceController) {
        throw runtime_error("DroneController::start(): invalid device controller");
    }
    eARCONTROLLER_ERROR error = ARCONTROLLER_Device_Start(m_deviceController);

    if (error != ARCONTROLLER_OK) {
        throw runtime_error("DroneController::start(): failed to start device controller");
    }

    // wait for the state to START
    waitForStateChange();

    // Check to make sure drone is in the running (started) state
    eARCONTROLLER_DEVICE_STATE deviceState = ARCONTROLLER_Device_GetState (m_deviceController, &error);
    if ((error != ARCONTROLLER_OK) || (deviceState != ARCONTROLLER_DEVICE_STATE_RUNNING))
    {
        fprintf(stderr, "DroneController::start():ERROR: Device is not in running (START) state!");
    }
}

void DroneController::stop()
{
    if (!m_deviceController) {
        throw runtime_error("DroneController::stop(): invalid device controller");
    }

    eARCONTROLLER_ERROR error = ARCONTROLLER_OK;
    eARCONTROLLER_DEVICE_STATE state = ARCONTROLLER_Device_GetState(m_deviceController, &error);

    if (error != ARCONTROLLER_OK) {
        throw runtime_error("DroneController::stop(): failed to get device stage");
    }

    if ((error == ARCONTROLLER_OK) && (state != ARCONTROLLER_DEVICE_STATE_STOPPED)) {
        // controller is okay, and state is not already stopped
        error = ARCONTROLLER_Device_Stop(m_deviceController); // stop it
        if (error != ARCONTROLLER_OK) {
            fprintf(stderr, "- error:%s", ARCONTROLLER_Error_ToString(error)); // some error occurred
        }
        waitForStateChange(); // wait for the state to change to stopped
    } // else already stopped
}

// pass the callback to registerCommandReceivedCallback(), the callback must be of the form
// void commandReceived(eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData)
void DroneController::registerCommandReceivedCallback(const CommandReceivedCallback &callback, void *customData)
{
    if (!m_deviceController) {
        throw runtime_error("DroneController::registerCommandReceivedCallback(): invalid device controller");
    }
    eARCONTROLLER_ERROR error = ARCONTROLLER_Device_AddCommandReceivedCallback (m_deviceController, callback, customData);

    if (error != ARCONTROLLER_OK) {
        throw runtime_error("DroneController::registerCommandReceivedCallback(): failed to register callback");
    }
}

// pass the callback to registerStateChangeCallback(), the callback must be of the form
// void stateChanged(eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData)
void DroneController::registerStateChangeCallback(const StateChangeCallback &callback, void *customData)
{
    printf("DroneController::registerStateChangeCallback(): registering...\n");
    if (!m_deviceController) {
        throw runtime_error("DroneController::registerStateChangeCallback(): invalid device controller");
    }

    eARCONTROLLER_ERROR error = ARCONTROLLER_Device_AddStateChangedCallback (m_deviceController, callback, customData);

    if (error != ARCONTROLLER_OK) {
        throw runtime_error("DroneController::registerStateChangeCallback(): failed to register callback");
    }
}

void DroneController::m_onStateChangedDefault(eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData)
{

    printf("stageChanged newState: %d ....\n", newState);
    DroneController *droneController = (DroneController *)(customData); // The custom data is a pointer to the class instance

    droneController->setLastState(newState);
    droneController->notifyStateChange();

    switch (newState)
    {
    case ARCONTROLLER_DEVICE_STATE_STOPPED:
        break;

    case ARCONTROLLER_DEVICE_STATE_RUNNING:
        break;

    default:
        break;
    }
}


} // wscDrone
