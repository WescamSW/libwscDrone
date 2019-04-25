/****************************************************************************//**
 * @file
 * @brief This file contains a VideoDriver class to interact with the video
 * system on the Bebop 2 drone.
 * @ingroup wscDrone wscDrone
 * @author Steve Lascos
 * @date Feb. 3, 2019
 * @copyright CONFIDENTIAL and PROPRIETARY to L3 Technologies Wescam. This
 * source code is copyrighted. The source code may not be copied, reproduced,
 * translated, or reduced to any electronic medium or machine-readable form
 * without the prior written consent of L-3 Wescam. This source code is
 * confidential and proprietary to L-3 Wescam and may not be reproduced,
 * published, or disclosed to others without company authorization.
 ******************************************************************************/

#ifndef VIDEODRIVER_H_
#define VIDEODRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>

#ifdef __cplusplus
}
#endif

#include "DroneController.h"
#include "VideoDecoder.h"

namespace wscDrone {

/// This driver extends the VideoDecoder class created from ROS (Robot Operating System).
class VideoDriver : public bebop_driver::VideoDecoder {
public:
    VideoDriver() = delete;
    /// Construct a VideoDriver with an associated DroneController
    /// @param droneController smart pointer to a DroneController instance
    VideoDriver(std::shared_ptr<wscDrone::DroneController> droneController);
    ~VideoDriver(); ///< default destructor

    /// Starts the H.264 video streaming interface on the drone
    void start();
    /// Stops the H.264 video streaming interface on the drone
    void stop();

private:
    ARCONTROLLER_Device_t *m_deviceController = nullptr; ///< raw pointer to the ARSDK device controller
};

}

#endif /* VIDEODRIVER_H_ */
