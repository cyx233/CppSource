#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QObject>
#include <QMouseEvent>
#include <QString>
#include "startwindow.h"
#include "dmfb.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    enum{
        START_STATE = 0,
        INPUT_STATE,
        OUTPUT_STATE,
        MAIN_STATE
    };
public slots:
    void go_to_next();
    void show_error(int);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);


private slots:
    void on_actionStart_triggered();
    void on_actionExit_triggered();
    void set_tunnel(QMouseEvent*);
    void change_state();
    void on_stateButton_clicked();

private:
    Ui::MainWindow *ui;
    StartWindow* start_window;
    DMFB* dmfb;
};

#endif // MAINWINDOW_H
