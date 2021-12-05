#ifndef DETECTOR_H
#define DETECTOR_H

#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

class Detector
{
public:

    Detector();

    static vector<Vec4i> detectLine(Mat & frame );

    static Mat detectByColor( Mat &frame ,Scalar s1,Scalar s2 );

    bool Is_2_Way(Point point);


private:

    static Mat reduceNoise(Mat &frame);//gray//gaussian//canny//

    static bool isAnEfficientRect(Rect rect);



};

#endif // DETECTOR_H
