#ifndef DATATRANSFER_H
#define DATATRANSFER_H
#include <iostream>
#include <qextserialport.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class DataTransfer
{
public:
    DataTransfer();
    void sendData();




private:
    int port;
    String device_name;

};

#endif // DATATRANSFER_H
