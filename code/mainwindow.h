#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>  //引用鼠标类头文件
#include "dialog.h"
#include <QPixmap>
#include <QPaintEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    Ui::MainWindow *ui;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QPoint last;
    QMenu *tableviewmenu;                                               //表格右键菜单
    void mousePressEvent(QMouseEvent *e);                               //鼠标按下
    void mouseMoveEvent(QMouseEvent *e);                                //鼠标移动
    void mouseReleaseEvent(QMouseEvent *e);                             //鼠标释放
    void paintEvent(QPaintEvent *);                                     //绘制透明背景
    QPixmap background;
    void showtableview(QString mod, QString select_1, QString select_2, QString order_1, QString order_2);   //读取数据画表格

private slots:
    void slotContextMenu(QPoint pos);                                   //表格右键槽函数
    void onUpdateInfo();                                                //表格右键“修改”槽函数
    void onDelInfo();                                                   //表格右键“删除”槽函数
    void on_pushButton_add_clicked();                                   //添加按钮槽函数
    void on_pushButton_choose_clicked();                                //筛选按钮槽函数
    void on_pushButton_find_clicked();                                  //查找按钮槽函数
    void on_pushButton_close_clicked();                                 //关闭窗口
    void on_pushButton_small_clicked();                                 //最小化窗口 
    void on_pushButton_note_clicked();                                  //日志
    void showthis();                                                    //显示主窗口
    void showuser(QString data);                                        //显示用户名

signals:
    void shownote();                                                    //显示日志
    void dbclose();

private:
    Dialog *ui2;
};

#endif // MAINWINDOW_H
