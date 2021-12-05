#include "capture.h"
#include "detector.h"
#include "robot.h"
#include<processor.h>
#include<QCoreApplication>
#include <qthread.h>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc,argv);

    QThread thread;
    Capture *camera;
    cout<<"here";
    camera=new Capture(0);
//    camera->moveToThread(&thread);
//    thread.start();
    cout<<"there";
    Prosessor *prosessor=new Prosessor(camera);


    return a.exec();
}
