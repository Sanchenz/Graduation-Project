#include "login.h"
#include "ui_login.h"
#include <QtGui>
#include <QtSql>
#include <QApplication>
#include <QMessageBox>
#include <QAction>

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    this->setWindowTitle(tr("电子元器件管理系统"));                                                               // 设置窗体标题
    ui->setupUi(this);
    this->setWindowFlags((Qt::Dialog | Qt::FramelessWindowHint));              // 无边框,模态
    this->setAttribute(Qt::WA_TranslucentBackground, true);                    // 透明背景
    ui->lineEdit_pass->setEchoMode(QLineEdit::Password);                       //星号显示密码

    sqlite();
}

login::~login()
{
    delete ui;
}

void login::sqlite()//数据库配置与连接*************************************************************************************************************
{
    //db.setHostName("127.0.0.1");                                                                  //连接数据库主机名（若填的为127.0.0.1不能连接，则改为localhost)
    //db.setPort(3306);                                                                             //连接数据库端口号
    db.setDatabaseName("work");                                                                     //连接数据库名
    //db.setUserName("root");                                                                       //数据库用户名
    //db.setPassword("123456");                                                                     //数据库密码
    db.open();
    if (!db.open())
    {
        ui->pushButton_sql->setStyleSheet("border-radius: 15px;background: #ff0000;");            //连接失败出现小红点
    }
    else
    {
        QSqlQuery query;
        query.exec("create table user(用户 varchar(100),密码 varchar(100));");
        query.exec("create table note(时间 datetime , 用户 varchar(100), 操作 varchar(100));");
        query.exec("create table work(录入时间 varchar(100),单号 int,名称 varchar(100),类型 varchar(50),品牌 varchar(50),封装 varchar(50),库存 int,单价 real(20,2),厂商 varchar(100),修改时间 datetime);");//DEFAULT CHARACTER SET gb2312
    }
}

void login::paintEvent(QPaintEvent *)//绘制背景图************************************************************************************************
{
    QPainter p2(this);
    p2.save();
    QBrush brush(QColor(0,0,0));
    p2.setBrush(brush);
    background.load(":/loginback.png");                                  //背景图片路径
    p2.drawPixmap(0,0,460,300,background);                          //背景图片坐标大小
    p2.restore();
}

void login::on_pushButton_login_clicked()//登陆按钮*************************************************************************************************
{
    QString user = ui->lineEdit_user->text();
    QString password = ui->lineEdit_pass->text();      // 获取输入框内容
    QString str = QString("select * from user where 用户 = '%1' and 密码 = '%2'").arg(user).arg(password);      //查询用户与密码
    QSqlQuery query;
    query.exec(str);
    QString userandpass;
    while(query.next())
    {
        userandpass = query.value(0).toString();
    }
    if(userandpass == NULL)
    {
        QMessageBox::critical(0,"错误","账号或密码错误",QMessageBox::Ok|QMessageBox::Default);
    }
    else
    {
        this->hide();
        emit showmainwindow();
        emit senduser(ui->lineEdit_user->text());
    }
}

void login::on_pushButton_res_clicked()//注册按钮****************************************************************************************************
{
    QString user = ui->lineEdit_user->text();
    QString password = ui->lineEdit_pass->text();                    // 获取输入框内容

    QByteArray BAuser = user.toLatin1();
    QByteArray BApass = password.toLatin1();
    char *u1 = BAuser.data();
    char *p1 = BApass.data();
    char *p2 = BApass.data();

    while ((*u1 && *u1 >= '0' && *u1 <= '9') || ((*u1 >= 'a' && *u1 <= 'z') || (*u1 >= 'A' && *u1 <= 'Z'))) u1++;   //如果是字母或者数字，指针后移
    if (*u1)                                                       //用户为数字或字母
    {
        user = "";
    }

    while (*p1 && *p1 >= '0' && *p1 <= '9') p1++;    //如果是数字，指针后移
    while ((*p2 >= 'a' && *p2 <= 'z') || (*p2 >= 'A' && *p2 <= 'Z')) p2++;   //如果是字母，指针后移
    if (!*p1)           //密码为纯数字             //判断密码是否为字母数字组合
    {
        password = "";
    }
    else if (!*p2)      //密码为纯字母
    {
        password = "";
    }
    else if (password.count() < 6 || password.count() > 15)        //密码长度
    {
        password = "";
    }

    if(user == "" || password == "")
    {
        QMessageBox::critical(0,"错误","账号应由 数字 或 字母 组成\n\n密码应由 数字 和 字母 组成\n\n密码应 高于6位 且 低于15位",QMessageBox::Ok|QMessageBox::Default);
    }
    else
    {

        QString str = QString("select * from user where 用户 = '%1'").arg(user);      //查询用户
        QSqlQuery query;
        query.exec(str);
        QString userandpass;
        while(query.next())
        {
            userandpass = query.value(0).toString();
        }
        if(userandpass == NULL)
        {
            QSqlQuery query;
            query.prepare("insert into user(用户,密码) values(?,?)");                          //添加数据到数据库
            query.addBindValue(user);
            query.addBindValue(password);
            query.exec();
            QString userinfo = QString("注册成功，账号为%1，密码为%2").arg(user).arg(password);
            QMessageBox::critical(0,"提示",userinfo,QMessageBox::Ok|QMessageBox::Default);
        }
        else
        {
            QMessageBox::critical(0,"错误","已存在此账号",QMessageBox::Ok|QMessageBox::Default);
        }
    }
}

void login::mousePressEvent(QMouseEvent *e)//鼠标按下********************************************************************************************
{
    if(e->pos().rx()>460 || e->pos().ry()>300)
        return ;
    last = e->globalPos();
}

void login::mouseMoveEvent(QMouseEvent *e)//鼠标移动********************************************************************************************
{
    if(e->pos().rx()>460 || e->pos().ry()>300)
        return ;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx,y()+dy);
}

void login::mouseReleaseEvent (QMouseEvent *e)//鼠标释放****************************************************************************************
{
    if(e->pos().rx()>460 || e->pos().ry()>300)
        return ;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

void login::on_pushButton_close_clicked()
{
    db.close();
    close();
}
