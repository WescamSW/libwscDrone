/*
 * VideoCallback.h
 *
 *  Created on: Apr 9, 2019
 *      Author: slascos
 */

#ifndef DEPS_EL_PL_LIBWSCDRONE_GIT_INC_VIDEOCALLBACK_H_
#define DEPS_EL_PL_LIBWSCDRONE_GIT_INC_VIDEOCALLBACK_H_

#include "DroneController.h"

namespace wscDrone {

eARCONTROLLER_ERROR onFrameReceived0(ARCONTROLLER_Frame_t *frame, void *customData);
eARCONTROLLER_ERROR onFrameReceived1(ARCONTROLLER_Frame_t *frame, void *customData);
eARCONTROLLER_ERROR onFrameReceived2(ARCONTROLLER_Frame_t *frame, void *customData);

}

#endif /* DEPS_EL_PL_LIBWSCDRONE_GIT_INC_VIDEOCALLBACK_H_ */
