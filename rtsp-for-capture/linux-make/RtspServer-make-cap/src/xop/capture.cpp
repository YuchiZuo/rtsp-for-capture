#include "capture.h"
#include "opencv2/opencv.hpp"
CameraCapture::CameraCapture() {
    m_buf = nullptr;
    size = 0;
}

CameraCapture::~CameraCapture() {
    if (m_buf != nullptr) {
        delete[] m_buf;
        m_buf = nullptr;
    }
}

bool CameraCapture::open(int cameraIndex) {
    cap.open(cameraIndex);
    return cap.isOpened();
}

int CameraCapture::captureAndCompress() {
    cv::Mat frame;
    cap.read(frame);

    if (frame.empty())
        return false;

    std::vector<uchar> buf;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(80); // JPEG压缩质量

    cv::imencode(".jpg", frame, buf, params);

    size = buf.size();
    m_buf = new unsigned char[size];
    std::copy(buf.begin(), buf.end(), m_buf);
    displayImage();
    return size;
}

void CameraCapture::displayImage() {
    cv::Mat img = cv::imdecode(cv::Mat(1, size, CV_8UC1, m_buf), cv::IMREAD_COLOR);
    cv::imshow("Camera Image", img);
    cv::waitKey(1);
}

unsigned char* CameraCapture::getImageBuffer() {
    return m_buf;
}

int CameraCapture::getImageSize() {
    return size;
}

int CameraCapture::ReadFrame(char* in_buf) {
    int in_buf_size = captureAndCompress();
    if (in_buf_size < 0) {
        std::cout << "code frame failed";
        return -1;
    }
    memcpy(in_buf, m_buf, in_buf_size);
    
    return in_buf_size;
}
