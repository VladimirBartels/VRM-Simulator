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
    qreal width = this->rect().width();
    qreal height = this->rect().height();

    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFocus();

    _isStrip = true;    // by default the line is visible
    _color = color;    

    // create a strip line
    _strip = new QGraphicsLineItem(width / 2, 0, width / 2, height, this);
    _strip->setPen(QPen(QBrush(_color), LINEWIDTH, Qt::DashLine));

    setColor(_color);
}

Border::~Border()
{
    if (_strip != nullptr)
    {
        //delete _strip; Strip is or will be deleted in Init() by Supervisor
        _strip = nullptr;
    }
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

    _strip->setVisible(_isStrip);
}
