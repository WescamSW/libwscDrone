/*
 * Utils.cpp
 *
 *  Created on: Feb 7, 2019
 *      Author: slascos
 */
#include <thread>
#include "Utils.h"

namespace wscDrone {

void waitSeconds(int seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void waitMilliseconds(int milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

float degressToRadians(float degrees)
{
    return ((PI_F / 180.0f) * degrees);
}

} // wscDrone


