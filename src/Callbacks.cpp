/*
 * Callbacks.cpp
 *
 *  Created on: Feb 8, 2019
 *      Author: slascos
 */
#include <memory>
#include <iostream>
#include <unistd.h>

#include "Semaphore.h"
#include "Bebop2.h"
#include "Callbacks.h"
#include "VideoFrame.h"

using namespace std;
using namespace wscDrone;

// Global variables
//extern std::vector<std::shared_ptr<Semaphore>>  g_stateSemaphores;
extern std::vector<std::shared_ptr<Bebop2>>     g_drones;
//extern std::vector<std::unique_ptr<std::mutex>> g_bufferGuards;
//extern std::vector<shared_ptr<VideoFrame>>      g_frames;

namespace wscDrone {

// Device stage changed handler
// called when the state of the device controller has changed
void onStateChangedGeneric(eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData, int droneId)
{

    printf("stageChanged newState: %d ....\n", newState);

    g_drones[droneId]->getDroneController()->setLastState(newState);
    g_drones[droneId]->getDroneController()->notifyStateChange();
    switch (newState)
    {
    case ARCONTROLLER_DEVICE_STATE_STOPPED:
        //g_stateSemaphores[droneId]->notify();

        break;

    case ARCONTROLLER_DEVICE_STATE_RUNNING:
        //g_stateSemaphores[droneId]->notify();
        break;

    default:
        break;
    }
}
void onStateChanged0 (eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData)
{
    onStateChangedGeneric(newState, error, customData, 0);
}
void onStateChanged1 (eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData)
{
    onStateChangedGeneric(newState, error, customData, 1);
}
void onStateChanged2 (eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData)
{
    onStateChangedGeneric(newState, error, customData, 2);
}

// called when a command has been received from the drone
void onCommandReceivedGeneric(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData, int droneId)
{
    if (elementDictionary != NULL)
    {
        // if the command received is a battery state changed
        if (commandKey == ARCONTROLLER_DICTIONARY_KEY_COMMON_COMMONSTATE_BATTERYSTATECHANGED)
        {
            ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
            ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;

            // get the command received in the device controller
            HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
            if (element != NULL)
            {
                // get the value
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_COMMONSTATE_BATTERYSTATECHANGED_PERCENT, arg);

                if (arg != NULL)
                {
                    uint8_t batteryLevel = arg->value.U8;
                    g_drones[droneId]->setBatteryLevel(batteryLevel);
                    // do what you want with the battery level
                }
            }
        }


        // Print camera settings
        else  if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED) && (elementDictionary != NULL))
        {
            ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
            ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;
            HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
            if (element != NULL)
            {
                cout << "CAMERA INFO:" << endl;
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_FOV, arg);
                if (arg != NULL)
                {
                    float fov = arg->value.Float;
                    printf("fov:%f\n", fov);
                }
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_PANMAX, arg);
                if (arg != NULL)
                {
                    float panMax = arg->value.Float;
                    printf("panMax:%f\n", panMax);
                }
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_PANMIN, arg);
                if (arg != NULL)
                {
                    float panMin = arg->value.Float;
                    printf("panMin:%f\n", panMin);
                }
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_TILTMAX, arg);
                if (arg != NULL)
                {
                    float tiltMax = arg->value.Float;
                    printf("tiltMax:%f\n", tiltMax);
                }
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_CAMERASETTINGSSTATE_CAMERASETTINGSCHANGED_TILTMIN, arg);
                if (arg != NULL)
                {
                    float tiltMin = arg->value.Float;
                    printf("tiltMin:%f\n", tiltMin);
                }
            }
        }

        // Move ended
        else  if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PILOTINGEVENT_MOVEBYEND) && (elementDictionary != NULL))
        {
            printf("MoveBy complete\n");
            g_drones[droneId]->getPilot()->notifyMoveComplete();
        }

        // Flying state
        else  if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED) && (elementDictionary != NULL))
        {
            ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
            ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;
            HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
            if (element != NULL)
            {
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PILOTINGSTATE_FLYINGSTATECHANGED_STATE, arg);
                if (arg != NULL)
                {
                    int flyingState = arg->value.I32;
                    g_drones[droneId]->getPilot()->setFlyingState(flyingState);
                }
            }
        }

        // PictureFormatChanged
        if ((commandKey == ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PICTURESETTINGSSTATE_PICTUREFORMATCHANGED) && (elementDictionary != NULL))
        {
            ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
            ARCONTROLLER_DICTIONARY_ELEMENT_t *element = NULL;
            HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, element);
            if (element != NULL)
            {
                HASH_FIND_STR (element->arguments, ARCONTROLLER_DICTIONARY_KEY_ARDRONE3_PICTURESETTINGSSTATE_PICTUREFORMATCHANGED_TYPE, arg);
                if (arg != NULL)
                {
                    eARCOMMANDS_ARDRONE3_PICTURESETTINGSSTATE_PICTUREFORMATCHANGED_TYPE type =
                            (eARCOMMANDS_ARDRONE3_PICTURESETTINGSSTATE_PICTUREFORMATCHANGED_TYPE)(arg->value.I32);
                    std::cout << "PICTURE_FORMAT_CHANGED: " << type << std::endl;
                }
            }
        }
    }
}
void onCommandReceived0(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData)
{
    onCommandReceivedGeneric(commandKey, elementDictionary, customData, 0);
}
void onCommandReceived1(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData)
{
    onCommandReceivedGeneric(commandKey, elementDictionary, customData, 1);
}
void onCommandReceived2(eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData)
{
    onCommandReceivedGeneric(commandKey, elementDictionary, customData, 2);
}

// decoderConfigCallback
eARCONTROLLER_ERROR decoderConfigCallbackGeneric(ARCONTROLLER_Stream_Codec_t codec, void *customData, int droneId)
{
    printf("Codec update received\n");
    g_drones[droneId]->getVideoDriver()->SetH264Params(codec.parameters.h264parameters.spsBuffer, codec.parameters.h264parameters.spsSize,
                  codec.parameters.h264parameters.ppsBuffer, codec.parameters.h264parameters.ppsSize);

    return ARCONTROLLER_OK;
}
eARCONTROLLER_ERROR decoderConfigCallback0(ARCONTROLLER_Stream_Codec_t codec, void *customData)
{
    return decoderConfigCallbackGeneric(codec, customData, 0);
}
eARCONTROLLER_ERROR decoderConfigCallback1(ARCONTROLLER_Stream_Codec_t codec, void *customData)
{
    return decoderConfigCallbackGeneric(codec, customData, 1);
}
eARCONTROLLER_ERROR decoderConfigCallback2(ARCONTROLLER_Stream_Codec_t codec, void *customData)
{
    return decoderConfigCallbackGeneric(codec, customData, 2);
}

// onFrameRecieved
eARCONTROLLER_ERROR onFrameReceivedGeneric(ARCONTROLLER_Frame_t *frame, void *customData, int droneId)
{
    g_drones[droneId]->getVideoDriver()->Decode(frame);
    lock_guard<mutex> lock(*(g_drones[droneId]->getVideoDriver()->getBufferMutex()));

    shared_ptr<VideoFrame> videoFrame = g_drones[droneId]->getVideoDriver()->getFrame();
    memcpy(videoFrame->getRawPointer(),
    	g_drones[droneId]->getVideoDriver()->GetFrameRGBRawCstPtr(),
		   videoFrame->getFrameSizeBytes());

    return ARCONTROLLER_OK;
}

eARCONTROLLER_ERROR onFrameReceived0(ARCONTROLLER_Frame_t *frame, void *customData)
{
    return onFrameReceivedGeneric(frame, customData, 0);
}
eARCONTROLLER_ERROR onFrameReceived1(ARCONTROLLER_Frame_t *frame, void *customData)
{
    return onFrameReceivedGeneric(frame, customData, 1);
}
eARCONTROLLER_ERROR onFrameReceived2(ARCONTROLLER_Frame_t *frame, void *customData)
{
    return onFrameReceivedGeneric(frame, customData, 2);
}

} // wscDrone
