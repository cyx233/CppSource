#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_QuitButton_clicked();
    void pressEvent(int);

private:
    Ui::MainWindow *ui;
    int calculate(int,int);
    int ans;
};

#endif // MAINWINDOW_H
