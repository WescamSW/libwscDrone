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

    registerVideoCallback(m_decoderConfigCallDefault, m_onFrameReceivedDefault, this);
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

void VideoDriver::registerVideoCallback(const VideoDecoderConfigCallback &decoderCallback, const VideoFrameReceivedCallback &videoCallback, void *customData)
{
    if (!m_deviceController) {
        throw runtime_error("VideoDriver::registerStateChangeCallback(): invalid device controller");
    }

    eARCONTROLLER_ERROR error;
    error = ARCONTROLLER_Device_SetVideoStreamCallbacks (m_deviceController, decoderCallback, videoCallback, NULL , customData);

    if (error != ARCONTROLLER_OK) {
        throw runtime_error("VideoDriver::registerStateChangeCallback(): failed to register callback");
    }
}


eARCONTROLLER_ERROR VideoDriver::m_decoderConfigCallDefault(ARCONTROLLER_Stream_Codec_t codec, void *customData)
{
    VideoDriver *videoDriver = (VideoDriver *)(customData);
    printf("Codec update received\n");
    videoDriver->SetH264Params(codec.parameters.h264parameters.spsBuffer, codec.parameters.h264parameters.spsSize,
                  codec.parameters.h264parameters.ppsBuffer, codec.parameters.h264parameters.ppsSize);

    return ARCONTROLLER_OK;
}

// onFrameRecieved
eARCONTROLLER_ERROR VideoDriver::m_onFrameReceivedDefault(ARCONTROLLER_Frame_t *frame, void *customData)
{
    VideoDriver *videoDriver = (VideoDriver *)(customData);

    videoDriver->Decode(frame);
    lock_guard<mutex> lock(*(videoDriver->getBufferMutex()));

    shared_ptr<VideoFrame> videoFrame = videoDriver->getFrame();
    memcpy(videoFrame->getRawPointer(),
        videoDriver->GetFrameRGBRawCstPtr(),
        videoFrame->getFrameSizeBytes());

    return ARCONTROLLER_OK;
}


} // wscDrone
