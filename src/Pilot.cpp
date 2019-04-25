/*
 * piloting.c
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <stdexcept>
#include "Utils.h"
#include "Pilot.h"

using namespace std;

namespace wscDrone {

Pilot::Pilot(std::shared_ptr<DroneController> droneController)
{
    m_deviceController = droneController->getDeviceController();
    if (!m_deviceController) {
        throw runtime_error("Pilot(...): invalid deviceController");
    }
}

eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE Pilot::getFlyingState()
{
    if (!m_deviceController) {
        throw runtime_error("Pilot::getFlyingState(): invalid device controller");
    }

    eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE flyingState = ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_MAX;
    eARCONTROLLER_ERROR error;
    ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary = ARCONTROLLER_ARDrone3_GetCommandElements(m_deviceController->aRDrone3, ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED, &error);
    if (error == ARCONTROLLER_OK && elementDictionary != NULL)
    {
        ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
        ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;
        HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
        if (element != NULL)
        {
            // Get the value
            HASH_FIND_STR(element->arguments, ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE, arg);
            if (arg != NULL)
            {
                // Enums are stored as I32
                flyingState = static_cast<eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE>(arg->value.I32);
            }
        }
    } else {
        fprintf(stderr, "getFlyingState: ERROR getting element dictionary\n");
    }
    return flyingState;
}

void Pilot::takeOff()
{
    if (!m_deviceController) {
        throw runtime_error("Pilot::takeOff(): invalid device controller");
    }

    if (getFlyingState() == ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_LANDED)
    {
        printf("takeOff(): PILOTING: Sending takeoff command\n");
        m_deviceController->aRDrone3->sendPilotingTakeOff(m_deviceController->aRDrone3);
    } else {
        printf("takeOff(): Can't takeoff, drone is not in LANDED state\n");
    }
}

void Pilot::land()
{
    if (!m_deviceController) {
        throw runtime_error("Pilot::land(): invalid device controller");
    }

    eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE flyingState = getFlyingState();
    if (flyingState == ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_FLYING || flyingState == ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_HOVERING)
    {
        printf("land(): PILOTING: Sending the landing command\n");
        m_deviceController->aRDrone3->sendPilotingLanding(m_deviceController->aRDrone3);
    }
}

// NOTE:sendPilotingMoveBy expects radians
void Pilot::moveRelativeMetres(float dx, float dy, float dz, float heading, bool wait)
{
    m_deviceController->aRDrone3->sendPilotingMoveBy(m_deviceController->aRDrone3, dx, dy, dz, degressToRadians(heading)); // not implemented in the SDK yet
    if (wait) {
        moveSem.wait();
    }
    // wait until out of flying state, should go to hovering
    while (m_flyingState == FlyingState::FLYING) {}
}

void Pilot::setHeading(float heading)
{
    moveRelativeMetres(0.0f, 0.0f, 0.0f, heading);
    //waitSeconds(5); // the finish move event will trigger before the heading is complete so we must force a wait.
}

void Pilot::setFlyingState(int state)
{
    m_flyingState = static_cast<FlyingState>(state);
}

}
