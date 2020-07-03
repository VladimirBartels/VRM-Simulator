#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QTimer>

#include "border.h"

class Obstacle : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Obstacle(qreal x, qreal y, qreal w, qreal h, qint8 speed, QColor color = Qt::gray, QGraphicsItem *parent = Q_NULLPTR);

    void setColor(QColor color);
    void start();
    void stop();

    void setSpeed(const qint8 &speed);

public slots:
    void move();

private:    
    QColor _color;              // color of the obstacle
    qint8  _speed;              // speed of the obstacle
    QTimer *_moveTimer;
};

#endif // OBSTACLE_H
