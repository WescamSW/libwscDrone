/*
 * DroneDiscovery.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <cstdint> // this is missing from ARDiscovery.h
#include <stdexcept>
#include <iostream>

#include "DroneDiscovery.h"

using namespace std;

namespace wscDrone {

constexpr int BEBOP_DISCOVERY_PORT = 44444;

DroneDiscovery::DroneDiscovery(std::string ipAddress)
{
    m_ipAddress = ipAddress;

    eARDISCOVERY_ERROR error = ARDISCOVERY_OK;
    m_discoveryDevice = ARDISCOVERY_Device_New(&error);

    if (error == ARDISCOVERY_OK)
    {
        error = ARDISCOVERY_Device_InitWifi (m_discoveryDevice, ARDISCOVERY_PRODUCT_BEBOP_2, "bebop2", m_ipAddress.c_str(), BEBOP_DISCOVERY_PORT);
    }

    if (error != ARDISCOVERY_OK)
    {
        if (m_discoveryDevice) {
            ARDISCOVERY_Device_Delete(&m_discoveryDevice);
        }
        throw runtime_error("DroneDiscovery(...): failed to create discovery device");
    }
    cout << "DroneDiscovery(): Successfully created" << endl;
}

DroneDiscovery::~DroneDiscovery()
{
    if (m_discoveryDevice) {
        ARDISCOVERY_Device_Delete(&m_discoveryDevice);
    }
}

}

