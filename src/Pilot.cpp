/*
 * piloting.c
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <chrono>
#include <stdexcept>
#include "Utils.h"
#include "Pilot.h"

using namespace std;
using namespace chrono_literals;
namespace wscDrone {

constexpr int PILOT_TIMEOUT_MS = 10000;

Pilot::Pilot(std::shared_ptr<DroneController> droneController)
{
    m_deviceController = droneController->getDeviceController();
    if (!m_deviceController) {
        throw runtime_error("Pilot(...): invalid deviceController");
    }
}

void Pilot::notifyMoveComplete()
{
    moveSem.notify();
}

bool Pilot::waitMoveComplete() {
    return !moveSem.waitTimed(PILOT_TIMEOUT_MS);
}

eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE Pilot::getFlyingStateRaw()
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

    FlyingState flyingState = getFlyingState();
    //if (getFlyingState() == ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_LANDED)
    if (flyingState == FlyingState::LANDED)
    {
        printf("takeOff(): PILOTING: Sending takeoff command\n");
        m_deviceController->aRDrone3->sendPilotingTakeOff(m_deviceController->aRDrone3);

        // wait until state is hovering
        waitMilliseconds(5);
        while (m_flyingState != FlyingState::HOVERING) {
            waitMilliseconds(5);
        }
    } else {
        printf("takeOff(): Can't takeoff, drone is not in LANDED state\n");
    }
}

void Pilot::land()
{
    if (!m_deviceController) {
        throw runtime_error("Pilot::land(): invalid device controller");
    }

    //eARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE flyingState = getFlyingState();
    //FlyingState flyingState = getFlyingState();
    //if (flyingState == ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_FLYING || flyingState == ARCOMMANDS_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE_HOVERING)

    // Send land unconditionally to avoid any risk of the command being ignored by a coding bug
    {
        printf("land(): PILOTING: Sending the landing command\n");
        m_deviceController->aRDrone3->sendPilotingLanding(m_deviceController->aRDrone3);
    }
}

// NOTE:sendPilotingMoveBy expects radians
bool Pilot::moveRelativeMetres(float dx, float dy, float dz, float heading, bool wait)
{
    bool timedOut = true;

    m_deviceController->aRDrone3->sendPilotingMoveBy(m_deviceController->aRDrone3, dx, dy, dz, degressToRadians(heading)); // not implemented in the SDK yet
    if (wait) {
        timedOut = waitMoveComplete();
    }
    // wait until out of flying state, should go to hovering
    while (m_flyingState != FlyingState::HOVERING) {
    	waitMilliseconds(5);
    }

    // Return true on success, false on timeout.
    return !timedOut;
}

void Pilot::moveDirection(MoveDirection dir)
{
    switch(dir) {
        case MoveDirection::UP:
            // NOTE: -negative numbers mean increase altitude!
            moveRelativeMetres(0.0f, 0.0f, -MOVEMENT_STEP);
            break;
        case MoveDirection::DOWN:
            moveRelativeMetres(0.0f, 0.0f, MOVEMENT_STEP);
            break;
        case MoveDirection::FORWARD:
            moveRelativeMetres(MOVEMENT_STEP, 0.0f, 0.0f);
            break;
        case MoveDirection::BACK:
            moveRelativeMetres(-MOVEMENT_STEP, 0.0f, 0.0f);
            break;
        case MoveDirection::RIGHT:
            moveRelativeMetres(0.0f, MOVEMENT_STEP, 0.0f);
            break;
        case MoveDirection::LEFT:
            moveRelativeMetres(0.0f, -MOVEMENT_STEP, 0.0f);
            break;
        default:
            break;
    }
}

void Pilot::setHeading(float heading)
{
    moveRelativeMetres(0.0f, 0.0f, 0.0f, heading);
    //waitSeconds(5); // the finish move event will trigger before the heading is complete so we must force a wait.
}

void Pilot::setFlyingState(int state, bool debug)
{ 
    FlyingState temp = static_cast<FlyingState>(state);

    if (debug) {
	    switch(temp) {
		case FlyingState::HOVERING:
			printf("change to Hovering State \n");
			break;
		case FlyingState::FLYING:
			printf("change to Flying State \n");
			break;
		case FlyingState::TAKING_OFF:
			printf("change to TakeOFF State\n");
			break;
		case FlyingState::LANDING:
			printf("change to Landing State \n");
			break;
		case FlyingState::LANDED:
			printf("change to Landed State\n");
			break;
		case FlyingState::EMERGENCY:
			printf("change to Emergency State, Drone Landing.\n");
			break;
		case FlyingState::MOTOR_SPINUP:
			printf("change to Spinup State, Drone Motors Spining up.\n");
			break;
		default:
			printf("ERROR: Unhandled State=%d\n", state);
			break;
	    }
    }
    m_flyingState = temp;
}

}
