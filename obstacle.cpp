#include "obstacle.h"
#include <QPen>
#include <QDebug>

#define LINEWIDTH 5     // line width of obstacle rect
#define MOVESPEED 30    // speed of obstacle (every 30ms it moves down)
#define MOVESTEP  3     // movestep for moving an obstacle on amount of pixels


Obstacle::Obstacle(qreal x, qreal y, qreal w, qreal h, QColor color, QGraphicsItem *parent) : QGraphicsRectItem(x, y, w, h, parent)
{
    qDebug() << "Obstacle::Obstacle";

    _color = color;
    setColor(_color);

    _moveTimer = new QTimer;

    // connect timer and move clot
    connect(_moveTimer, SIGNAL(timeout()), this, SLOT(move()));

    _moveTimer->start(MOVESPEED);
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

void Obstacle::setColor(QColor color)
{
    // create pen to set color and border width
    this->setPen(QPen(QBrush(color), LINEWIDTH));

    // create brush to fill a car
    this->setBrush(QBrush(color.light(), Qt::SolidPattern));
}
