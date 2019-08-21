#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QString>
#include <QDialog>

namespace Ui {
class NewWindow;
}

class NewWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewWindow(QWidget *parent = nullptr);
    ~NewWindow();
    void set_text(QString s);

private:
    Ui::NewWindow *ui;
};

#endif // NEWWINDOW_H
