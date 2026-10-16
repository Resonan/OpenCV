#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    std::cout << "OpenCV Version: " << CV_VERSION << std::endl;

    // Create a simple blank image and put text on it
    cv::Mat img = cv::Mat::zeros(300, 600, CV_8UC3);
    cv::putText(img, "Conan + OpenCV Works!", cv::Point(30, 150),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

    // Display the image in a window
    cv::imshow("Conan OpenCV Window", img);
    cv::waitKey(0);

    return 0;
}