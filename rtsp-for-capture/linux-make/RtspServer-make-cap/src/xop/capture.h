#ifndef CAMERA_CAPTURE_H
#define CAMERA_CAPTURE_H

#include "opencv2/opencv.hpp"

class CameraCapture {
private:
    cv::VideoCapture cap;
    unsigned char* m_buf;
    int size;

public:
    CameraCapture();
    ~CameraCapture();

    bool open(int cameraIndex);
    int captureAndCompress();
    void displayImage();
    unsigned char* getImageBuffer();
    int getImageSize();
    int ReadFrame(char* in_buf);
};

#endif // CAMERA_CAPTURE_H
