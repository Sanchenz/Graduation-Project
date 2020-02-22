#ifndef NOTE_H
#define NOTE_H

#include <QDialog>
#include <QMouseEvent>  //引用鼠标类头文件
#include <QPixmap>
#include <QPaintEvent>
#include <QMenu>

namespace Ui {
class note;
}

class note : public QDialog
{
    Q_OBJECT

public:
    explicit note(QWidget *parent = 0);
    ~note();

    QPoint last;
    QMenu *tableviewmenu;                                               //表格右键菜单
    void mousePressEvent(QMouseEvent *e);                               //鼠标按下
    void mouseMoveEvent(QMouseEvent *e);                                //鼠标移动
    void mouseReleaseEvent(QMouseEvent *e);                             //鼠标释放
    void paintEvent(QPaintEvent *);                                     //绘制透明背景
    QPixmap background;
    void showtableview(QString date_1, QString date_2);

private slots:
    void slotContextMenu(QPoint pos);                                   //表格右键槽函数
    void info();                                                        //表格右键“详情”槽函数
    void showthis();                                                    //显示日志
    void on_pushButton_close_clicked();                                 //关闭按钮
    void on_pushButton_find_clicked();                                  //查找按钮

private:
    Ui::note *ui;
};

#endif // NOTE_H
