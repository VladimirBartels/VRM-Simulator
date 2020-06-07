#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include <QObject>
#include <QGraphicsScene>
#include "ui_mainwindow.h"

#include "car.h"
#include "border.h"
#include "obstacle.h"

class Supervisor : public QGraphicsScene
{
    Q_OBJECT
public:
    Supervisor(Ui::MainWindow *ui);
    void createCase1();
    void createCase2();
    void createCase3();
    void startStopCase();
    void changeSpeed(qint8 speed);

    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void sceneChanged(const QList<QRectF> &);
    void createObstacle();

private:
    void init();                    // set everything to initial state
    qreal getLeftSideX(qreal parentW, qreal childW);    // get left side of a road/board
    qreal getRightSideX(qreal parentW, qreal childW);   // get right side of a road/board

    Ui::MainWindow *_ui;
    Border         *_border;        // main border is a map/world for all elements in it
    QList<Car*>     _garage;        // garage is needed to have all caes somewhere
    qint8           _speed;
    qint8           _activeCase;    // only one case can be displayed at one time
    bool            _isCaseStarted; // is a case already started

    // case 2
    QTimer         *_obstacleTimer; // every timeout an obstacle will be created
};

#endif // SUPERVISOR_H
