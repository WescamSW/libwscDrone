/*
 * Callbacks.h
 *
 *  Created on: Feb 8, 2019
 *      Author: slascos
 */

#ifndef INC_CALLBACKS_H_
#define INC_CALLBACKS_H_

#include "DroneController.h"

namespace wscDrone {

// Callback prototypes
void onStateChanged0(eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData);
void onStateChanged1(eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData);
void onStateChanged2(eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData);

void onCommandReceived0(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData);
void onCommandReceived1(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData);
void onCommandReceived2(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData);

eARCONTROLLER_ERROR decoderConfigCallback0(ARCONTROLLER_Stream_Codec_t codec, void *customData);
eARCONTROLLER_ERROR decoderConfigCallback1(ARCONTROLLER_Stream_Codec_t codec, void *customData);
eARCONTROLLER_ERROR decoderConfigCallback2(ARCONTROLLER_Stream_Codec_t codec, void *customData);

eARCONTROLLER_ERROR onFrameReceived0(ARCONTROLLER_Frame_t *frame, void *customData);
eARCONTROLLER_ERROR onFrameReceived1(ARCONTROLLER_Frame_t *frame, void *customData);
eARCONTROLLER_ERROR onFrameReceived2(ARCONTROLLER_Frame_t *frame, void *customData);

}

#endif /* INC_CALLBACKS_H_ */
