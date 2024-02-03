#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "client.h"
#include <vector>

int main() {
  int  facetimeCamera = 0;
  int continuityCamera = 1; 
  int numOfFramesToCatpure = 10000;
    cv::VideoCapture cap(0); 
    if (!cap.isOpened()) {
        std::cerr << "Failed to open webcam\n";
        return -1;
    }

    cv::Mat frame, decodedFrame;
    cv::namedWindow("Webcam", cv::WINDOW_NORMAL);



for(int i = 0; i < numOfFramesToCatpure; i++) {
    cap >> frame; 

    if (frame.empty()) break; 

    std::vector<uchar> encoded; 
    std::vector<int> compression_params; 
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY); 
    compression_params.push_back(10); // 0 - 100

    cv::Mat resizedFrame; 
    cv::resize(frame, resizedFrame, cv::Size(400, 300)); 
    cv::imencode(".jpg", resizedFrame, encoded, compression_params); 

    std::cout << "Encoded frame size in bytes: " << encoded.size() << std::endl;

    sendDataToSocket(reinterpret_cast<const char*>(encoded.data()), encoded.size()); 

    std::cout << "Frame end" << std::endl;

    decodedFrame = cv::imdecode(encoded, cv::IMREAD_COLOR);
    cv::imshow("Webcam", decodedFrame); 

    if (cv::waitKey(30) == 27) break; 
}

    cap.release();
    cv::destroyAllWindows();

    return 0;
}
