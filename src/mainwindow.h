#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QObject>
#include <QMouseEvent>
#include <QString>
#include <QTimer>
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
public slots:
    void go_to_next();
    void show_error(int);
    void next_lock(int);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);


private slots:
    void on_actionStart_triggered();
    void on_actionExit_triggered();
    void set_tunnel(QMouseEvent*);
    void set_barrier(QMouseEvent*);
    void change_state();
    void on_stateButton_clicked();
    void on_nextStepButton_clicked();
    void on_preStepButton_clicked();
    void on_RestartButton_clicked();
    void on_AutoButton_clicked();

    void on_WashButton_clicked();

private:
    Ui::MainWindow *ui;
    StartWindow* start_window;
    DMFB* dmfb;
    QTimer* timer;
};

#endif // MAINWINDOW_H
