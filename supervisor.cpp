#include "supervisor.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>


// set boarder's size: startX, StartY, Width, Height
#define BORDER_X 25
#define BORDER_Y 25
#define BORDER_W 400
#define BORDER_H 600

#define OBSTACLE_RESPAWN 3000   // obstacle respawn time, create obstacle every n seconds

Supervisor::Supervisor(Ui::MainWindow *ui) : _ui(ui)
{
    this->setSceneRect(0, 0, BORDER_W + 50, BORDER_H + 50);
    this->setFocus();
    this->setStickyFocus(true);

    _obstacleTimer = new QTimer;

    // connect default changed signal with our sceneChanged slot
    connect(this, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(sceneChanged(const QList<QRectF> &)));

    // connect obstacleTimer with createObstacle slot
    connect(_obstacleTimer, SIGNAL(timeout()), this, SLOT(createObstacle()));

    _border = NULL;
    _garage.clear();

    _ui->radioButton_manual->setChecked(true);
}

void Supervisor::createCase1()
{
    init(); // set initial state

    _activeCase = 1;

    // create a main graphic view (is created in mainwindow gui)
    //QGraphicsView *view = new QGraphicsView();

    // create main rect border
    _border = new Border(0, 0, BORDER_W, BORDER_H, Qt::darkGray);    

    // create cars
    Car *car1 = new Car(0, 0, 80, 100, Qt::darkBlue, id_1, eSlow, _border);
    Car *car2 = new Car(0, 0, 80, 100, Qt::darkRed,  id_2, eSlow, _border);

    // put cars in garage
    _garage.push_back(car1);
    _garage.push_back(car2);

    qDebug() << "Border is: " + QString::number(_border->x()) + " " + QString::number(_border->y()) + " " + QString::number(_border->rect().height()) + " " + QString::number(_border->rect().width());
    qDebug() << "Cars are : " + QString::number(car1->x()) + " " + QString::number(car1->y()) + " " + QString::number(car1->rect().height()) + " " + QString::number(car1->rect().width());

    // add elements to the scene
    this->addItem(_border);
    //this->addItem(car1);  // is alread added as a child of added parent _border
    //this->addItem(car2);  // is alread added as a child of added parent _border

    // by moving items inside of a scene we will have a real position
    _border->setPos(BORDER_X, BORDER_Y);
    car1->setPos(getLeftSideX( BORDER_W, car1->rect().width()), BORDER_H - car1->rect().height() - 10);
    car2->setPos(getRightSideX(BORDER_W, car2->rect().width()), BORDER_H - car2->rect().height() - 10);
}

void Supervisor::createCase2()
{
    init(); // set initial state

    _activeCase = 2;  

    // create main rect border
    // !!! for now it does not matter the position, because after adding to scene, each item position will be (0, 0)
    _border = new Border(0, 0, BORDER_W, BORDER_H, Qt::darkGray);

    // create cars
    // !!! for now it does not matter the position, because after adding to scene, each item position will be (0, 0)
    Car *car1 = new Car(0, 0, 80, 100, Qt::darkYellow, id_1, eNormal, _border);
    _garage.push_back(car1);

    qDebug() << "Border is: " + QString::number(_border->x()) + " " + QString::number(_border->y()) + " " + QString::number(_border->rect().height()) + " " + QString::number(_border->rect().width());
    qDebug() << "Cars are : " + QString::number(car1->x()) + " " + QString::number(car1->y()) + " " + QString::number(car1->rect().height()) + " " + QString::number(car1->rect().width());

    // add elements to the scene
    this->addItem(_border);
    // this->addItem(car1); // is alread added as a child of added parent _border

    // by moving items inside of a scene we will have a real position
    _border->setPos(BORDER_X, BORDER_Y);
    car1->setPos(getRightSideX(BORDER_W, car1->rect().width()), BORDER_H - car1->rect().height() - 10);

    qDebug() << "Border is: " + QString::number(_border->x()) + " " + QString::number(_border->y()) + " " + QString::number(_border->rect().height()) + " " + QString::number(_border->rect().width());
    qDebug() << "Cars are : " + QString::number(car1->x()) + " " + QString::number(car1->y()) + " " + QString::number(car1->rect().height()) + " " + QString::number(car1->rect().width());
}

void Supervisor::createCase3()
{
    init(); // set initial state

    _activeCase = 3;

    // create main rect border
    // !!! for now it does not matter the position, because after adding to scene, each item position will be (0, 0)
    _border = new Border(0, 0, BORDER_W, BORDER_H, Qt::darkGray);
    _border->setStrip(false);

    // create cars
    // !!! for now it does not matter the position, because after adding to scene, each item position will be (0, 0)
    Car *car1 = new Car(0, 0, 40, 50, Qt::darkMagenta, id_1, eSlow, _border);
    Car *car2 = new Car(0, 0, 40, 50, Qt::darkGreen,   id_2, eSlow, _border);
    Car *car3 = new Car(0, 0, 40, 50, Qt::darkBlue,    id_3, eSlow, _border);
    Car *car4 = new Car(0, 0, 40, 50, Qt::darkYellow,  id_4, eSlow, _border);

    // add cars to garage
    _garage.push_back(car1);
    _garage.push_back(car2);
    _garage.push_back(car3);
    _garage.push_back(car4);

    // add elements to the scene
    // cars will be also added as they are children of parent _border
    this->addItem(_border);

    // by moving items inside of a scene we will have a real position
    _border->setPos(BORDER_X, BORDER_Y);
    car1->setPos(BORDER_W / 5 * 1 - car1->rect().width() / 2, BORDER_H - car1->rect().height() - 60);
    car2->setPos(BORDER_W / 5 * 2 - car2->rect().width() / 2, BORDER_H - car1->rect().height() - 60);
    car3->setPos(BORDER_W / 5 * 3 - car3->rect().width() / 2, BORDER_H - car1->rect().height() - 60);
    car4->setPos(BORDER_W / 5 * 4 - car4->rect().width() / 2, BORDER_H - car1->rect().height() - 60);
}

void Supervisor::startStopCase()
{
    if (_activeCase == 1)
    {
        if (_isCaseStarted == false)
        {
            foreach (Car *car, _garage)
            {
                car->start();
            }
        }
        else
        {
            foreach (Car *car, _garage)
            {
                car->stop();
            }
        }

    }
    else if (_activeCase == 2)
    {
        if (_isCaseStarted == false)
        {
            // launch first obstacle immediately and then each OBSTACLE resp time
            createObstacle();
            _obstacleTimer->start(OBSTACLE_RESPAWN);    // createObstacle() will be called every timeout
        }
        else
        {
            _obstacleTimer->stop();
        }
    }
    else if (_activeCase == 3)
    {
        if (_isCaseStarted == false)
        {
            foreach (Car *car, _garage)
            {
                car->setDirection(eInAngle);
                car->setRotation(car->rotation() -90 + qrand() % 180);
                car->start();
            }
        }
        else
        {
            foreach (Car *car, _garage)
            {
                car->stop();
            }
        }
    }

    _isCaseStarted = !_isCaseStarted;
}

void Supervisor::changeSpeed(qint8 speed)
{
    foreach (Car *car, _garage)
    {
        switch (speed)
        {
        case 0:
            car->setSpeed(eNoSpeed);
            break;
        case 1:
            car->setSpeed(eSlow);
            break;
        case 2:
            car->setSpeed(eNormal);
            break;
        case 3:
            car->setSpeed(eFast);
            break;
        case 4:
            car->setSpeed(eTurbo);
            break;
        case 5:
            car->setSpeed(eUltraSound);
            break;
        default:
            // do nothing
            break;
        }
    }
}

void Supervisor::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "Supervisor::keyPressEvent: Border is: " + QString::number(_border->x()) + " " + QString::number(_border->y()) + " " + QString::number(_border->rect().height()) + " " + QString::number(_border->rect().width());

    foreach (Car *car, _garage)
    {
        // <<< GENERAL BEHAVIOR >>>
        switch (event->key())
        {        
        case Qt::Key_0:
            car->setSpeed(eNoSpeed);
            break;
        case Qt::Key_1:
            car->setSpeed(eSlow);
            break;
        case Qt::Key_2:
            car->setSpeed(eNormal);
            break;
        case Qt::Key_3:
            car->setSpeed(eFast);
            break;
        case Qt::Key_4:
            car->setSpeed(eTurbo);
            break;
        case Qt::Key_5:
            car->setSpeed(eUltraSound);
            break;
        case Qt::Key_Up:
            // if car's front side still inside of a border
            if (car->y() > 0)
            {
                car->moveForward();
            }
            break;
        case Qt::Key_Down:
            // if car's rear side still inside of a border
            if (car->y() + car->rect().height() < _border->rect().height())
            {
                car->moveBackward();
            }
            break;
        case Qt::Key_Left:
            // if car's left side still inside of a border
            if (car->x() > 0)
            {
                car->moveLeft();
            }
            break;
        case Qt::Key_Right:
            // if car's right side still inside of a border
            if (car->x() + car->rect().width() < _border->rect().width())
            {
                car->moveRight();
            }
            break;
        default:
            // do nothing
            break;
        }
        // <<< END GENERAL BEHAVIOR >>>


        // <<< ONLY FOR CASE 1 >>>
        if (_activeCase == 1)
        {
            switch (event->key())
            {
            case Qt::Key_Space:
                if (car->isInMove())
                {
                    car->stop();
                }
                else
                {
                    car->start();
                }
                break;
            default:
                // do nothing
                break;
            }
        }
        // <<< END ONLY FOR CASE 1 >>>


        // <<< ONLY FOR CASE 2 >>>
        if (_activeCase == 2)
        {
            switch (event->key())
            {

            default:
                // do nothing
                break;
            }
        }
        // <<< END ONLY FOR CASE 2 >>>


        // <<< ONLY FOR CASE 3 >>>
        if (_activeCase == 3)
        {
            switch (event->key())
            {
            case Qt::Key_Space:
                if (car->isInMove())
                {
                    car->stop();
                }
                else
                {
                    car->setDirection(eInAngle);
                    car->setRotation(car->rotation() -90 + qrand() % 180);
                    car->start();
                }
                break;
            default:
                // do nothing
                break;
            }
        }
        // <<< END ONLY FOR CASE 3 >>>


        // <<< FOR DEBUG PURPOSE >>>
        switch (event->key())
        {
        case Qt::Key_A:
            car->setRotation(car->getRotation() - 10);
            qDebug() << "Supervisor::keyPressEvent: Key A: angle is " + QString::number(car->getRotation());
            break;
        case Qt::Key_D:
            car->setRotation(car->getRotation() + 10);
            qDebug() << "Supervisor::keyPressEvent: Key D: angle is " + QString::number(car->getRotation());
            break;
        default:
            // do nothing
            break;
        }
        // <<< END FOR DEBUG PURPOSE
    }

    QGraphicsScene::keyPressEvent(event);
}

void Supervisor::sceneChanged(const QList<QRectF> &)
{
    //qDebug() << "sceneChanged: Border is: " + QString::number(_border->x()) + " " + QString::number(_border->y()) + " " + QString::number(_border->rect().height()) + " " + QString::number(_border->rect().width());

    foreach (Car *car, _garage)
    {
        // <<< GENERAL BEHAVIOR >>>
        {
            // a car can not go outside the border
            if ((car->direction() == eLeft) && (car->x() <= 0) )
            {
                if (car->isInMove())
                {
                    car->stop();
                }
            }
            else if ((car->direction() == eRight) && (car->x() + car->rect().width() >= _border->rect().width()) )
            {
                if (car->isInMove())
                {
                    car->stop();
                }
            }
            // TODO: add top and bottom

            // Feature: Collision Detection with obstacles and other cars
            bool isCollided = false;

            // if a car collided with smth then turn it red
            foreach(QGraphicsItem* item, car->collidingItems())
            {
                // a car is collided with obstacle or other car
                if (typeid(*item) == typeid(Obstacle) || typeid(*item) == typeid(Car))
                {
                    isCollided = true;
                    if (car->isInCollision() == false)
                    {
                        car->incCollisionCounter();
                        car->setIsInCollision(true);
                        car->setColor(Qt::red);

                        // TODO: number of colisions, how to use car->incCollisionCounter() for multiple cars?
                        _ui->lineEdit_collisions->setText(QString::number(_ui->lineEdit_collisions->text().toInt() + 1));
                    }
                }
            }

            // if a car is not colliding anymore
            if (isCollided == false && car->isInCollision() == true)
            {
                car->setIsInCollision(false);
                car->setDefaultColor();
            }
        } // <<< end GENERAL BEHAVIOR >>>


        // <<< ONLY FOR CASE 1 >>>
        if (_activeCase == 1)
        {
            // check position and stop or change direction
            // if a car reached upper bodder
            if (car->y() <= 0)
            {
                car->setDirection(eBackward);
            }

            // if a car reached lower boarder
            if ( car->y() + car->rect().height() >= _border->rect().height() )
            {
                car->setDirection(eForward);
            }
        } // <<< end ONLY FOR CASE 1 >>>


        // <<< ONLY FOR CASE 2 >>>
        if (_activeCase == 2)
        {
            // Feature: Obstacle Avoidance Systems (Active or Passive + Warnings):
            foreach (QGraphicsItem* item, this->items())
            {
                // find all obstacles in the scene
                if (typeid(*item) == typeid(Obstacle))
                {
                    Obstacle* obstacle = (Obstacle*)item;

                    //qDebug() << "sceneChanged: obstacle is : " + QString::number(obstacle->x()) + " " + QString::number(obstacle->y()) + " " + QString::number(obstacle->rect().height()) + " " + QString::number(obstacle->rect().width());
                    //qDebug() << "sceneChanged: car is      : " + QString::number(car->x()) + " " + QString::number(car->y()) + " " + QString::number(car->rect().height()) + " " + QString::number(car->rect().width());

                    // obstacles are close
                    if (car->y()-250 <= obstacle->y())
                    {
                        // if car and obstacle are on the same lane
                        if ( (car->x() + car->rect().width() >= obstacle->x()) && (car->x() <= obstacle->x() + obstacle->rect().width()) )
                        {
                            obstacle->setColor(QColor(255, 100, 0));    // orange warning color

                            if (_ui->radioButton_auto->isChecked() && car->y()-200 <= obstacle->y())
                            {
                                //if obstacle on the left lane - move to right lane
                                if (car->x() < _border->rect().width() / 2)
                                {
                                    if (!car->isInMove())
                                    {
                                        // call shiftToRight()
                                        car->setDirection(eRight);
                                        car->start();
                                    }
                                }
                                //if obstacle on the right lane - move to left lane
                                else
                                {
                                    if (!car->isInMove())
                                    {
                                        car->setDirection(eLeft);
                                        car->start();
                                    }
                                }
                            }
                        }
                        // if a car and an obstacle are not on the same lane anymore - set usual color
                        else
                        {
                            obstacle->setColor(Qt::gray);
                        }
                    }
                }
            }

            // if a car moved from right lane to left - stop it
            if (car->x() == getLeftSideX(BORDER_W, car->rect().width()))
            {
                if (car->isInMove() && car->direction() == eLeft)
                {
                    car->stop();
                }
            }

            // if a car moved from let lane to right - stop it
            if (car->x() == getRightSideX(BORDER_W, car->rect().width()))
            {
                if (car->isInMove() && car->direction() == eRight)
                {
                    car->stop();
                }
            }
        } // <<< end ONLY FOR CASE 2 >>>


        // <<< ONLY FOR CASE 3 >>>
        if (_activeCase == 3)
        {
            eCarPosition currentPosition;

            // Feature: Allowwing a car to turn when it close to the borders
            // if a car reached (almost: + car's height as buffer) TOP borders side
            if (car->y() - car->rect().height() <= 0)
            {
                currentPosition = eTop;
                // set rotation only once in the beginning, but not every time while its rotating and its new position
                if (car->direction() != eRotation && car->getLastPosition() != currentPosition)
                {
                    car->setLastPosition(eTop);
                    qDebug() << "Supervisor::sceneChanged: TOP border, angle is " + QString::number(car->rotation());

                    if (car->getRotation() <= 90)
                    {
                        car->setRotationAngle(+90);
                        //car->setRotationAngle(+45 + (qrand() % 45));
                    }
                    else // car->getRotation() >= 270 && car->getRotation() <= 360
                    {
                        car->setRotationAngle(-90);
                        //car->setRotationAngle(-45 - (qrand() % 45));
                    }

                    car->setDirection(eRotation);
                }
            }
            // if a car reached (almost: + car's height as buffer) RIGHT borders side
            else if (car->x() + car->rect().height() >= _border->rect().width())
            {
                currentPosition = eRightSide;
                // set rotation only once in the beginning, but not every time while its rotating and its new position
                if (car->direction() != eRotation && car->getLastPosition() != currentPosition)
                {  
                    car->setLastPosition(eRightSide);
                    qDebug() << "Supervisor::sceneChanged: RIGHT border, angle is " + QString::number(car->getRotation());
                    qDebug() << "Supervisor::sceneChanged: car->x()+car->height = " + QString::number(car->x() + car->rect().height());

                    if (car->getRotation() > 90 && car->getRotation() <= 180)
                    {
                        car->setRotationAngle(+90);
                    }
                    else
                    {
                        car->setRotationAngle(-90);
                    }

                    car->setDirection(eRotation);
                }
            }
            // if a car reached (almost: + car's height as buffer) BOTTOM borders side
            else if (car->y() + car->rect().height() >= _border->rect().height())
            {
                currentPosition = eBottom;
                // set rotation only once in the beginning, but not every time while its rotating and its new position
                if (car->direction() != eRotation && car->getLastPosition() != currentPosition)
                {
                    car->setLastPosition(eBottom);
                    qDebug() << "Supervisor::sceneChanged: BOTTOM border, angle is " + QString::number(car->getRotation());

                    if (car->getRotation() > 180 && car->getRotation() <= 270)
                    {
                        car->setRotationAngle(+90);
                    }
                    else
                    {
                        car->setRotationAngle(-90);
                    }

                    car->setDirection(eRotation);
                }
            }
            // if a car reached (almost: + car's height as buffer) LEFT borders side
            else if (car->x() - car->rect().height() <= 0)
            {
                currentPosition = eLeftSide;
                // set rotation only once in the beginning, but not every time while its rotating and its new position
                if (car->direction() != eRotation && car->getLastPosition() != currentPosition)
                {
                    car->setLastPosition(eLeftSide);
                    qDebug() << "Supervisor::sceneChanged: LEFT border, angle is " + QString::number(car->getRotation());

                    if (car->getRotation() > 270)
                    {
                        car->setRotationAngle(+90);
                    }
                    else
                    {
                        car->setRotationAngle(-90);
                    }

                    car->setDirection(eRotation);
                }
            }

        }
    }
}

void Supervisor::createObstacle()
{
    qDebug() << "Supervisor::createObstacle()";

    // relative to obstacle parent coordinates
    qreal obstacleWidth = 80;   // obstacle width
    qreal obstacleHeight = 80;  // obstacle height
    qreal obstacleX = (rand() % 2 == 0 ? getLeftSideX(BORDER_W, obstacleWidth) : getRightSideX(BORDER_W, obstacleWidth)); // start x point inside of a parent (border): left or right lane
    qreal obstacleY = 0;        // start y point inside of a parent (border): top of it

    Obstacle *obstacle = new Obstacle(0, 0, obstacleWidth, obstacleHeight, Qt::gray, _border);
    obstacle->setPos(obstacleX, obstacleY);

    // item has already been added to this scene by setting it as a child of a border which is aready in scene
    // do not need: this->addItem(obstacle);
}

void Supervisor::init()
{
    _activeCase = 0;
    _isCaseStarted = false;
    _border = nullptr;

    foreach (QGraphicsItem* item, this->items())
    {
        delete item;
    }

    if (!_garage.isEmpty())
    {
        _garage.clear();
    }

    if (_obstacleTimer != nullptr)
    {
        _obstacleTimer->stop();
    }
}

qreal Supervisor::getLeftSideX(qreal parentW, qreal childW)
{
    qreal leftSideX = 0;

    leftSideX = ((parentW / 2) - childW) / 2;

    return leftSideX;
}

qreal Supervisor::getRightSideX(qreal parentW, qreal childW)
{
    qreal rightSideX = 0;

    rightSideX = (parentW / 2) + ((parentW / 2) - childW) / 2;

    return rightSideX;
}

