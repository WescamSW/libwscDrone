/*
 * VideoDecoder.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <stdexcept>
#include <iostream>

#include "../inc/VideoDriver.h"
#include "DroneController.h"

using namespace std;

namespace wscDrone {

VideoDriver::VideoDriver(std::shared_ptr<DroneController> droneController)
{
    m_deviceController = droneController->getDeviceController();
    if (!m_deviceController) {
        throw runtime_error("VideoDecoder(...): invalid deviceController");
    }
}

VideoDriver::~VideoDriver()
{
}

void VideoDriver::start()
{
    printf("VideoDecoder::start() is starting video stream\n");
    ARCONTROLLER_Device_StartVideoStream(m_deviceController);
}

void VideoDriver::stop()
{
    printf("VideoDecoder::stop() is stopping video stream\n");
    ARCONTROLLER_Device_StopVideoStream(m_deviceController);
}

} // wscDrone
