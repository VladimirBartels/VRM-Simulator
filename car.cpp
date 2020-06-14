#include "car.h"
#include "obstacle.h"
#include <QPen>
#include <QFont>
#include <QDebug>

#define LINEWIDTH       5   // line width of a car rect
#define MOVESTEP        2   // movestep for moving a car on a amount of pixels
#define ROTATIONSTEP    5   // every mpve time rotate a car on this angle
#define MOVESPEED       24  // speed of a car (every 24ms it moves)


Car::Car(qreal x, qreal y, qreal w, qreal h, QColor color, quint8 id, qint8 speed, QGraphicsItem *parent) : QGraphicsRectItem(x, y, w, h, parent)
{
    _color = color;
    _id = id;
    _speed = speed;
    _moveTimer = new QTimer;
    _isInMove = false;
    _isInCollision = false;
    _direction = eForward;
    _rotationAngle = 0;
    _colisionCounter = 0;
    _lastPosition = eMiddle;

    setColor(_color);

    // set(print) car id on a car
    qint8 textSize = this->rect().height() / 5;
    QGraphicsTextItem *textId = new QGraphicsTextItem(QString::number(_id), this);
    textId->setPos(this->rect().height() / 4, this->rect().width() / 4);
    textId->setFont(QFont("Times", textSize, QFont::Bold));
    textId->setDefaultTextColor(_color.dark());

    // connect timer and move clot
    connect(_moveTimer, SIGNAL(timeout()), this, SLOT(move()));

    qDebug() << "A car  is created";
}

void Car::setSpeed(const quint8 &speed)
{
    _speed = speed;

    if (_speed > eNoSpeed)
    {
        start();
    }
    else
    {
        stop();
    }
}

void Car::start()
{
    if (_speed)
    {
        _moveTimer->start(MOVESPEED / _speed);
        _isInMove = true;
        move();
    }
}

void Car::stop()
{
    _moveTimer->stop();
    _isInMove = false;
}

void Car::rotate()
{
    if (_rotationAngle < 0)
    {
        this->setRotation(this->rotation()-ROTATIONSTEP);
        _rotationAngle += ROTATIONSTEP;
    }
    else if (_rotationAngle > 0)
    {
        this->setRotation(this->rotation()+ROTATIONSTEP);
        _rotationAngle -= ROTATIONSTEP;
    }

    if (abs(_rotationAngle) < ROTATIONSTEP) // when rotation is over
    {
        // when a rotation (turn) is over, continue movement
        _direction = eInAngle;
        moveInAngle();
    }
}

void Car::moveForward()
{
    this->setPos(x(), y() - MOVESTEP);
}

void Car::moveBackward()
{
    this->setPos(x(), y() + MOVESTEP);
}

void Car::moveLeft()
{
    this->setPos(x() - MOVESTEP, y());
}

void Car::moveRight()
{
    this->setPos(x() + MOVESTEP, y());
}

void Car::moveInAngle()
{
    // maps item's and parent's coordinate systems. Position schour be relative and NOT absolute
    this->setPos(mapToParent(0, -MOVESTEP));
}

bool Car::isInMove()
{
    return _isInMove;
}

quint16 Car::getRotation()
{
    qint16 rotation = (qint16)(this->rotation()) % 360;

    if (rotation < 0)
    {
        rotation = 360 + rotation;
    }

    return rotation;
}

void Car::move()
{
    if (_direction == eForward)
    {
        moveForward();
    }
    else if (_direction == eBackward)
    {
        moveBackward();
    }
    else if (_direction == eLeft)
    {
        moveLeft();
    }
    else if (_direction == eRight)
    {
        moveRight();
    }
    else if (_direction == eInAngle)
    {
        moveInAngle();
    }
    else if (_direction == eRotation)
    {
        rotate();
    }
    else {}
}

quint8 Car::getId() const
{
    return _id;
}

eCarPosition Car::getLastPosition() const
{
    return _lastPosition;
}

void Car::setLastPosition(const eCarPosition &lastPosition)
{
    _lastPosition = lastPosition;
}

void Car::setRotationAngle(const qint16 &rotationAngle)
{
    _rotationAngle = rotationAngle;
}

bool Car::isInCollision() const
{
    return _isInCollision;
}

void Car::setIsInCollision(bool isInCollision)
{
    _isInCollision = isInCollision;
}

qint32 Car::collisionCounter() const
{
    return _colisionCounter;
}

void Car::setCollisionCounter(const qint32 &colisionCounter)
{
    _colisionCounter = colisionCounter;
}

void Car::incCollisionCounter()
{
    _colisionCounter++;
}

void Car::setColor(QColor color)
{
    // create pen to set color and border width
    this->setPen(QPen(QBrush(color), LINEWIDTH));

    // create brush to fill a car
    this->setBrush(QBrush(color.light(), Qt::SolidPattern));
}

void Car::setDefaultColor()
{
    this->setPen(QPen(QBrush(_color), LINEWIDTH));
    this->setBrush(QBrush(_color.light(), Qt::SolidPattern));
}

eCarDirection Car::direction() const
{
    return _direction;
}

void Car::setDirection(const eCarDirection &direction)
{
    _direction = direction;
}

bool Car::sceneEvent(QEvent *event)
{
//    // if a car collided with smth then turn it red
//    foreach(QGraphicsItem* item, this->collidingItems())
//    {
//        qDebug() << "Car::sceneEvent: car is collided with smth";

//        if (typeid(*item) == typeid(Obstacle))
//        {
//            qDebug() << "Car::sceneEvent: car is collided with obstacle";
//            setColor(Qt::red);
//        }
//        else
//        {
//            //setColor(Qt::yellow);
//        }
//    }

    // call default event
    return QGraphicsRectItem::sceneEvent(event);
}

void Car::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalCarClicked(_id);

    // call default event
    QGraphicsItem::mousePressEvent(event);
}

