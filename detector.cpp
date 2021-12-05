#include "detector.h"

Detector::Detector()
{

}

vector<Vec4i> Detector::detectLine(Mat & frame)
{
    vector<vector<Point> > contours;
    Mat reduced_noise;
    int k=0;
    vector<Vec4i> lines;
    Rect temp_rect;



    reduced_noise=reduceNoise(frame);
    //put them in a method
    cv::Mat test = cv::Mat::zeros(cv::Size(160,120), CV_64FC1);

    HoughLinesP(reduced_noise, lines, 6, CV_PI/180, 180, 50, 25  );//50,50,10
    //cout<<"line size "<<lines.size()<<endl;
    return lines;
    /*
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
       // line( reduced_noise, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        line(test, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 3, CV_AA);



    }

    for(int i=0;i<10;i++)
    {
    imshow("test",test);
    waitKey(10);
    }


    findContours(reduced_noise,contours,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);
    vector<Rect> bounded_rects(contours.size());
    vector<Point> contour_poly;
    cout<<"contours size :"<<contours.size()<<endl;
    for(int i=0;i<contours.size();i++){

        approxPolyDP(Mat(contours[i]),contour_poly,3,true);
        temp_rect=boundingRect(Mat(contour_poly));

        if(isAnEfficientRect(temp_rect)){
            bounded_rects[k]=temp_rect;
            k++;

        }
    }
    return bounded_rects;
*/
}


bool Detector::isAnEfficientRect(Rect rect)
{

    if(rect.area()>2500)
        return true;
    return false;
}

Mat Detector::detectByColor(Mat& frame, Scalar s1, Scalar s2)
{
    //detect by color Red
    //31 ,120,54
    //179,255,245
    //detect by color Blue
    //110,50,50
    //130,255,255
    Mat color_filtered;
    Mat hsv_color;
    cvtColor(frame,hsv_color,CV_BGR2HSV);
    inRange(hsv_color,s1,s2,color_filtered);
    return color_filtered;
}

bool Detector::Is_2_Way(Point point)
{
    //if();
}


Mat Detector::reduceNoise(Mat &frame)
{
    Mat blured;
    Mat editted;
    Mat temp;

    GaussianBlur(frame,blured,Size(5,5),20);
    Canny(blured, editted, 50, 200, 3);
    //cvtColor(temp,temp, CV_GRAY2BGR);
   // threshold(frame,editted,100,255,THRESH_BINARY);

    return editted;

}
