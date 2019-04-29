/*
 * PyFrame.cpp
 *
 *  Created on: Apr 28, 2019
 *      Author: Eric Dyer
 *
 *  Description: This module is used to handle requests for image data
 *  and provide a generic buffer pointer to that data
 */

#include "PyFrame.h"

PyFrame::PyFrame(unsigned height, unsigned width)
: VideoFrame(height, width)
{
    // m_frame = std::make_shared<cv::Mat>(height,width,CV_8UC3);
    m_height = height;
    m_width  = width;
    const size_t FRAMESIZE = m_width * m_height * 3;
    // m_frame  = std::make_shared<std::array<char, 10>>();
    m_frame  = new char[FRAMESIZE];
}

char *PyFrame::getRawPointer()
{
    return reinterpret_cast<char *>(m_frame);
}

size_t PyFrame::getFrameSizeBytes()
{
    return m_width * m_height * 3;
}

unsigned PyFrame::getHeight() {
    return m_height;
}

unsigned PyFrame::getWidth() {
    return m_width;
}

