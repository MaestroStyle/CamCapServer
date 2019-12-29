#include "CameraTransmitter.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CameraTransmitter w;
    w.show();
//    w.startCapture(0);
//    cv::VideoCapture *cap = new cv::VideoCapture(0);
////    cap.open(0);
//    cv::Mat frame;
//    while(true){
//        cap->read(frame);
//        cv::imshow("Live", frame);
//        if(cv::waitKey(0) >= 0){
//            break;
//        }
//    }
//    cv::destroyAllWindows();
    return a.exec();
}
