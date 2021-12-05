#ifndef QRSCANNER_H
#define QRSCANNER_H
#include<iostream>
#include <opencv2/opencv.hpp>
//#include <zbar.h>
using namespace std;
using namespace cv;
//using namespace zbar;

class QRScanner
{

public:

    typedef struct{

        String data;
        String type;
        vector<Point> location;
    }decoded_object;

    QRScanner();

    //void decode(Mat &src,vector<decoded_object> decoded_objs);

    //void display(Mat &im, vector<decoded_object>&decodedObjects);

private:
};

#endif // QRSCANNER_H
