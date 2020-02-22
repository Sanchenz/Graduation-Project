#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QMouseEvent>  //引用鼠标类头文件
#include <QPixmap>
#include <QPaintEvent>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    QString user;
    QString data_ordernum;
    QString data_name;
    QString data_type;
    QString data_make;
    QString data_package;
    QString data_num;
    QString data_price;
    QString data_company;
    QString time_add;
    QString done_ordernum;
    QString done_name;
    QString done_type;
    QString done_make;
    QString done_package;
    QString done_num;
    QString done_price;
    QString done_company;

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    QPoint last;
    void mousePressEvent(QMouseEvent *e);                               //鼠标按下
    void mouseMoveEvent(QMouseEvent *e);                                //鼠标移动
    void mouseReleaseEvent(QMouseEvent *e);                             //鼠标释放
    void paintEvent(QPaintEvent *);                                     //绘制透明背景
    QPixmap background;

private slots:
    void on_pushButton_add_clicked();                                   //修改按钮
    void on_pushButton_close_clicked();                                 //关闭按钮

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
