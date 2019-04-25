/*
 * Bebop2.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <string>
#include "Callbacks.h"
#include "Bebop2.h"

using namespace std;

namespace wscDrone {

const std::string BEBOP_IP_ADDRESS = "192.168.42.1";

Bebop2::Bebop2(string ipAddress)
{
    m_callsign = Callsign::LONE_WOLF;
    m_ipAddress = ipAddress;
    m_droneDiscovery  = std::make_shared<DroneDiscovery>(m_ipAddress);
    m_droneController = std::make_shared<DroneController>(m_droneDiscovery);
    m_camera          = std::make_shared<CameraControl>(m_droneController);
    m_pilot           = std::make_shared<Pilot>(m_droneController);
    m_video           = std::make_shared<VideoDriver>(m_droneController);

    m_droneController->registerStateChangeCallback(onStateChanged0);
    m_droneController->registerCommandReceivedCallback(onCommandReceived0);
}

Bebop2::Bebop2(Callsign callsign)
{
    m_callsign = callsign;
    std::string ipAddress;
    switch (callsign) {
    case Callsign::ALPHA :
        ipAddress = "192.168.1.101";
        break;
    case Callsign::BRAVO :
        ipAddress = "192.168.1.102";
        break;
    case Callsign::CHARLIE :
        ipAddress = "192.168.1.103";
        break;
    case Callsign::LONE_WOLF :
    default :
        ipAddress = BEBOP_IP_ADDRESS;
        break;
    }
    m_ipAddress = ipAddress;
    m_droneDiscovery  = std::make_shared<DroneDiscovery>(m_ipAddress);
    m_droneController = std::make_shared<DroneController>(m_droneDiscovery);
    m_camera          = std::make_shared<CameraControl>(m_droneController);
    m_pilot           = std::make_shared<Pilot>(m_droneController);
    m_video           = std::make_shared<VideoDriver>(m_droneController);

    switch (callsign) {

    case Callsign::BRAVO :
        m_droneController->registerStateChangeCallback(onStateChanged1);
        m_droneController->registerCommandReceivedCallback(onCommandReceived1);
        break;

    case Callsign::CHARLIE :
        m_droneController->registerStateChangeCallback(onStateChanged2);
        m_droneController->registerCommandReceivedCallback(onCommandReceived2);
        break;

    case Callsign::ALPHA :
    case Callsign::LONE_WOLF :
    default :
        m_droneController->registerStateChangeCallback(onStateChanged0);
        m_droneController->registerCommandReceivedCallback(onCommandReceived0);
        break;
    }
}

void Bebop2::registerVideoCallback(const VideoFrameReceivedCallback &videoCallback)
{

    switch (m_callsign) {

    case Callsign::BRAVO :
        m_droneController->registerVideoCallback(static_cast<unsigned>(Callsign::BRAVO), videoCallback);
        break;

    case Callsign::CHARLIE :
        m_droneController->registerVideoCallback(static_cast<unsigned>(Callsign::CHARLIE), videoCallback);
        break;

    case Callsign::ALPHA :
    case Callsign::LONE_WOLF :
    default :
        m_droneController->registerVideoCallback(static_cast<unsigned>(Callsign::ALPHA), videoCallback);
        break;
    }

}

}
