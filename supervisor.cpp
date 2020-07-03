#include "supervisor.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

#define PI 3.14159265358979

// set boarder's size: startX, StartY, Width, Height
#define BORDER_X 25
#define BORDER_Y 25
#define BORDER_W 400
#define BORDER_H 600

#define OBSTACLE_RESPAWN 3200   // obstacle respawn time, create obstacle every n seconds

Supervisor::Supervisor(Ui::MainWindow *ui) : _ui(ui)
{
    qDebug() << "Supervisor::Supervisor";

    this->setSceneRect(0, 0, BORDER_W + 50, BORDER_H + 50);
    this->setFocus();
    this->setStickyFocus(true);

    _obstacleTimer = new QTimer();

    // connect default changed signal with our sceneChanged slot
    connect(this, SIGNAL(changed(const QList<QRectF> &)), this, SLOT(sceneChanged(const QList<QRectF> &)));

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
    car1->setPos(getLeftLaneX( BORDER_W, car1->rect().width()), BORDER_H - car1->rect().height() - 10);
    car2->setPos(getRightLaneX(BORDER_W, car2->rect().width()), BORDER_H - car2->rect().height() - 10);

    // to see a car statistics when a car is clicked
    foreach (Car *car, _garage)
    {
        connect(car, SIGNAL(signalCarClicked(int)), this, SLOT(slotCarClicked(int)), Qt::UniqueConnection);
    }
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
    car1->setPos(getRightLaneX(BORDER_W, car1->rect().width()), BORDER_H - car1->rect().height() - 10);

    // to see a car statistics when a car is clicked
    foreach (Car *car, _garage)
    {
        connect(car, SIGNAL(signalCarClicked(int)), this, SLOT(slotCarClicked(int)), Qt::UniqueConnection);
    }

    _obstacleTimer->setInterval(_speed ? (OBSTACLE_RESPAWN / _speed) : OBSTACLE_RESPAWN);

    // connect obstacleTimer with createObstacle slot
    connect(_obstacleTimer, SIGNAL(timeout()), this, SLOT(slotCreateObstacle()), Qt::UniqueConnection);
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
    //car1->setPos(BORDER_W / 5 * 1 - car1->rect().width() / 2, BORDER_H - car1->rect().height() - 60);
    car2->setPos(BORDER_W / 5 * 2 - car2->rect().width() / 2, BORDER_H - car2->rect().height() - 60);
    //car3->setPos(BORDER_W / 5 * 3 - car3->rect().width() / 2, BORDER_H - car3->rect().height() - 60);
    car4->setPos(BORDER_W / 5 * 4 - car4->rect().width() / 2, BORDER_H - car4->rect().height() - 60);

    //car2->setRotation(car2->rotation() + 45);

    car1->setRotation(car1->rotation() + 90);
    car1->setPos(BORDER_W / 5 * car1->getId() - car1->rect().width() / 2, BORDER_H / 5 * 1 + 50);
    car3->setRotation(car3->rotation() + 90);
    car3->setPos(BORDER_W / 5 * 1 - car3->rect().width() / 2, BORDER_H / 5 * 3 );

    foreach (Car *car, _garage)
    {
        // set specific direction for this case to move car in different angle
        car->setDirection(eInAngle);

        // to see a car statistics when a car is clicked
        connect(car, SIGNAL(signalCarClicked(int)), this, SLOT(slotCarClicked(int)), Qt::UniqueConnection);
    }
}

void Supervisor::startStopCase()
{
    static int remainingTime = _speed ? (OBSTACLE_RESPAWN / _speed) : OBSTACLE_RESPAWN;

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
            _obstacleTimer->start(remainingTime);    // createObstacle() will be called every timeout

            // launch first obstacle immediately and then each OBSTACLE resp time
            if (_obstacleTimer->remainingTime() == (_speed ? (OBSTACLE_RESPAWN / _speed) : OBSTACLE_RESPAWN))
            {
                slotCreateObstacle();
            }

            foreach (QGraphicsItem* item, this->items())
            {
                // find all obstacles in the scene
                if (typeid(*item) == typeid(Obstacle))
                {
                    Obstacle* obstacle = (Obstacle*)item;

                    obstacle->start();
                }
            }
        }
        else
        {
            // when clicked on "Stop", save remaining obstacle time. Reset it back by click on "Start"
            remainingTime = _obstacleTimer->remainingTime();
            _obstacleTimer->stop();

            foreach (QGraphicsItem* item, this->items())
            {
                // find all obstacles in the scene
                if (typeid(*item) == typeid(Obstacle))
                {
                    Obstacle* obstacle = (Obstacle*)item;

                    obstacle->stop();
                }
            }
        }
    }
    else if (_activeCase == 3)
    {
        if (_isCaseStarted == false)
        {
            foreach (Car *car, _garage)
            {
                //car->setRotation(car->rotation() -90 + qrand() % 180);
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
    // convert slider position to speed value
    switch (speed)
    {
    case 0:
        _speed = eSlow;
        break;
    case 1:
        _speed = eNormal;
        break;
    case 2:
        _speed = eFast;
        break;
    case 3:
        _speed = eTurbo;
        break;
    case 4:
        _speed = eUltraSound;
        break;
    default:
        // do nothing
        break;
    }

    // change speed for all cars
    foreach (Car *car, _garage)
    {
        car->setSpeed(_speed);
    }

    // change speed for all obstacles (if there are any)
    foreach (QGraphicsItem* item, this->items())
    {
        // find all obstacles in the scene
        if (typeid(*item) == typeid(Obstacle))
        {
            Obstacle* obstacle = (Obstacle*)item;

            obstacle->setSpeed(_speed);
        }
    }

    _obstacleTimer->setInterval(_speed ? (OBSTACLE_RESPAWN / _speed) : OBSTACLE_RESPAWN);

    updateCarStatistic();
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
            _speed = eNoSpeed;
            car->setSpeed(_speed);
            break;
        case Qt::Key_1:
            _speed = eSlow;
            car->setSpeed(_speed);
            break;
        case Qt::Key_2:
            _speed = eNormal;
            car->setSpeed(_speed);
            break;
        case Qt::Key_3:
            _speed = eFast;
            car->setSpeed(_speed);
            break;
        case Qt::Key_4:
            _speed = eTurbo;
            car->setSpeed(_speed);
            break;
        case Qt::Key_5:
            _speed = eUltraSound;
            car->setSpeed(_speed);
            break;
        case Qt::Key_Up:
            // if car's front side still inside of a border
            if (car->y() > 0)
            {
                car->moveForward(_speed);
            }
            break;
        case Qt::Key_Down:
            // if car's rear side still inside of a border
            if (car->y() + car->rect().height() < _border->rect().height())
            {
                car->moveBackward(_speed);
            }
            break;
        case Qt::Key_Left:
            // if car's left side still inside of a border
            if (car->x() > 0)
            {
                car->moveLeft(_speed);
            }
            break;
        case Qt::Key_Right:
            // if car's right side still inside of a border
            if (car->x() + car->rect().width() < _border->rect().width())
            {
                car->moveRight(_speed);
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

    // update statistics only fot selected car (_selectedCar)
    updateCarStatistic();

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
            collisionDetection(car);

        } // <<< end GENERAL BEHAVIOR >>>


        // <<< ONLY FOR CASE 1 >>>
        if (_activeCase == 1 && _isCaseStarted)
        {
            // check position and  change direction
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
        if (_activeCase == 2 && _isCaseStarted)
        {
            // Feature: Obstacle Avoidance System (Active or Passive + Warnings):
            obstacleAvoidanceSystem(car);
        } // <<< end ONLY FOR CASE 2 >>>


        // <<< ONLY FOR CASE 3 >>>
        if (_activeCase == 3 && _isCaseStarted)
        {
            // Feature: Allowwing a car to turn when it is close to the borders
            bordersDetection(car);

            // Feature: Cars Avoidance Systems:            
            if (_ui->radioButton_auto->isChecked())
            {
                carsAvoidanceSystem(car);
            }
        }
    }
}

void Supervisor::slotCreateObstacle()
{
    qDebug() << "Supervisor::createObstacle()";

    // relative to obstacle parent coordinates
    qreal obstacleWidth = 80;   // obstacle width
    qreal obstacleHeight = 80;  // obstacle height
    qreal obstacleX = 0;        // start x point inside of a parent (border)
    qreal obstacleY = 0;        // start y point inside of a parent (border): top of it

    qreal leftSideX = getLeftLaneX(BORDER_W, obstacleWidth);
    qreal rightSideX = getRightLaneX(BORDER_W, obstacleWidth);

    // calculate random obstacle position on the road/board
    switch (rand() % 4)
    {
        case 0:
            obstacleX = leftSideX;
        break;
        case 1:
            obstacleX = leftSideX + (rightSideX - leftSideX) / 3 * 1;
        break;
        case 2:
            obstacleX = leftSideX + (rightSideX - leftSideX) / 3 * 2;
        break;
        case 3:
            obstacleX = rightSideX;
        break;
        default:
            // do nothing
        break;
    }

    // firstly create obstacle inside of a parent
    Obstacle *obstacle = new Obstacle(0, 0, obstacleWidth, obstacleHeight, _speed, Qt::gray, _border);

    // then set coordinates (move obstacle) inside of a parent (border)
    obstacle->setPos(obstacleX, obstacleY);

    // item has already been added to this scene by setting it as a child of a border which is aready in scene
    // do not need: this->addItem(obstacle);

    // set interval back to default value if it was set to remaining time by clicking "Start\Stop" button.
    _obstacleTimer->setInterval(_speed ? (OBSTACLE_RESPAWN / _speed) : OBSTACLE_RESPAWN);
}

void Supervisor::slotCarClicked(int carId)
{
    _selectedCar = carId;

    updateCarStatistic();
}

void Supervisor::init()
{
    _activeCase = 0;    // 0 means NO active cases (active cases start from 1 [1..])
    _selectedCar = 0;   // 0 means NO selected cars (cars Ids start from 1 [1..])
    _isCaseStarted = false;
    _border = nullptr;
    _speed = eSlow;     // default speed

    // order is importand. Should be after _selectedCar
    updateCarStatistic();

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

    _ui->lineEdit_collisions->setText(QString::number(0));
}

void Supervisor::updateCarStatistic()
{
    // if a car is selected (car id starts form 1) update statistic only for selected car
    if (_selectedCar > 0 )
    {
        // find a car with this id and update statistic for it
        foreach(Car* car, _garage)
        {
            if (car->getId() == _selectedCar)
            {
                _ui->label_carId_value->setText(QString::number(_selectedCar));
                _ui->label_carX_value->setText(QString::number(car->x()));
                _ui->label_carY_value->setText(QString::number(car->y()));
                _ui->label_carAngle_value->setText(QString::number(car->getRotation()));
                _ui->label_carDirection_value->setText(QString::number(car->direction()));
                _ui->label_carSpeed_value->setText(QString::number(car->getSpeed()));
                break;
            }
        }
    }
    else
    {
        _ui->label_carId_value->clear();
        _ui->label_carX_value->clear();
        _ui->label_carY_value->clear();
        _ui->label_carAngle_value->clear();
        _ui->label_carDirection_value->clear();
        _ui->label_carSpeed_value->clear();
    }
}

void Supervisor::collisionDetection(Car *car)
{
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
}

void Supervisor::bordersDetection(Car *car)
{
    eCarPosition currentPosition;

    // if a car reached (almost: + car's height as buffer) TOP borders side
    if (car->y() - car->rect().height() <= 0)
    {
        currentPosition = eTop;
        // set rotation only once in the beginning, but not every time while its rotating and its new position
        if (car->direction() != eRotation && car->getLastPosition() != currentPosition)
        {
            car->setLastPosition(eTop);

            if (car->getRotation() <= 90)
            {
                //car->setRotationAngle(+90);
                car->setRotationAngle(+180);
                //maybe: car->setRotationAngle(+45 + (qrand() % 45));
            }
            else // car->getRotation() >= 270 && car->getRotation() <= 360
            {
                //car->setRotationAngle(-90);
                car->setRotationAngle(-180);
                //maybe: car->setRotationAngle(-45 - (qrand() % 45));
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

            if (car->getRotation() > 90 && car->getRotation() <= 180)
            {
                //car->setRotationAngle(+90);
                car->setRotationAngle(+180);
            }
            else
            {
                //car->setRotationAngle(-90);
                car->setRotationAngle(-180);
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

            if (car->getRotation() > 180 && car->getRotation() <= 270)
            {
                //car->setRotationAngle(+90);
                car->setRotationAngle(+180);
            }
            else
            {
                //car->setRotationAngle(-90);
                car->setRotationAngle(-180);
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
            //qDebug() << "Supervisor::sceneChanged: LEFT border, angle is " + QString::number(car->getRotation());

            if (car->getRotation() > 270)
            {
                //car->setRotationAngle(+90);
                car->setRotationAngle(+180);
            }
            else
            {
                //car->setRotationAngle(-90);
                car->setRotationAngle(-180);
            }

            car->setDirection(eRotation);
        }
    }
}

void Supervisor::obstacleAvoidanceSystem(Car *car)
{
    bool isObstacle = false;    // if there are any obstacles too close
    const quint16 warningDistance = 250;
    const quint16 maneuverDistance = 200;

    foreach (QGraphicsItem* item, this->items())
    {
        // find all obstacles in the scene
        if (typeid(*item) == typeid(Obstacle))
        {
            Obstacle* obstacle = (Obstacle*)item;

            // obstacles are close - warn an obstacle (passive part)
            if (car->y() - warningDistance <= obstacle->y())
            {
                isObstacle = true;

                // obstacle can collide with a car
                if ( (car->x() + car->rect().width() >= obstacle->x()) && (car->x() <= obstacle->x() + obstacle->rect().width()) )
                {
                    // change color of obstacle to warn a car
                    obstacle->setColor(QColor(255, 100, 0));    // orange warning color

                    // obstacles are too close - start a maneuver (active part)
                    if (_ui->radioButton_auto->isChecked() && car->y() - maneuverDistance <= obstacle->y())
                    {
                        // obstacle is on the left lane:
                        if (car->x() < _border->rect().width() / 2)
                        {
                            // obstacle is more on the rigth side
                            if (obstacle->x() > car->x() + car->rect().width() / 2)
                            {
                                // a car can skirt obstacle from the left side (do not need to cross the lane)
                                if (!car->isInMove())
                                {
                                    car->setDirection(eLeft);
                                    car->start();
                                }
                            }
                            // obstacle is in front of the car - move to right lane
                            else
                            {
                                if (!car->isInMove())
                                {
                                    // call shiftToRight()
                                    car->setDirection(eRight);
                                    car->start();
                                }
                            }

                        }
                        // obstacle is on the right lane:
                        else
                        {
                            // obstacle is more on the rigth side
                            if (obstacle->x() + obstacle->rect().width() < car->x() + car->rect().width() / 2)
                            {
                                // a car can skirt obstacle from the right side (do not need to cross the lane)
                                if (!car->isInMove())
                                {
                                    car->setDirection(eRight);
                                    car->start();
                                }
                            }
                            // obstacle is in front of the car - move to left lane
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
                }
                // if a car and an obstacle are not on the same lane anymore - set usual color
                else
                {
                    obstacle->setColor(Qt::gray);
                }
            }
        }
    }

    // a car did skirt osbtacle from the left
    if (car->x() + car->rect().width() / 2 <= getLeftLaneX(BORDER_W, car->rect().width()))
    {
        // there are obstacle - wait
        if (isObstacle == true)
        {
            if (car->isInMove())
            {
                car->stop();
            }
        }
        // there were no or are no any obstacle more
        else
        {
            car->setDirection(eRight);
            car->start();
        }
    }
    // a car did skirt osbtacle from the right
    else if (car->x() - car->rect().width() / 2 >= getRightLaneX(BORDER_W, car->rect().width()))
    {
        // there are obstacle - wait
        if (isObstacle == true)
        {
            if (car->isInMove())
            {
                car->stop();
            }
        }
        // there were no or are no any obstacle more
        else
        {
            car->setDirection(eLeft);
            car->start();
        }
    }

    if (car->x() == getLeftLaneX(BORDER_W, car->rect().width()))
    {
        // a car moved from right lane to left - stop it
        if (car->isInMove() && car->direction() == eLeft)
        {
            car->stop();
        }
        // a car did skirt obstacle from left - stop it
        else if (car->isInMove() && car->direction() == eRight)
        {
            // TODO: restructure please
            car->stop();
        }
    }

    if (car->x() == getRightLaneX(BORDER_W, car->rect().width()))
    {
        // if a car moved from left lane to right - stop it
        if (car->isInMove() && car->direction() == eRight)
        {
            car->stop();
        }
        // a car did skirt obstacle from right - stop it
        else if (car->isInMove() && car->direction() == eLeft)
        {
            car->stop();
        }
    }
}

void Supervisor::carsAvoidanceSystem(Car *car)
{
    bool isHindrance = false;
    // check if there are any ObstacleCars
    foreach (QGraphicsItem* item, this->items())
    {
        // find all cars in the scene
        if (typeid(*item) == typeid(Car))
        {
            Car* ObstacleCar = (Car*)item;

            // if obstacle car is not a current car
            if (car->getId() != ObstacleCar->getId())
            {
                // if the distance between car's front left and obstacleCar front left point less than 100 and angle between 0 and 90 degrees
                if ( ((getDistanceBetweenPoints(car->x(), car->y(), ObstacleCar->x(), ObstacleCar->y()) < 100) &&
                      (getAngleBetweenPoints(car->x(), car->y(), ObstacleCar->x(), ObstacleCar->y()) > car->getRotation() ) &&
                      (getAngleBetweenPoints(car->x(), car->y(), ObstacleCar->x(), ObstacleCar->y()) < car->getRotation() + 90)) ||
                // if the distance between car's front left and obstacleCar front left point less than 100 and angle between 0 and 90 degrees
                     ((getDistanceBetweenPoints(car->x(), car->y(), ObstacleCar->getRearLeftX(), ObstacleCar->getRearLeftY()) < 100) &&
                      (getAngleBetweenPoints(car->x(), car->y(), ObstacleCar->getRearLeftX(), ObstacleCar->getRearLeftY()) > car->getRotation() ) &&
                      (getAngleBetweenPoints(car->x(), car->y(), ObstacleCar->getRearLeftX(), ObstacleCar->getRearLeftY()) < car->getRotation() + 90)))
                {
                    isHindrance = true;

                    // if no obstacle detected yet
                    if (car->isWaiting() == false)
                    {
                        car->setIsWaiting(true);
                        car->setColor(Qt::black);
                    }
                }
            }
        }
    }

    // if there are no more ObstacleCars
    if (isHindrance == false && car->isWaiting() == true)
    {
        car->setIsWaiting(false);
        car->setDefaultColor();
    }
}

qreal Supervisor::getLeftLaneX(qreal parentW, qreal childW)
{
    qreal leftSideX = 0;

    leftSideX = ((parentW / 2) - childW) / 2;

    return leftSideX;
}

qreal Supervisor::getRightLaneX(qreal parentW, qreal childW)
{
    qreal rightSideX = 0;

    rightSideX = (parentW / 2) + ((parentW / 2) - childW) / 2;

    return rightSideX;
}

qreal Supervisor::getAngleBetweenPoints(qreal x1, qreal y1, qreal x2, qreal y2)
{
    qreal angle = 0;

    angle = (atan2(y2 - y1, x2 - x1) * 180 / PI) + 90;

    if (angle < 0)
    {
        angle = 360 + angle;
    }

    return angle;
}

qreal Supervisor::getDistanceBetweenPoints(qreal x1, qreal y1, qreal x2, qreal y2)
{
    qreal distance = 0;

    distance = sqrt(pow(y2-y1, 2) + pow(x2-x1, 2));

    return distance;
}

