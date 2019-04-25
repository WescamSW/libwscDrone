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

VideoDriver::VideoDriver(std::shared_ptr<DroneController> droneController, std::shared_ptr<VideoFrame> frame)
{
    m_deviceController = droneController->getDeviceController();
    if (!m_deviceController) {
        throw runtime_error("VideoDecoder(...): invalid deviceController");
    }

    m_bufferGuard = make_shared<mutex>();
    if (!m_bufferGuard) {
    	throw runtime_error("VideoDecoder(...): failed to create mutex");
    }

    m_frame = frame;
    if (!m_frame) {
    	throw runtime_error("VideoDecoder(...): failed to create frame");
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

void VideoDriver::setFrame(std::shared_ptr<VideoFrame> frame) {
	if (m_frame) {
		m_frame = frame;
	}
}

} // wscDrone
