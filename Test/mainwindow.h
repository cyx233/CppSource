#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "newwindow.h"

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
    void on_actionshow_triggered();

    void on_actionexec_triggered();

    void on_actionmodal_true_triggered();

    void on_actionmodal_false_triggered();

private:
    Ui::MainWindow *ui;
    NewWindow* new_window;
};

#endif // MAINWINDOW_H
