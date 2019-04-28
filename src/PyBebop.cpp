/*
 * api.cpp
 *
 * Create on March 27, 2019
 *
 */

// #include <cstdio>
#include <iostream>

#include <boost/python.hpp>

#include "PyBebop.h"

using namespace std;

const std::string BEBOP_IP_ADDRESS = "192.168.42.1";

PyBebop::PyBebop(int callSign)
{
    /* Assign IP Address */
    std::string ipAddress;
    switch (callSign) {
    case 0 :
        ipAddress = "192.168.1.101";
        m_flightAltitude = 1;
        cout << "101" << endl;
        break;
    case 1 :
        ipAddress = "192.168.1.102";
        m_flightAltitude = 2;
        cout << "102" << endl;
        break;
    case 2 :
        ipAddress = "192.168.1.103";
        m_flightAltitude = 3;
        break;
    case 3 :
        ipAddress = BEBOP_IP_ADDRESS;
        m_flightAltitude = 1;
        break;
    default :
        cout << "Done drone init" << endl;
        break;
    }



    /* Create child classes */
    m_ipAddress = ipAddress;

    m_droneDiscovery  = std::make_shared<wscDrone::DroneDiscovery>(m_ipAddress);
    m_droneController = std::make_shared<wscDrone::DroneController>(m_droneDiscovery);
    m_camera          = std::make_shared<wscDrone::CameraControl>(m_droneController);
    m_pilot           = std::make_shared<wscDrone::Pilot>(m_droneController);
    // m_video           = std::make_shared<VideoDriver>(m_droneController, frame);

    m_droneController->registerCommandReceivedCallback(m_onCommandReceivedDefault, this);



    initDrone();
    startDrone();

    // for (int i = 0; i < g_drones.size(); i++) {
    //     startDrone(i);
    // }

    count[0] = 0;
    count[1] = 0;
}

void PyBebop::changeCount(int idx, int value)
{
    count[idx]  = value;

    printf("Changed %d: %d\n", idx, count[idx]);
}

void PyBebop::takeoffDrone() {
    m_pilot->takeOff();
    wscDrone::waitSeconds(3);
    setFlightAltitude();
}

void PyBebop::landDrone()
{
    m_camera->setForward();
    m_pilot->land();
    wscDrone::waitSeconds(5);
}

void PyBebop::stopDrone()
{
    landDrone();
    m_video->stop();
    m_droneController->stop();
}

// /*********************/
// /* PRIVATE FUCNTIONS */
// /*********************/
void PyBebop::initDrone()
{
//     if (numberOfDrones < 1 || numberOfDrones > 4) {
//         cout << "\nYou must specify a number of drones between 1 and 3" << endl;
//         exit(EXIT_SUCCESS);
//     }

//     if (numberOfDrones >= 1) {
//         g_frames.emplace_back(make_shared<VideoFrameGeneric>(BEBOP2_STREAM_HEIGHT, BEBOP2_STREAM_WIDTH));
//         g_drones.emplace_back(make_shared<Bebop2>(Callsign::ALPHA, g_frames[0]));
//     }

//     if (numberOfDrones >= 2) {
//         g_frames.emplace_back(make_shared<VideoFrameGeneric>(BEBOP2_STREAM_HEIGHT, BEBOP2_STREAM_WIDTH));
//         g_drones.emplace_back(make_shared<Bebop2>(Callsign::LONE_WOLF, g_frames[1]));
//     }

//     if (numberOfDrones >= 3) {
//         g_frames.emplace_back(make_shared<VideoFrameGeneric>(BEBOP2_STREAM_HEIGHT, BEBOP2_STREAM_WIDTH));
//         g_drones.emplace_back(make_shared<Bebop2>(Callsign::BRAVO, g_frames[2]));
//     }

//     if (numberOfDrones >= 4) {
//         g_frames.emplace_back(make_shared<VideoFrameGeneric>(BEBOP2_STREAM_HEIGHT, BEBOP2_STREAM_WIDTH));
//         g_drones.emplace_back(make_shared<Bebop2>(Callsign::CHARLIE, g_frames[3]));
//     }
    cout << "Done drone init" << endl;
}

void PyBebop::startDrone()
{

//     ControlPtr       control = g_drones[droneId]->getDroneController();
//     CameraControlPtr camera  = g_drones[droneId]->getCameraControl();
//     PilotPtr         pilot   = g_drones[droneId]->getPilot();
//     VideoDriverPtr   video   = g_drones[droneId]->getVideoDriver();

//     control->start();
//     cout << "Done CONTROL START" << endl;
//     waitSeconds(1);
//     video->start();
//     cout << "Done VIDEO START" << endl;
//     camera->setForward();
//     waitSeconds(1);
    cout << "Done drone start" << endl;
}

void PyBebop::setFlightAltitude()
{
    wscDrone::waitSeconds(5);
    m_pilot->moveRelativeMetres(0.0, 0.0, -m_flightAltitude);
    wscDrone::waitSeconds(7);
}

// called when a command has been received from the drone
void PyBebop::m_onCommandReceivedDefault(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData)
{
    PyBebop *drone = (PyBebop *)(customData);

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


using namespace boost::python;

BOOST_PYTHON_MODULE(libwscDrone)
{
    class_<PyBebop>("PyBebop", init<int>())
        // .def("changeCount", &PyBebop::PyBebop)
        .def("changeCount", &PyBebop::changeCount)
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