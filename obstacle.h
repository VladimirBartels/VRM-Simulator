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
    Obstacle(qreal x, qreal y, qreal w, qreal h, QColor color = Qt::gray, QGraphicsItem *parent = Q_NULLPTR);

    void setColor(QColor color);
    void start();
    void stop();

public slots:
    void move();

private:    
    QColor _color;
    QTimer *_moveTimer;
};

#endif // OBSTACLE_H
