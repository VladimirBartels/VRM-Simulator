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
    ~Car();

    // events:
    bool sceneEvent(QEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    // members/methods    
    void setColor(QColor color);        // set any color
    void setDefaultColor();             // set color defined in constructor

    void start();                       // a car will move continuesly
    void stop();                        // a car will stop
    void rotate();                      // start smooth rotation during a turn

    void moveForward();                 // move one step forward (used with timer)
    void moveBackward();                // move one step backward (used with timer)
    void moveLeft();                    // move one step left (used with timer)
    void moveRight();                   // move one step right (used with timer)
    void moveInAngle();                 // movement in rotation - only forward according to rotation angle

    void moveForward(quint8 speed);     // move one step forward (used without timer)
    void moveBackward(quint8 speed);    // move one step backward (used without timer)
    void moveLeft(quint8 speed);        // move one step left (used without timer)
    void moveRight(quint8 speed);       // move one step right (used without timer)

    bool isInMove();                    // check if a car is already moving
    qreal getRearLeftX();               // get x oordinate of rear left point depending of the angle
    qreal getRearLeftY();               // get y oordinate of rear left point depending of the angle

    void setSpeed(const quint8 &speed);
    qint8 getSpeed() const;

    quint16 getRotation();              // returns rotateio angle, convertor to default rect.Rotation. returns [0..359] grad

    eCarDirection direction() const;
    void setDirection(const eCarDirection &direction);

    qint32 collisionCounter() const;
    void setCollisionCounter(const qint32 &collisionCounter);
    void incCollisionCounter();

    bool isInCollision() const;         // check if a car is in collision with other objects
    void setIsInCollision(bool isInCollision);

    void setRotationAngle(const qint16 &rotationAngle);

    eCarPosition getLastPosition() const;
    void setLastPosition(const eCarPosition &lastPosition);

    quint8 getId() const;

    bool isWaiting() const;
    void setIsWaiting(bool isWaiting);

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
    bool          _isWaiting;         // if a car is waiting as there are some obstacle on the right hand
    eCarDirection _direction;         // forward, backward, left, right
    qint16        _rotationAngle;     // rotation angle during a turn
    qint32        _colisionCounter;   // number of colisions with obstacles and other cars
    eCarPosition  _lastPosition;      // previous position of the car (top, bottom, left side, right side, middle of the board)
};

#endif // CAR_H
