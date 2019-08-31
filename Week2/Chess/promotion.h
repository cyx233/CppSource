#ifndef PROMOTION_H
#define PROMOTION_H

#include <QDialog>
#include <QPaintEvent>
#include <QVector>
#include <QPixmap>

namespace Ui {
class Promotion;
}

class Promotion : public QDialog
{
    Q_OBJECT

public:
    explicit Promotion(QWidget *parent = nullptr);
    ~Promotion();
    void set_color(int color,int x, int y);
    int get_type(){return type;}

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Promotion *ui;
    int type,x,y;
};

#endif // PROMOTION_H
