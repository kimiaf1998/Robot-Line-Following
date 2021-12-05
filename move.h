#ifndef MOVE_H
#define MOVE_H

#include <QObject>

class Move : public QObject

{

    enum DIRECTION{
        Left,Right,Straight
    };

    Q_OBJECT
public:
    explicit Move(QObject *parent = 0);

    DIRECTION direction;


    DIRECTION getDirection() const;

signals:

public slots:



};

#endif // MOVE_H
