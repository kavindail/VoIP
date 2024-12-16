#include "client.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

int main() {
  const int facetimeCamera = 0;
  const int continuityCamera = 1;
  const int numOfFramesToCatpure = 10000;
  const int compressionQuality = 10; // 0 to 100
  const int windowWidth = 400;
  const int windowLength = 250;
  const int port = 54999;
  cv::VideoCapture cap(facetimeCamera);
  std::cout << "Sending to port: " << port << std::endl;

  if (!cap.isOpened()) {
    std::cerr << "Failed to open webcam\n";
    return -1;
  }

  cv::Mat frame, decodedFrame;
  cv::namedWindow("Webcam", cv::WINDOW_NORMAL);

  for (int i = 0; i < numOfFramesToCatpure; i++) {
    cap >> frame;
    if (frame.empty())
      break;

    std::vector<uchar> encoded;
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(compressionQuality);

    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, cv::Size(windowWidth, windowLength));
    cv::imencode(".jpg", resizedFrame, encoded, compression_params);

    std::cout << "Frame size: " << encoded.size() << std::endl;

    sendDataToSocket(reinterpret_cast<const char *>(encoded.data()),
                     encoded.size(), port);

    decodedFrame = cv::imdecode(encoded, cv::IMREAD_COLOR);
    cv::imshow("Webcam", decodedFrame);

    if (cv::waitKey(100) == 27)
      break;
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
