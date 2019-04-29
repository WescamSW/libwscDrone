/*
 * Bebop2.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <string>
#include <iostream>
#include "Bebop2.h"

using namespace std;

// Set the default flight altitudes for the drones
constexpr float ALPHA_FLIGHT_ALTITUDE     = 1.0f;
constexpr float BRAVO_FLIGHT_ALTITUDE     = 1.0f;
constexpr float CHARLIE_FLIGHT_ALTITUDE   = 1.0f;
constexpr float LONE_WOLF_FLIGHT_ALTITUDE = 1.0f;

namespace wscDrone {

const std::string BEBOP_IP_ADDRESS = "192.168.42.1";

Bebop2::Bebop2(string ipAddress, std::shared_ptr<VideoFrame> frame)
{
    m_callsign = Callsign::LONE_WOLF;
    m_ipAddress = ipAddress;
    m_droneDiscovery  = std::make_shared<DroneDiscovery>(m_ipAddress);
    m_droneController = std::make_shared<DroneController>(m_droneDiscovery);
    m_camera          = std::make_shared<CameraControl>(m_droneController, PhotoType::RAW);
    m_pilot           = std::make_shared<Pilot>(m_droneController, 1.0f); // initial height = 1.0 meteres
    m_video           = std::make_shared<VideoDriver>(m_droneController, frame);

    m_droneController->registerCommandReceivedCallback(m_onCommandReceivedDefault, this);
}

Bebop2::Bebop2(Callsign callsign, std::shared_ptr<VideoFrame> frame)
{
    m_callsign = callsign;
    PhotoType photoType;
    float initialFlightAltitude;

    std::string ipAddress;
    switch (callsign) {
    case Callsign::ALPHA :
        ipAddress = "192.168.1.101";
        photoType = PhotoType::RAW;
        initialFlightAltitude = ALPHA_FLIGHT_ALTITUDE;
        break;
    case Callsign::BRAVO :
        ipAddress = "192.168.1.102";
        photoType = PhotoType::JPEG;
        initialFlightAltitude = BRAVO_FLIGHT_ALTITUDE;
        break;
    case Callsign::CHARLIE :
        ipAddress = "192.168.1.103";
        photoType = PhotoType::RAW;
        initialFlightAltitude = CHARLIE_FLIGHT_ALTITUDE;
        break;
    case Callsign::LONE_WOLF :
    default :
        ipAddress = BEBOP_IP_ADDRESS;
        photoType = PhotoType::RAW;
        initialFlightAltitude = LONE_WOLF_FLIGHT_ALTITUDE;
        break;
    }
    m_ipAddress = ipAddress;
    m_droneDiscovery  = std::make_shared<DroneDiscovery>(m_ipAddress);
    m_droneController = std::make_shared<DroneController>(m_droneDiscovery);
    m_camera          = std::make_shared<CameraControl>(m_droneController, photoType);
    m_pilot           = std::make_shared<Pilot>(m_droneController, initialFlightAltitude);
    m_video           = std::make_shared<VideoDriver>(m_droneController, frame);

    m_droneController->registerCommandReceivedCallback(m_onCommandReceivedDefault, this);
}

// called when a command has been received from the drone
void Bebop2::m_onCommandReceivedDefault(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData)
{
    Bebop2 *drone = (Bebop2 *)(customData);

    if (elementDictionary != NULL)
    {
        // if the command received is a battery state changed
        if (commandKey == ARCONTROLLER_DICTIONARY_KEY_COMMON_COMMONSTATE_BATTERYSTATECHANGED)
        {
            ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
            ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;

            // get the command received in the device controller
            HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
            if (element != NULL)
            {
                // get the value
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_COMMONSTATE_BATTERYSTATECHANGED_PERCENT, arg);

                if (arg != NULL)
                {
                    uint8_t batteryLevel = arg->value.U8;
                    drone->setBatteryLevel(batteryLevel);
                    // do what you want with the battery level
                }
            }
        }

        // Camera orientation changed
//        if (commandKey == ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_CAMERASTATE_ORIENTATIONV2)
//        {
//            cout << "Camera orientation done" << endl;
//            drone->getCameraControl()->notifyCameraChange();
//        }


        // Print camera settings
        else  if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED) && (elementDictionary != NULL))
        {
            ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
            ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;
            HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
            if (element != NULL)
            {
                cout << "CAMERA INFO:" << endl;
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_FOV, arg);
                if (arg != NULL)
                {
                    float fov = arg->value.Float;
                    printf("fov:%f\n", fov);
                }
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_PANMAX, arg);
                if (arg != NULL)
                {
                    float panMax = arg->value.Float;
                    printf("panMax:%f\n", panMax);
                }
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_PANMIN, arg);
                if (arg != NULL)
                {
                    float panMin = arg->value.Float;
                    printf("panMin:%f\n", panMin);
                }
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_TILTMAX, arg);
                if (arg != NULL)
                {
                    float tiltMax = arg->value.Float;
                    printf("tiltMax:%f\n", tiltMax);
                }
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_TILTMIN, arg);
                if (arg != NULL)
                {
                    float tiltMin = arg->value.Float;
                    printf("tiltMin:%f\n", tiltMin);
                }
            }
        }

        // Move ended
        else  if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PILOTINGEVENT_MOVEBYEND) && (elementDictionary != NULL))
        {
            printf("MoveBy complete\n");
            drone->getPilot()->notifyMoveComplete();
        }

        // Flying state
        else  if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED) && (elementDictionary != NULL))
        {
            ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
            ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;
            HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
            if (element != NULL)
            {
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE, arg);
                if (arg != NULL)
                {
                    int flyingState = arg->value.I32;
                    drone->getPilot()->setFlyingState(flyingState);
                }
            }
        }

        // PictureFormatChanged
        if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PICTURESETTINGSSTATE_PICTUREFORMATCHANGED) && (elementDictionary != NULL))
        {
            ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
            ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;
            HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
            if (element != NULL)
            {
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PICTURESETTINGSSTATE_PICTUREFORMATCHANGED_TYPE, arg);
                if (arg != NULL)
                {
                    eARCOMMANDS_ARDRONE3_PICTURESETTINGSSTATE_PICTUREFORMATCHANGED_TYPE type =
                            (eARCOMMANDS_ARDRONE3_PICTURESETTINGSSTATE_PICTUREFORMATCHANGED_TYPE)(arg->value.I32);
                    std::cout << "PICTURE_FORMAT_CHANGED: " << type << std::endl;
                }
            }
        }
    }
}


}
