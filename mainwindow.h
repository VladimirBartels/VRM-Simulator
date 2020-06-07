#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "supervisor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_startStop_clicked();

    void on_horizontalSlider_speed_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    Supervisor *_supervisor;
};

#endif // MAINWINDOW_H
