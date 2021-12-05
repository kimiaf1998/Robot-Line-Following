#ifndef HANDLER_H
#define HANDLER_H
#include<opencv2/opencv.hpp>
#include<qextserialport.h>
#include <iostream>
#include <QObject>
#include <math.h>
#include "data_type.h"
#include "detector.h"
#include "capture.h"
#include "robot.h"

using namespace cv;
using namespace std;
class Prosessor : public QObject
{
    enum DETECTION_TYPE{
        Line,Color
    };

    Q_OBJECT
public:

    explicit Prosessor(Capture* camera,QObject *parent = 0);

    Mat findColorBounds(Mat src);

    void doDetectionOperations(Mat src);

    void drawLine(vector<Rect> bounded_rects, Mat dst);

    void drawLine(vector<Vec4i> lines, Mat dst);

    void handleTheMovment( vector<Rect> rects);

    void handleTheMovment( vector<Vec4i> lines);

    Point findMinimumPoint(Vec4i l,Vec4i l1);

    Point findMaximumPoint(Vec4i l,Vec4i l1);


   // void drawLine()

private:

    void static on_trackbar(int value, void *);

    void static on_trackbar1(int value, void *);

    void static on_trackbar2(int value, void *);

    void static erosion(int value, void* );

    void static dilation(int value, void* );

    bool static myfunction (const Vec4i &v1 ,const Vec4i &v2);

    void static callBackBtn(int event, int x, int y, int flags, void* userdata);

    void makeTrackbar();

    void createBtn();

    bool openSerialPort();

    bool sendData(const QByteArray TAG,const QByteArray data);

    bool isSlopeEqual(Vec4i l1, Vec4i l2);

    bool hasIntersection(Vec4i l1);

    bool isOutOfLine(Point p,Vec4i l,Vec4i l1);

    void erosion_dilation_operation(Mat src);

    Point updateRobotPoint(vector<Vec4i> l1,vector<Vec4i> l2);

    Rect findMaxRect(vector<Rect> rects);

    bool hasTraversed(Point p);

    bool hasTraversedIn1Ways(Vec4i l1);

    bool hasTraversedIn2Ways(Vec4i l1);

    double getLineSize(Vec4i l1);

    bool checkPosition(Point robot_point);

    Capture *camera;

    Robot *robot;

    QTimer *timer;

    QThread *thread;

    QextSerialPort * port;


signals:

    void _SIGNAL_frame();

    void _SIGNAL_move();

    void _SIGNAL_move(double angle,Status);

    void _SIGNAL_showFrame(Mat & frame);

    void _SIGNAL_move_by_qrcode(Status nextMove);

    void _SIGNAL_updateRobotMovement(Point point);






public slots:

    void _SLOT_frameReceiver(Mat &frme);

    void _SLOT_received_data();


};

#endif // HANDLER_H


