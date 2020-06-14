#ifndef CAR_H
#define CAR_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QTimer>


enum eCarDirection
{
    eForward  = 0,
    eBackward = 1,
    eLeft     = 2,  // move (slide) left
    eRight    = 3,  // move (slide) right
    eInAngle  = 4,  // movement in different angle
    eRotation = 5   // do a smooth turn
};

enum eCarId
{
    id_1 = 1,
    id_2 = 2,
    id_3 = 3,
    id_4 = 4,
    id_5 = 5
};

enum eCarSpeed
{
    eNoSpeed    = 0,
    eSlow       = 1,
    eNormal     = 2,
    eFast       = 4,
    eTurbo      = 8,
    eUltraSound = 16
};

enum eCarPosition
{
    eMiddle    = 0,
    eTop       = 1,
    eLeftSide  = 2,
    eRightSide = 3,
    eBottom    = 4
};


class Car : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Car(qreal x, qreal y, qreal w, qreal h, QColor color, quint8 id, qint8 speed = 0, QGraphicsItem *parent = Q_NULLPTR);

    // events:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    // members/methods
    void setSpeed(const quint8 &speed);
    void setColor(QColor color);        // set any color
    void setDefaultColor();             // set color defined in constructor
    void start();                       // a car will move continuesly
    void stop();                        // a car will stop
    void rotate();                      // start smooth rotation during a turn
    void moveForward();                 // move one step forward
    void moveBackward();                // move one step backward
    void moveLeft();                    // move one step left
    void moveRight();                   // move one step right
    void moveInAngle();                 // movement in rotation - only forward according to rotation angle
    bool isInMove();                    // check if a car is already moving

    quint16 getRotation();              // convertor to default rect.Rotation. returns [0..359] grad

    eCarDirection direction() const;
    void setDirection(const eCarDirection &direction);

    qint32 collisionCounter() const;
    void setCollisionCounter(const qint32 &collisionCounter);
    void incCollisionCounter();

    bool isInCollision() const;
    void setIsInCollision(bool isInCollision);

    void setRotationAngle(const qint16 &rotationAngle);

    eCarPosition getLastPosition() const;
    void setLastPosition(const eCarPosition &lastPosition);

    quint8 getId() const;

signals:
    void signalCarClicked(int carId);

public slots:
    void move();

private:    

    QColor        _color;             // default color of the car
    quint8        _id;                // unique car id number
    qint8         _speed;             // speed of a car
    QTimer       *_moveTimer;         // a timer for car's movements
    bool          _isInMove;          // if a car is already in move or not
    bool          _isInCollision;     // is a car already and still in a caollision with obstacles or other cars
    eCarDirection _direction;         // forward, backward, left, right
    qint16        _rotationAngle;     // rotation angle during a turn
    qint32        _colisionCounter;   // number of colisions with obstacles and other cars
    eCarPosition  _lastPosition;      // previous position of the car (top, bottom, left side, right side, middle of the board)
};

#endif // CAR_H
