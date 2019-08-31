#ifndef COLORCHOOSE_H
#define COLORCHOOSE_H

#include <QDialog>

namespace Ui {
class ColorChoose;
}

class ColorChoose : public QDialog
{
    Q_OBJECT

public:
    explicit ColorChoose(QWidget *parent = nullptr);
    ~ColorChoose();
    int get_color(){return color;}

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ColorChoose *ui;
    int color;
};

#endif // COLORCHOOSE_H
