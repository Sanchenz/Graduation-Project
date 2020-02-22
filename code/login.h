#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMouseEvent>  //引用鼠标类头文件
#include <QPixmap>
#include <QPaintEvent>
#include <QtSql>

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    explicit login(QWidget *parent = 0);
    ~login();

    QPoint last;
    void mousePressEvent(QMouseEvent *e);                          //鼠标按下
    void mouseMoveEvent(QMouseEvent *e);                           //鼠标移动
    void mouseReleaseEvent(QMouseEvent *e);                        //鼠标释放
    void paintEvent(QPaintEvent *);                                //绘制透明背景
    QPixmap background;

private slots:
    void sqlite();                                                 //数据库配置与连接
    void on_pushButton_close_clicked();                            //关闭
    void on_pushButton_login_clicked();                            //登陆
    void on_pushButton_res_clicked();                              //注册

signals:
    void showmainwindow();                                         //跳转主窗口信号
    void senduser(QString);                                        //发送用户名到主窗口

private:
    Ui::login *ui;
};

#endif // LOGIN_H
