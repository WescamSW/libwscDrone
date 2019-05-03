/*
 * Bebop2FrameIF.h
 *
 *  Created on: Apr 28, 2019
 *      Author: Eric Dyer
 */

#ifndef SRC_VIDEOOPENCV_H_
#define SRC_VIDEOOPENCV_H_

#include <memory>
// #include <thread>
// #include <opencv2/core.hpp>
#include "wscDrone/VideoFrame.h"

class Bebop2FrameIF : public VideoFrame {
public:
    Bebop2FrameIF() = delete;
    Bebop2FrameIF(unsigned height, unsigned width);
    Bebop2FrameIF(unsigned height, unsigned width, const uint8_t * frame);
    ~Bebop2FrameIF() {}

    unsigned getHeight() override;
    unsigned getWidth() override;

    char *getRawPointer() override;
    size_t getFrameSizeBytes() override;
    // void justDisplay();
    // std::shared_ptr<cv::Mat> getFrame() { return m_frame; }

    // Derived Class functions

    // static std::shared_ptr<std::thread> launchDisplayThread();
private:
    char * m_frame = nullptr;
    unsigned m_height;
    unsigned m_width;
};


#endif /* SRC_VIDEOOPENCV_H_ */