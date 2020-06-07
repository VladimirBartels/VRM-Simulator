#include "border.h"
#include <QDebug>
#include <QPen>

#define LINEWIDTH 5     // line width of obstacle rect and strip line

Border::Border(QColor color) : QGraphicsRectItem()
{
    setColor(color);
}

Border::Border(qreal x, qreal y, qreal w, qreal h, QColor color) : QGraphicsRectItem(x, y, w, h)
{
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFocus();

    _isStrip = true;
    _color = color;

    setColor(_color);
    setStrip(_isStrip);
}

void Border::keyPressEvent(QKeyEvent *)
{
    qDebug() << "Border::keyPressEvent";
}

void Border::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
    qDebug() << "Border::mouseMoveEvent";
}

void Border::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    qDebug() << "Border::mousePressEvent";
}

void Border::setColor(QColor color)
{
    // set a pen for a border

    this->setPen(QPen(QBrush(color), LINEWIDTH));
}

void Border::setStrip(bool isStrip)
{
    _isStrip = isStrip;

    if (_isStrip == true)
    {
        qreal width = this->rect().width();
        qreal height = this->rect().height();

        _strip = new QGraphicsLineItem(width / 2, 0, width / 2, height, this);
        _strip->setPen(QPen(QBrush(_color), LINEWIDTH, Qt::DashLine));
    }
    else
    {
        if (_strip != nullptr)
        {
            delete _strip;
            _strip = nullptr;
        }
    }
}
