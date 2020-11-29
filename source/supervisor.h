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
    ~Supervisor();
    void createCase1();
    void createCase2();
    void createCase3();
    void startStopCase();                       // starts/stops each case: every object (stops timer)
    void changeSpeed(qint8 speed);              // change speed of the whole case (speed of each object)

    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void sceneChanged(const QList<QRectF> &);   // default slot from QGraphicsScene
    void slotCreateObstacle();                  // slot is called every _obstacleTimer timeout
    void slotCarClicked(int carId);             // slot is called when a car is selected (clicked)

private:
    void init();                                        // set everything to initial state
    void updateCarStatistic();                          // updating car statistic (coordinates, angles, direction etc)

    // main features:
    void collisionDetection(Car *car);                  // Collision Detection with obstacles and other cars
    void bordersDetection(Car *car);                    // Borders detection
    void obstacleAvoidanceSystem(Car *car);             // Obstacle Avoidance System
    void carsAvoidanceSystem(Car *car);                 // Cars Avoidance Systems

    // support functions
    qreal getLeftLaneX(qreal parentW, qreal childW);    // get left lane x coordinate of a road/board
    qreal getRightLaneX(qreal parentW, qreal childW);   // get right lane x coordinate of a road/board

    qreal getAngleBetweenPoints(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal getDistanceBetweenPoints(qreal x1, qreal y1, qreal x2, qreal y2);

    Ui::MainWindow *_ui;
    Border         *_border;        // main border is a map/world for all elements in it
    QList<Car*>     _garage;        // garage is needed to have all caes somewhere
    qint8           _speed;         // map speed (speed for every car)
    qint8           _activeCase;    // only one case can be displayed at one time
    qint8           _selectedCar;   // selected car id for a car statistics info
    bool            _isCaseStarted; // is a case already started

    // case 2
    QTimer         *_obstacleTimer; // every timeout an obstacle will be created
};

#endif // SUPERVISOR_H
