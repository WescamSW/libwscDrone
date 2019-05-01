/*
 * CameraControl.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <iostream>
#include <stdexcept>
#include "Utils.h"
#include "CameraControl.h"

using namespace std;

namespace wscDrone {

constexpr int PHOTO_TIMEOUT_MS = 10000;

CameraControl::CameraControl(std::shared_ptr<DroneController> droneController)
{
    m_deviceController = droneController->getDeviceController();
    if (!m_deviceController) {
        throw runtime_error("CameraControl(...): invalid deviceController");
    }
}

void CameraControl::setTiltPan(float tilt, float pan)
{
    if (!m_deviceController) {
        throw runtime_error("CameraControl::setTiltPan(): invalid device controller");
    }
    m_deviceController->aRDrone3->sendCameraOrientationV2(m_deviceController->aRDrone3, tilt, pan);
}

void CameraControl::setForward(void)
{
    if (!m_deviceController) {
        throw runtime_error("CameraControl::setForward(): invalid device controller");
    }
    m_deviceController->aRDrone3->sendCameraOrientationV2(m_deviceController->aRDrone3, 0.0f, 0.0f);
    waitMilliseconds(300);
}

void CameraControl::setPhotoType(PhotoType photoType)
{
    switch(photoType)
    {
    case PhotoType::RAW :
        m_deviceController->aRDrone3->sendPictureSettingsPictureFormatSelection(
                m_deviceController->aRDrone3, ARCOMMANDS_ARDRONE3_PICTURESETTINGS_PICTUREFORMATSELECTION_TYPE_RAW);
        break;

    case PhotoType::JPEG_4_3 :
        m_deviceController->aRDrone3->sendPictureSettingsPictureFormatSelection(
                m_deviceController->aRDrone3, ARCOMMANDS_ARDRONE3_PICTURESETTINGS_PICTUREFORMATSELECTION_TYPE_JPEG);
        break;

    case PhotoType::SNAPSHOT :
        m_deviceController->aRDrone3->sendPictureSettingsPictureFormatSelection(
                m_deviceController->aRDrone3, ARCOMMANDS_ARDRONE3_PICTURESETTINGS_PICTUREFORMATSELECTION_TYPE_SNAPSHOT);
        break;

    case PhotoType::FISHEYE :
        m_deviceController->aRDrone3->sendPictureSettingsPictureFormatSelection(
                m_deviceController->aRDrone3, ARCOMMANDS_ARDRONE3_PICTURESETTINGS_PICTUREFORMATSELECTION_TYPE_JPEG_FISHEYE);
        break;
    }
}

void CameraControl::capturePhoto()
{
    if (!m_deviceController) {
        throw runtime_error("CameraControl::setForward(): invalid device controller");
    }

    printf("CameraControl::capturePhoto is stopping video stream to take a photo\n");
    ARCONTROLLER_Device_StopVideoStream(m_deviceController);

    m_deviceController->aRDrone3->sendMediaRecordPictureV2(m_deviceController->aRDrone3);
    m_waitPhotoComplete(); // Block until photo complete
}

void CameraControl::m_notifyPhotoComplete()
{
    m_photoSem.notify();
}

bool CameraControl::m_waitPhotoComplete() {

    bool timeout = !m_photoSem.waitTimed(PHOTO_TIMEOUT_MS);

    printf("CameraControl::m_waitPhotoComplete() is re-starting video stream after completing a photo\n");
    ARCONTROLLER_Device_StartVideoStream(m_deviceController);

    return timeout;
}

void CameraControl::setCameraState(CameraState cameraState)
{
    if ((m_photoInProgress==false) && (cameraState==CameraState::BUSY)) {
        m_photoInProgress = true;
    }
    else if ((m_photoInProgress==true) && (cameraState==CameraState::READY)) {
        m_photoInProgress = false;
        m_notifyPhotoComplete();
    }
    m_cameraState = cameraState;
}

} // wscDrone

