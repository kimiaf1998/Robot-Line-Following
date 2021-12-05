#include "robot.h"
#define CAMERA_CENTER_X 320
#define CAMERA_CENTER_Y 240

#define PORT_NAME "ttySAC0"

Robot::Robot(QObject *parent)
{
    this->position.x=640;
    this->position.y=480;
    this->angle=0;
    this->port=new QextSerialPort(PORT_NAME,QextSerialPort::EventDriven);
    this->port_is_open=false;
    nextState=AHEAD;

    openSerialPort();

}
Point Robot::getPosition() const
{

    return position;
}
double Robot::getAngle() const
{
    return angle;
}
Status Robot::getNextState() const
{
    return nextState;
}

bool Robot::canMove(vector<Vec4i> lines)
{
    if(lines.size()==0 || lines.size()==1){
        this->nextState=STOP;
        this->angle=0;
        return false;
    }
    moves=true;
    return true;
}

void Robot::setPosition(const Point &value)
{
    position = value;
}

bool Robot::sendData(const QByteArray data)
{
    if(port_is_open){
        qDebug()<<"port is open"<<endl;

        port->write(data,data.size());//send data

        return true;
    }
    return false;

}




void Robot::updateState(double delta_x)

{
    if( delta_x >= -10 && delta_x <=10)
        this->nextState=AHEAD;
    else if( delta_x  < 0 )
        this->nextState=TURN_LEFT;
    else
        this->nextState=TURN_RIGHT;


}

void Robot::updateAngle()
{

    if(this->nextState==AHEAD)
        this->angle=0;
    else if(this->nextState==TURN_LEFT)
        this->angle=-this->angle;


}

void Robot::_SLOT_updateRobotMovement(Point robot_point)
{

    int delta_x;
    int delta_y;
    double angle;/*
    delta_x=robot_point.x-this->position.x;
    delta_y=robot_point.y-this->position.y;*/




    this->position=robot_point;

    delta_x=CAMERA_CENTER_X-robot_point.x;
    delta_y=CAMERA_CENTER_Y-robot_point.y;

    //cout<<"****** delta x : "<<delta_x<<" | delta y :  "<<delta_y<<endl;


    angle=atan2((double)(abs( delta_y)),(double)(abs(delta_x)))* 180 /CV_PI;
    cout<<"ANGLE :"<<angle<<endl;
    angle =90 - angle ;
    cout<<"ANGLE 2:"<<angle<<endl;
    this->angle=angle;
    updateState(delta_x);
    updateAngle();

    qDebug()<<"** Robot Position **"<<endl;
    qDebug()<<" (X,Y) = ("<<this->position.x<<","
           <<this->position.y<<") *******"<<endl;
    //data sending
    sendData("Position :");
    sendData("X :");
    sendData(doubleToQByte(this->position.x));
    sendData("Y :");
    sendData(doubleToQByte(this->position.y));




}


void Robot::_SLOT_move()
{
    qDebug()<<"-- Robot Movement --"<<endl;
    cout<<"Robot Has To Turn "<<this->angle<<" Degree  & "
       <<toStringStatus(this->nextState)<<endl;

    //data sending
    sendData("Angle :");
    sendData(doubleToQByte(this->angle));
    sendData("direction :");
    sendData(toStringStatus(this->nextState).c_str());


}

void Robot::_SLOT_move(double angle, Status status)
{
    this->angle=angle;
    this->nextState=status;

    qDebug()<<"-- Robot Movement --"<<endl;
    cout<<"Robot Has To Turn "<<this->angle<<" Degree  & "
       <<toStringStatus(this->nextState)<<endl;

    //data sending
    sendData("Angle :");
    sendData(doubleToQByte(this->angle));
    sendData("direction :");
    sendData(toStringStatus(this->nextState).c_str());

}



String Robot::toStringStatus(int number)
{
    switch(number){

    case 0:
        return "GO AHEAD";

    case 1:
        return "TURN LEFT";

    case 2:
        return "TURN RIGHT";

    default:
        return "STOP";
    }

}

bool Robot::openSerialPort()
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
         port_is_open=true;
        return true;
    }
    else
    {
        //Open error

        qDebug() << "device failed to open:" << port->errorString() <<endl;
        return false;
    }

}

QByteArray Robot::doubleToQByte(double data)
{

    ostringstream streamObj;
    //Add double to stream
    streamObj << data;
    // Get string from output string stream
    String str=streamObj.str();
    QByteArray qb;
    qb.append(str.c_str());
    return qb;

}

void Robot::_SLOT_move_by_qrcode(Status nextMove)
{
    this->nextMove=nextMove;
    this->angle=-90;

    cout<<"Robot has To  "<<toStringStatus(nextMove)<<" " <<this->angle <<" degree"<<endl;


}


