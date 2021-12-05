#include "capture.h"

Capture::Capture(int port, QObject *parent) : QObject(parent)
{

    this->timer=new QTimer();
    this->port=port;

    for(int i=0;i<5;i++)
        windows[i]=" ";

    connect(timer,SIGNAL(timeout()),this,SLOT(_SLOT_frameReceiver()));
    //connect(this,SIGNAL(_SIGNAL_showFrame(Mat &)),this,SLOT(_SLOT_showFrame(Mat&)));

}


bool Capture::turnCameraOn()
{
    //cap.open(port);
    cap.open(port);
    //cap.set()
    timer->start(2);
    if(cap.isOpened()){
        cout<<"camera is opened"<<endl;
        return true;}
    return false;
}
int Capture::getPort() const
{
    return port;
}



void Capture::displayFrame(String window_name, Mat frame)
{
    if(!this->checkExistWindow(window_name))
        namedWindow(window_name,WINDOW_NORMAL);

    imshow(window_name,frame);
    waitKey(3);

}

void Capture::_SLOT_frameReceiver()
{
    cap>>frame;
    emit _SIGNAL_frame(frame);
    //displayFrame("Image",frame);



    //emit _SIGNAL_showFrame(frame);

}

void Capture::_SLOT_showFrame(Mat &frame)
{
    cout<<"display image"<<endl;
    displayFrame("Image",frame);

}

bool Capture::checkExistWindow(String name)
{
    for(int i=0;i<5;i++){
        if(windows[i].compare(name)==0)
            return true;
    }
    return false;
}
Mat Capture::getFrame() const
{
    return frame;
}

QThread *Capture::getThread() const
{
    return thread;
}





