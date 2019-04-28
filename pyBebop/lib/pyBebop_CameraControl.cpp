/*
 * CameraControl.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: slascos
 */
#include <stdexcept>
#include "Utils.h"
#include "CameraControl.h"

using namespace std;

constexpr int CAMERA_TIMEOUT_MS = 5000;

namespace wscDrone {

CameraControl::CameraControl(std::shared_ptr<DroneController> droneController)
{
    m_deviceController = droneController->getDeviceController();
    if (!m_deviceController) {
        throw runtime_error("CameraControl(...): invalid deviceController");
    }

    //m_cameraSemaphore = make_shared<Semaphore>();
//    m_deviceController->aRDrone3->sendPictureSettingsPictureFormatSelection(
//            m_deviceController->aRDrone3, ARCOMMANDS_ARDRONE3_PICTURESETTINGS_PICTUREFORMATSELECTION_TYPE_RAW);
}

//bool CameraControl::waitForCameraChange()
//{
//    return !m_cameraSemaphore->waitTimed(CAMERA_TIMEOUT_MS);
//}
//
//void CameraControl::notifyCameraChange()
//{
//    m_cameraSemaphore->notify();
//}

void CameraControl::setTiltPan(float tilt, float pan)
{
    if (!m_deviceController) {
        throw runtime_error("CameraControl::setTiltPan(): invalid device controller");
    }
    m_deviceController->aRDrone3->sendCameraOrientationV2(m_deviceController->aRDrone3, tilt, pan);
    //waitForCameraChange();
}

void CameraControl::setForward(void)
{
    if (!m_deviceController) {
        throw runtime_error("CameraControl::setForward(): invalid device controller");
    }
    m_deviceController->aRDrone3->sendCameraOrientationV2(m_deviceController->aRDrone3, 0.0f, 0.0f);
    //waitForCameraChange();
}

void CameraControl::capturePhoto()
{
    if (!m_deviceController) {
        throw runtime_error("CameraControl::setForward(): invalid device controller");
    }
    m_deviceController->aRDrone3->sendPictureSettingsPictureFormatSelection(
            m_deviceController->aRDrone3, ARCOMMANDS_ARDRONE3_PICTURESETTINGS_PICTUREFORMATSELECTION_TYPE_RAW);
    waitSeconds(1);
    m_deviceController->aRDrone3->sendMediaRecordPictureV2(m_deviceController->aRDrone3);
}

} // wscDrone

