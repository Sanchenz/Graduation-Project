#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtSql>
#include <QApplication>
#include <QMessageBox>
#include <QAction>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("电子元器件管理系统"));                                                               // 设置窗体标题
    this->setWindowFlags((Qt::Dialog | Qt::FramelessWindowHint));              // 无边框,模态
    this->setAttribute(Qt::WA_TranslucentBackground, true);                    // 透明背景

    MainWindow *p = (MainWindow*) parentWidget();                              // 获取父窗口指针
    int row= p->ui->tableView->currentIndex().row();
    QAbstractItemModel *model = p->ui->tableView->model();

    user = p->ui->label_user->text();
    data_ordernum = model->data(model->index(row,1)).toString();               // 获取父窗口表格内容
    data_name = model->data(model->index(row,2)).toString();
    data_type = model->data(model->index(row,3)).toString();
    data_make = model->data(model->index(row,4)).toString();
    data_package = model->data(model->index(row,5)).toString();
    data_num = model->data(model->index(row,6)).toString();
    data_price = model->data(model->index(row,7)).toString();
    data_company = model->data(model->index(row,8)).toString();
    time_add = model->data(model->index(row,0)).toString();

    ui->plainTextEdit_ordernum->setPlainText(data_ordernum);                    // 填写内容
    ui->plainTextEdit_name->setPlainText(data_name);
    ui->comboBox_type->setCurrentText(data_type);
    ui->plainTextEdit_make->setPlainText(data_make);
    ui->plainTextEdit_package->setPlainText(data_package);
    ui->plainTextEdit_num->setPlainText(data_num);
    ui->plainTextEdit_price->setPlainText(data_price);
    ui->plainTextEdit_company->setPlainText(data_company);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::paintEvent(QPaintEvent *)//绘制背景图************************************************************************************************
{
    QPainter p3(this);
    p3.save();
    QBrush brush(QColor(0,0,0));
    p3.setBrush(brush);
    background.load(":/back2.png");                                  //背景图片路径
    p3.drawPixmap(0,0,450,260,background);                               //背景图片坐标大小
    p3.restore();
}

void Dialog::on_pushButton_add_clicked()
{
    MainWindow *p = (MainWindow*) parentWidget();
    QString ordernum = ui->plainTextEdit_ordernum->toPlainText();
    QString name = ui->plainTextEdit_name->toPlainText();                                                       // 获取输入框内容
    QString type = ui->comboBox_type->currentText();
    QString make = ui->plainTextEdit_make->toPlainText();
    QString package = ui->plainTextEdit_package->toPlainText();
    QString num = ui->plainTextEdit_num->toPlainText();
    QString price = ui->plainTextEdit_price->toPlainText();
    QString company = ui->plainTextEdit_company->toPlainText();
    QDateTime time = QDateTime::currentDateTime();                                                              //获取系统现在的时间
    QString time_change = time.toString("yyyy-MM-dd hh:mm:ss");                                                    //设置显示格式

    QSqlQuery query;
    QString str =  QString("delete from work where 单号 = '%1'").arg(data_ordernum);
    query.exec(str);

    QByteArray BA_ordernum = ordernum.toLatin1();                                                               //判断ordernum是否数字
    const char *o = BA_ordernum.data();
    while(*o && *o>='0' && *o<='9') o++;
    if (*o)
    {
        ordernum = "";
    }

    if (ordernum == "")
    {
    }
    else
    {
        QString str = QString("select * from work where 单号 = '%1'").arg(ordernum);                       //判断ordernum是否重复
        QSqlQuery query;
        query.exec(str);
        QString table_ordernum;
        while(query.next())
        {
            table_ordernum = query.value(1).toString();
        }
        if(table_ordernum == NULL)
        {
        }
        else
        {
            ordernum = "重复";
        }
    }

    QByteArray BA_num = num.toLatin1();                                                                         //判断num是否数字
    const char *n = BA_num.data();
    while(*n && *n>='0' && *n<='9') n++;
    if (*n)
    {
        num = "";
    }

    QByteArray BA_price = price.toLatin1();                                                                     //判断price是否为小数或整数
    const char *a = BA_price.data();
    while(( *a && *a>='0' && *a<='9' )||( *a=='.' )) a++;
    if (*a)
    {
        price = "";
    }


    done_ordernum = QString("单号 %1 改为 %2 ;").arg(data_ordernum).arg(ordernum);
    if(name == data_name)
    {
        done_name = "";
    }
    else
    {
        done_name = QString("名称 %1 改为 %2 ;").arg(data_name).arg(name);
    }

    if(type == data_type)
    {
        done_type = "";
    }
    else
    {
        done_type = QString("类型 %1 改为 %2 ;").arg(data_type).arg(type);
    }

    if(make == data_make)
    {
        done_make = "";
    }
    else
    {
        done_make = QString("品牌 %1 改为 %2 ;").arg(data_make).arg(make);
    }

    if(package == data_package)
    {
        done_package = "";
    }
    else
    {
        done_package = QString("封装 %1 改为 %2 ;").arg(data_package).arg(package);
    }

    if(num == data_num)
    {
        done_num = "";
    }
    else
    {
        done_num = QString("库存 %1 改为 %2 ;").arg(data_num).arg(num);
    }

    if(price == data_price)
    {
        done_price = "";
    }
    else
    {
        done_price = QString("价格 %1 改为 %2 ;").arg(data_price).arg(price);
    }

    if(company == data_company)
    {
        done_company = "";
    }
    else
    {
        done_company = QString("厂商 %1 改为 %2 ;").arg(data_company).arg(company);
    }

    if(ordernum == "")
    {
        QMessageBox::critical(0,"错误","元器件单号错误\n单号不能为空\n单号应为数字",QMessageBox::Ok|QMessageBox::Default);
    }
    else
        if(ordernum == "重复")
        {
            QMessageBox::critical(0,"错误","元器件单号重复",QMessageBox::Ok|QMessageBox::Default);
        }
        else
            if(name == "")
            {
                QMessageBox::critical(0,"错误","请输入元器件“名称”",QMessageBox::Ok|QMessageBox::Default);                    //错误提示
            }
            else
                if(make == "")
                {
                    QMessageBox::critical(0,"错误","请输入元器件“品牌”",QMessageBox::Ok|QMessageBox::Default);
                }
                else
                    if(package == "")
                    {
                        QMessageBox::critical(0,"错误","请输入元器件“封装”",QMessageBox::Ok|QMessageBox::Default);
                    }
                    else
                        if(num == "")
                        {
                            QMessageBox::critical(0,"错误","请输入正确的元器件“数量”",QMessageBox::Ok|QMessageBox::Default);
                        }
                        else
                            if(price == "")
                            {
                                QMessageBox::critical(0,"错误","请输入正确的元器件“单价”",QMessageBox::Ok|QMessageBox::Default);
                            }
                            else
                                if(company == "")
                                {
                                    QMessageBox::critical(0,"错误","请输入元器件“供应商”",QMessageBox::Ok|QMessageBox::Default);
                                }
                                else
                                {
                                    QMessageBox::StandardButton reply;
                                    QString ntnp;
                                    ntnp = QString("是否修改为元器件\n单号：%1\n名称：%2\n类型：%3\n品牌：%4\n封装：%5\n数量：%6\n单价：%7\n供应商：%8").arg(ordernum).arg(name).arg(type).arg(make).arg(package).arg(num).arg(price).arg(company);     //提示窗口以及按钮事件
                                    QString done;
                                    done = QString("修改元器件：%1%2%3%4%5%6%7%8").arg(done_ordernum).arg(done_name).arg(done_type).arg(done_make).arg(done_package).arg(done_num).arg(done_price).arg(done_company);
                                    reply = QMessageBox::critical(0,"提示",ntnp,QMessageBox::Yes|QMessageBox::No);
                                    if (reply == QMessageBox::Yes)
                                    {
                                        QSqlQuery query;
                                        query.prepare("insert into work(录入时间,单号,名称,类型,品牌,封装,库存,单价,厂商,修改时间) values(?,?,?,?,?,?,?,?,?,?)");                          //添加数据到数据库
                                        query.addBindValue(time_add);
                                        query.addBindValue(ordernum);
                                        query.addBindValue(name);
                                        query.addBindValue(type);
                                        query.addBindValue(make);
                                        query.addBindValue(package);
                                        query.addBindValue(num);
                                        query.addBindValue(price);
                                        query.addBindValue(company);
                                        query.addBindValue(time_change);
                                        query.exec();
                                        query.prepare("insert into note(时间,用户,操作) values(?,?,?)");
                                        query.addBindValue(time_change);
                                        query.addBindValue(user);
                                        query.addBindValue(done);
                                        query.exec();
                                        p->showtableview("all","","","","");
                                        close();
                                    }
                                }
}

void Dialog::mousePressEvent(QMouseEvent *e)//鼠标按下********************************************************************************************
{
    if(e->pos().rx()>450 || e->pos().ry()>260)
        return ;
    last = e->globalPos();
}

void Dialog::mouseMoveEvent(QMouseEvent *e)//鼠标移动********************************************************************************************
{
    if(e->pos().rx()>450 || e->pos().ry()>260)
        return ;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx,y()+dy);
}

void Dialog::mouseReleaseEvent (QMouseEvent *e)//鼠标释放****************************************************************************************
{
    if(e->pos().rx()>450 || e->pos().ry()>260)
        return ;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

void Dialog::on_pushButton_close_clicked()
{
    close();
}
