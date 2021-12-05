
#include "processor.h"
#define PORT 1
#define H_SLIDER_MAX 179
#define SV_SLIDER_MAX 255
#define MAX_ELEMENT  2
#define MAX_KERNEL_SIZE  21

#define CAMERA_CENTER_X 320
#define CAMERA_CENTER_Y 240

#define DELTA_SIZE 40
#define PORT_NAME "/dev/ttySAC0"

//640*480

int h_slider=0;
int s_slider=0;
int v_slider=0;
int h_slider1=0;
int s_slider1=0;
int v_slider1=0;

int erosion_elem = 0;
int erosion_size = 11;
int dilation_elem = 0;
int dilation_size = 11;
Mat frame;
Point robot_p=Point(0,0);
int counter =0 ;



struct myclass {
    bool operator() (Vec4i v1 , Vec4i v2) {

        int delta_x1;
        int delta_x2;
        int delta_y1;
        int delta_y2;

        delta_x1=v1[0]-v1[2];
        delta_y1=v1[1]-v1[3];

        delta_x2=v2[0]-v2[2];
        delta_y2=v2[1]-v2[3];

        double distnce1=sqrt(delta_x1*delta_x1+delta_y1*delta_y1);
        double distnce2=sqrt(delta_x2*delta_x2+delta_y2*delta_y2);

        return ( distnce1  < distnce2);
    }
}myobject;

Prosessor::Prosessor(Capture* camera, QObject *parent) : QObject(parent)
{

    this->robot=new Robot();
    this->camera=camera;
    this->camera->turnCameraOn();
    this->port = new QextSerialPort(PORT_NAME, QextSerialPort::EventDriven);//"/dev/ttyUSB0"

    //    this->thread=new QThread();
    //    this->timer=new QTimer();

    connect(this->camera,SIGNAL(_SIGNAL_frame(Mat &)),this,SLOT(_SLOT_frameReceiver(Mat &)));
    connect(this,SIGNAL(_SIGNAL_move()),this->robot,SLOT(_SLOT_move()));
    connect(this,SIGNAL(_SIGNAL_move(double,Status)),this->robot,SLOT(_SLOT_move(double,Status)));
    connect(this,SIGNAL(_SIGNAL_updateRobotMovement(Point)),this->robot,SLOT(_SLOT_updateRobotMovement(Point)));
    connect(this,SIGNAL(_SIGNAL_showFrame(Mat &)),this->camera,SLOT(_SLOT_showFrame(Mat&)));
    connect(this,SIGNAL(_SIGNAL_move_by_qrcode(Status)),this->robot ,SLOT(_SLOT_move_by_qrcode(Status)));
    connect(port,SIGNAL(bytesWritten(qint64)),this,SLOT(_SLOT_received_data()));
    /*
    timer->moveToThread(thread);
    thread->start();
    timer->start(2);*/

}

Mat Prosessor::findColorBounds(Mat src)
{

    frame=src.clone();

    Mat detected_color=Detector()
            .detectByColor(frame,Scalar(79,181,26),Scalar(110,255,255));

    //imshow("RED Detector",detected_color);

    return detected_color;
}

void Prosessor::makeTrackbar()
{

    namedWindow("RED Detector",WINDOW_NORMAL);
    createTrackbar("Lower H","RED Detector",&h_slider,H_SLIDER_MAX,on_trackbar);
    createTrackbar("Lower S","RED Detector",&s_slider,SV_SLIDER_MAX,on_trackbar);
    createTrackbar("Lower V","RED Detector",&v_slider,SV_SLIDER_MAX,on_trackbar);

    //if need we will call suiatable function instead of on_trackbar

    createTrackbar("Higher H","RED Detector",&h_slider1,H_SLIDER_MAX,on_trackbar);
    createTrackbar("Higher S","RED Detector",&s_slider1,SV_SLIDER_MAX,on_trackbar);
    createTrackbar("Higher V","RED Detector",&v_slider1,SV_SLIDER_MAX,on_trackbar1);


    namedWindow("Erode & Dilate ",WINDOW_NORMAL);
    createTrackbar("Erosion Element :\n 0: Rect \n 1: Cross \n 2: Ellipse","Erode & Dilate ",
                   &erosion_elem,MAX_ELEMENT,erosion);
    createTrackbar("Kernel Size","Erode & Dilate ",&erosion_size,MAX_KERNEL_SIZE,erosion);
    createTrackbar("Dilation Element :\n 0: Rect \n 1: Cross \n 2: Ellipse","Erode & Dilate ",
                   &dilation_elem,MAX_ELEMENT,dilation);
    createTrackbar("KernelSize","Erode & Dilate ",&dilation_size,MAX_KERNEL_SIZE,dilation);
}

bool Prosessor::openSerialPort()
{

    port->setPortName(PORT_NAME);
    port->setBaudRate(BAUD9600);
    port->setDataBits(DATA_8);
    port->setParity(PAR_NONE);
    port->setStopBits(STOP_1);
    port->setFlowControl(FLOW_OFF);

    if (port->open(QIODevice::ReadWrite))
    {
        //Connected
         qDebug() << "device opened sucessfully" << endl;
        return true;
    }
    else
    {
        //Open error

        qDebug() << "device failed to open:" << port->errorString() <<endl;
        return false;
    }
}

bool Prosessor::sendData(QByteArray TAG, QByteArray data)
{
    if(openSerialPort()){
        port->write(TAG,TAG.size());//you must know at first the type of data
        port->write(data,data.size());//send data

        return true;
    }
    return false;

}

void Prosessor::doDetectionOperations(Mat src)
{

    vector<Vec4i> detected_lines;

    detected_lines=Detector().detectLine(src);

    handleTheMovment(detected_lines);

    //drawLine(detected_lines,this->camera->getFrame());


    //circle(this->camera->getFrame(),Point(CAMERA_CENTER_X,CAMERA_CENTER_Y),10,Scalar( 20 , 104, 80),2);



}

void Prosessor::_SLOT_frameReceiver(Mat & frme)
{
    frame=frme.clone();

    int start_s=clock();
    erosion(erosion_size,0);
    dilation(dilation_size,0);

    Mat detected_color=findColorBounds(frame);//first detect color then pass it to line detector

    doDetectionOperations(detected_color);

    int stop_s=clock();
    cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;

    //sending data via qexteerialport
    double time=(stop_s-start_s)/double(CLOCKS_PER_SEC)*1000;
    sendData(QByteArray("Time"),QByteArray(reinterpret_cast<const char*>(&time), sizeof(time)));


}

void Prosessor::_SLOT_received_data()
{
    qDebug()<<"Data Recieved"<<endl;
}

void Prosessor::drawLine(vector<Rect> bounded_rects, Mat dst)
{
    Rect rect;

    for(int i=0;i<bounded_rects.size();i++){
        rect=bounded_rects[i];
        if(!rect.empty()){
            rectangle(dst,rect.tl(),rect.br(),Scalar(200,50,150),2);

        }
    }
}

void Prosessor::drawLine(vector<Vec4i> lines, Mat dst)
{

    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,110,255), 3, CV_AA);

    }
}

void Prosessor::handleTheMovment(vector<Vec4i> lines)
{
    Point robot_point=Point(0,0);

    bool has_intersection=false;
    int min_x;
    int max_x;
    counter ++;
    if(robot->canMove(lines)){

        sort(lines.begin(),lines.end(),myfunction);

        //check the intersection

        for(int i=lines.size()-1;i>=0;i--)
        {

            Vec4i l = lines[i];
            if(hasIntersection(l) ){
                if(!hasTraversedIn2Ways(l)){

                    cout<<"**  Has Intersection  **"<<endl;
                    has_intersection=true;
                    emit _SIGNAL_move_by_qrcode(TURN_LEFT);
                    break;
                }
                else{
                    lines.erase(lines.begin() + i);
                }

            }
        }
        if(!has_intersection)
            for(int i=lines.size()-1;i>=0;i--)
            {

                Vec4i l = lines[i];
                Vec4i l1 = lines[i-1];


                if(isSlopeEqual(l,l1) ){
                    //cout<<"--- 2 line's slope is equal ---"<<endl;
                    robot_point=Point((l[0]+l[2]+l1[0]+l1[2])/4
                            ,(l[1]+l[3]+l1[1]+l1[3])/4);
                    //qDebug()<<"Robot Point Y Test : "<<robot_point.y<<endl;
                    if(!isOutOfLine(robot_point,l,l1)){
                        if(!hasTraversed(robot_point)){

                            //if(counter<70) {
                            //line(this->camera->getFrame(),Point(CAMERA_CENTER_X,CAMERA_CENTER_Y),robot_point,Scalar(150,100,260),3);
                            //emit _SIGNAL_updateRobotMovement(robot_point);
                            //  emit _SIGNAL_move();
                            //}else
                                //if(checkPosition(robot_point)){
                                    //cout<<"Position is right"<<endl;
                                    //line(this->camera->getFrame(),Point(CAMERA_CENTER_X,CAMERA_CENTER_Y),robot_point,Scalar(150,200,160),3);
                                    emit _SIGNAL_updateRobotMovement(robot_point);
                                    emit _SIGNAL_move();

                                    //break;
                                //}

                               // cout<<"Position not right"<<endl;

                        }
                        else
                        {
                            if(robot_point.x - CAMERA_CENTER_X <10 &&
                                    robot_point.x - CAMERA_CENTER_X >-10)
                                emit _SIGNAL_move(0,AHEAD);
                            else if(robot_point.x - CAMERA_CENTER_X < -10)
                                emit _SIGNAL_move(0,TURN_LEFT);
                            else
                                emit _SIGNAL_move(0,TURN_RIGHT);

                        }
                    }

                }

            }

    }
    else
        qDebug()<<"******    Robot Has To Stop .    ********"<<endl<<"There is No Line!"<<endl;

}

bool Prosessor::hasTraversed(Point p)
{

    if(CAMERA_CENTER_Y -p.y < 10 /*&&
                            CAMERA_CENTER_Y -p.y > -120*/)
        return true;
    return false;

}

bool Prosessor::hasTraversedIn1Ways(Vec4i l1)
{

    int delta_x;
    delta_x=l1[0]-l1[2];
    if(delta_x< DELTA_SIZE && delta_x > -DELTA_SIZE )
        return true;
    return false;


}

bool Prosessor::hasTraversedIn2Ways(Vec4i l1)
{
    int y=(l1[1]+l1[3])/2;

    if(y > CAMERA_CENTER_Y -50)
        return true;

    return false;
}

bool Prosessor::isSlopeEqual( Vec4i l1,  Vec4i l2)
{
    double m1=0;
    double m2=0;


    Point start_pt1= Point(l1[0], l1[1]);
    Point end_pt1  = Point(l1[2], l1[3]);

    int delta_x1=start_pt1.x - end_pt1.x;
    if(delta_x1 !=0)
        m1 = abs((start_pt1.y - end_pt1.y)/(delta_x1));

    Point start_pt2= Point(l2[0], l2[1]);
    Point end_pt2  = Point(l2[2], l2[3]);

    int delta_x2=start_pt2.x - end_pt2.x;

    if(delta_x2 !=0)
        m2 = abs((start_pt2.y - end_pt2.y)/(delta_x2));

    if(delta_x2 ==0 && delta_x1==0)
        return true;

    return m1==m2;

}

bool Prosessor::hasIntersection(Vec4i l1)
{

    int y1,y2;

    y1=l1[1];
    y2=l1[3];

    int delta_y =y2-y1;
    return (delta_y > -DELTA_SIZE && delta_y < DELTA_SIZE);
}

bool Prosessor::isOutOfLine(Point p, Vec4i l, Vec4i l1)
{
    int max_x=findMaximumPoint(l,l1).x;
    int min_x=findMinimumPoint(l,l1).x;
    if(p.x>max_x+5 || p.x <min_x-5)
        return true;
    return false;



}

bool Prosessor::myfunction (const Vec4i &v1 ,const Vec4i &v2) {

    int delta_x1;
    int delta_x2;
    int delta_y1;
    int delta_y2;

    delta_x1=v1[0]-v1[2];
    delta_y1=v1[1]-v1[3];

    delta_x2=v2[0]-v2[2];
    delta_y2=v2[1]-v2[3];

    double distnce1=sqrt(delta_x1*delta_x1+delta_y1*delta_y1);
    double distnce2=sqrt(delta_x2*delta_x2+delta_y2*delta_y2);

    return ((distnce1 ) < (distnce2));


}

double Prosessor::getLineSize(Vec4i l1)
{
    int delta_x1;
    int delta_y1;

    delta_x1=l1[0]-l1[2];
    delta_y1=l1[1]-l1[3];

    return sqrt(delta_x1*delta_x1+delta_y1*delta_y1);

}

bool Prosessor::checkPosition(Point robot_point)
{

    if(robot_point.x < CAMERA_CENTER_X + 25 &&
            robot_point.x > CAMERA_CENTER_X - 25)
        return true;

    else
        if(abs(robot_point.x -CAMERA_CENTER_X) > 180)
            counter =0;
    return false;
}

Point Prosessor::updateRobotPoint(vector<Vec4i> l1,vector<Vec4i> l2)
{
    int x1,x2;
    int y1,y2;





}

Point Prosessor::findMinimumPoint(Vec4i l, Vec4i l1)
{
    int min_x=INFINITY;
    int min_y=INFINITY;




    if(l[0]<l1[0] &&
            l[0] < l1[2] ){
        min_x=l[0];
        min_y=l[1];
    }
    else if( l[2] <l1[0] &&
             l[2] < l1[2]){
        min_x=l[2];
        min_y=l[3];
    }
    else if(l1[0] < l[0] &&
            l1[0] < l[2]){
        min_x=l1[0];
        min_y=l1[1];

    }
    else{

        min_x=l1[2];
        min_y=l1[3];

    }



    return Point(min_x,min_y);

}

Point Prosessor::findMaximumPoint(Vec4i l,Vec4i l1)
{
    int max_x=INFINITY;
    int max_y=INFINITY;



    if(l[0]>l1[0] &&
            l[0]> l1[2] ){
        max_x=l[0];
        max_y=l[1];
    }
    else if( l[2]>l1[0] &&
             l[2] > l1[2]){
        max_x=l[2];
        max_y=l[3];
    }
    else if(l1[0]>l[0] &&
            l1[0]> l[2]){
        max_x=l1[0];
        max_y=l1[1];

    }
    else{

        max_x=l1[2];
        max_y=l1[3];

    }



    return Point(max_x,max_y);

}

void Prosessor::erosion(int value, void *)
{
    //kernel 11,11
    int erosion_type = MORPH_RECT;
    Mat element=getStructuringElement(erosion_type, Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                      Point( erosion_size, erosion_size ));

    erode(frame,frame,element);
    // imshow("Erode & Dilate ",frame);


}

void Prosessor::dilation(int value, void *)
{
    int dilation_type = MORPH_RECT;
    Mat element=getStructuringElement(dilation_type, Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                      Point( dilation_size, dilation_size ));

    dilate(frame,frame,element);

    // imshow("Erode & Dilate ",frame);

}

void  Prosessor::on_trackbar(int value, void *)
{
    //cout<<"H Slider Value : "<<value<<endl;


}

void Prosessor::on_trackbar1(int value, void *)
{
    //cout<<"S Slider Value : "<<value<<endl;
    // if(value ==250)
    //   actived=true;
    //robot_p=Point(640,480);
}

void Prosessor::on_trackbar2(int value, void *)
{
    //cout<<"V Slider Value : "<<value<<endl;

}
