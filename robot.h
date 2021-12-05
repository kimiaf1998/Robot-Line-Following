#ifndef ROBOT_H
#define ROBOT_H
#include"status.h"
#include<QObject>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<math.h>
#include<qdebug.h>
#include "qextserialport.h"

using namespace cv;
using namespace std;

class Robot :public QObject
{
Q_OBJECT
public:
    explicit Robot(QObject *parent = 0);

    Point getPosition() const;

    double getAngle() const;

    Status getNextState() const;

    bool canMove(vector<Vec4i> lines);

    void setPosition(const Point &value);

    bool sendData(const QByteArray data);


private :

    Point position;

    QextSerialPort * port;

    double angle;

    bool moves;

    bool port_is_open;

    Status nextState;

    Status nextMove;

    void updateState(double delta_x);

    void updateAngle();

    String toStringStatus(int number);

    bool openSerialPort();

    QByteArray doubleToQByte(double data);

signals:

public slots:

    void _SLOT_move_by_qrcode(Status nextMove);

    void _SLOT_updateRobotMovement(Point robot_point);

    void _SLOT_move();//set the robot direction to move

    void _SLOT_move(double angle,Status status);


};

#endif // ROBOT_H
