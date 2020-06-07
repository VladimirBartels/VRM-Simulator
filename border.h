#ifndef BORDER_H_
#define BORDER_H_

#include <QGraphicsRectItem>
#include <QMouseEvent>

class Border : public QObject, public QGraphicsRectItem
{
public:
    Border(QColor color);
    Border(qreal x, qreal y, qreal w, qreal h, QColor color);

    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void setStrip(bool isStrip);    // set flag and draw it

private:
    void setColor(QColor color);
    QColor _color;
    QGraphicsLineItem *_strip;
    bool _isStrip;
};

#endif // BORDER_H_
