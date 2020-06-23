#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _supervisor = new Supervisor(ui);    

    ui->view1->setScene(_supervisor);
    ui->view2->setScene(_supervisor);
    ui->view3->setScene(_supervisor);

    ui->tabWidget->setCurrentIndex(0);
    on_tabWidget_tabBarClicked(ui->tabWidget->currentIndex());

    // only for case2
    ui->groupBox_OCS->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if (index == 0)
    {
        //ui->view1->setFocusPolicy(Qt::StrongFocus);        
        ui->groupBox_OCS->setEnabled(false);
        ui->horizontalSlider_speed->setValue(0);
        _supervisor->createCase1();
    }
    else if (index == 1)
    {
        //ui->view2->setFocusPolicy(Qt::StrongFocus);
        ui->horizontalSlider_speed->setValue(0);
        ui->groupBox_OCS->setEnabled(true);
        _supervisor->createCase2();
    }
    else if (index == 2)
    {
        //ui->view3->setFocusPolicy(Qt::StrongFocus);
        ui->horizontalSlider_speed->setValue(0);
        ui->groupBox_OCS->setEnabled(true);
        _supervisor->createCase3();
    }
}

void MainWindow::on_pushButton_startStop_clicked()
{
    _supervisor->startStopCase();
}

void MainWindow::on_horizontalSlider_speed_valueChanged(int value)
{
    _supervisor->changeSpeed(value);
}
