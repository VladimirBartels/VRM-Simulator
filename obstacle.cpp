#include "obstacle.h"
#include <QPen>
#include <QDebug>

#define LINEWIDTH 5     // line width of obstacle rect
#define MOVESPEED 32    // speed of obstacle (every n ms it moves down)
#define MOVESTEP  2     // movestep for moving an obstacle on amount of pixels


Obstacle::Obstacle(qreal x, qreal y, qreal w, qreal h, qint8 speed, QColor color, QGraphicsItem *parent) : QGraphicsRectItem(x, y, w, h, parent)
{
    qDebug() << "Obstacle::Obstacle";

    _color = color;
    _speed = speed;
    _moveTimer = new QTimer;

    setColor(_color);
    setSpeed(_speed);

    start();

    // connect timer and move slot
    connect(_moveTimer, SIGNAL(timeout()), this, SLOT(move()));
}

void Obstacle::move()
{
    this->moveBy(0, 3); // move by number of pixels

    Border *border = (Border*)this->parentItem();

    // delete object if it goes outside of a border
    if (this->y() + this->rect().height() >= border->rect().height())
    {
        delete(this);
    }
}

void Obstacle::setSpeed(const qint8 &speed)
{
    _speed = speed;

    if (_speed == 0)
    {
        stop();
    }
    else
    {
        _moveTimer->setInterval(MOVESPEED / _speed);
    }
}

void Obstacle::setColor(QColor color)
{
    // create pen to set color and border width
    this->setPen(QPen(QBrush(color), LINEWIDTH));

    // create brush to fill a car
    this->setBrush(QBrush(color.light(), Qt::SolidPattern));
}

void Obstacle::start()
{
    _moveTimer->start();
}

void Obstacle::stop()
{
    _moveTimer->stop();
}
