#ifndef CAPTURE_H
#define CAPTURE_H
#include<opencv2/opencv.hpp>
#include<iostream>
#include <QObject>
#include <QThread>
#include <QTimer>
using namespace cv;
using namespace std;
class Capture : public QObject
{
    Q_OBJECT
public:
    explicit Capture(int port,QObject *parent = 0);

    bool turnCameraOn();

    int getPort() const;

    QThread *getThread() const;

    Mat getFrame() const;


private:
    int port;

    void displayFrame(String window_name,Mat frame);

    bool checkExistWindow(String name);

    String windows[5];

    VideoCapture cap;

    QTimer *timer;

    QThread *thread;

    Mat frame;


signals:

    void _SIGNAL_frame(Mat &frame);


public slots:

    void _SLOT_frameReceiver();//get frame and emit signal display

    void _SLOT_showFrame(Mat &frame);//displayframe in window

};


#endif // CAPTURE_H
