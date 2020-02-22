#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QtGui>
#include <QtSql>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("电子元器件管理系统"));                                                               // 设置窗体标题
    this->setWindowFlags((Qt::FramelessWindowHint));                                                           // 无边框
    this->setAttribute(Qt::WA_TranslucentBackground, true);                                                    // 透明背景
    showuser(QString());
    showtableview("all","","","","");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showuser(QString data)
{
    QString user = QString("%1").arg(data);
    ui->label_user->setText(user);
}

void MainWindow::paintEvent(QPaintEvent *)//绘制背景图************************************************************************************************
{
    QPainter p(this);
    p.save();
    QBrush brush(QColor(0,0,0));
    p.setBrush(brush);
    background.load(":/back.png");                                  //背景图片路径
    p.drawPixmap(0,0,1100,630,background);                          //背景图片坐标大小
    p.restore();
}

void MainWindow::showtableview(QString mod,QString select_1,QString select_2,QString order_1,QString order_2)//存数据，画表格*******************************************************************************
{
    QString select;
    if(mod == "all")
    {
        select = QString("select * from work");
    }
    if(mod == "select")
    {
        select = QString("select * from work where %1 = '%2'").arg(select_1).arg(select_2);
    }
    if(mod == "order")
    {
        select = QString("select * from work order by %1 %2").arg(order_1).arg(order_2);
    }
    if(mod == "select and order")
    {
        select = QString("select * from work where %1 = '%2' order by %3 %4").arg(select_1).arg(select_2).arg(order_1).arg(order_2);
    }
    //建立表格
    static QSqlQueryModel *mode = new QSqlQueryModel(ui->tableView);                                            //建立一个Model
    mode->setQuery(QString("%1").arg(select)) ;                                                                 //从数据库读取数据
    /*mode->setHeaderData(5,Qt::Horizontal,QObject::tr("库存/个"));                                              //表头设置
    mode->setHeaderData(6,Qt::Horizontal,QObject::tr("单价/元"));*/
    ui->tableView->setModel(mode);
    ui->tableView->verticalHeader()->hide();                                                                   //隐藏行号方法
    //ui->tableView->horizontalHeader()->setStretchLastSection(true);                                            //最后一列填满表
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);                                       //设置选择模式,表示只能选中单个
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{color:black; padding-left:4px; border:1px solid rgb(225,225,225,0);}");

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);                                                //可弹出右键菜单
    tableviewmenu = new QMenu(ui->tableView);
    tableviewmenu->addAction("修改",this,SLOT(onUpdateInfo()));                                                 //设置菜单项,并连接槽函数
    tableviewmenu->addAction("删除",this,SLOT(onDelInfo()));
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(0, 130); //录入时间
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);                            //固定列宽（列宽必须在setModel之后设置才有效）
    ui->tableView->setColumnWidth(1, 90);  //单号                                                                    //设置列宽（列宽必须在setModel之后设置才有效）
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(2, 135); //名称
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(3, 75);  //类型
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(4, 100); //品牌
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(5, 100); //封装
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(6, 60);  //库存
    ui->tableView->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(7, 60);  //单价
    ui->tableView->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(8, 160); //厂商
    ui->tableView->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(9, 145); //修改时间
}

void MainWindow::slotContextMenu(QPoint pos)//右键菜单槽函数*******************************************************************************************
{
    auto index = ui->tableView->indexAt(pos);
    if (index.isValid())
    {
        tableviewmenu->exec(QCursor::pos());                                                                          // 菜单出现的位置为当前鼠标的位置
    }
}

void MainWindow::on_pushButton_add_clicked()//添加按钮************************************************************************************************
{
    QString ordernum = ui->plainTextEdit_ordernum->toPlainText();
    QString name = ui->plainTextEdit_name->toPlainText();                                                            // 获取输入框内容
    QString type = ui->comboBox_type->currentText();
    QString make = ui->plainTextEdit_make->toPlainText();
    QString package = ui->plainTextEdit_package->toPlainText();
    QString num = ui->plainTextEdit_num->toPlainText();
    QString price = ui->plainTextEdit_price->toPlainText();
    QString company = ui->plainTextEdit_company->toPlainText();
    QString user = ui->label_user->text();
    QDateTime time = QDateTime::currentDateTime();                                                              //获取系统现在的时间
    QString time_add = time.toString("yyyy-MM-dd hh:mm:ss");                                                    //设置显示格式
    QString time_change = time.toString("yyyy-MM-dd hh:mm:ss");                                                    //设置显示格式

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
    const char *p = BA_price.data();
    while(( *p && *p>='0' && *p<='9' )||( *p=='.' )) p++;
    if (*p)
    {
        price = "";
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
                            QMessageBox::critical(0,"错误","请输入正确的元器件“库存”",QMessageBox::Ok|QMessageBox::Default);
                        }
                        else
                            if(price == "")
                            {
                                QMessageBox::critical(0,"错误","请输入正确的元器件“单价”",QMessageBox::Ok|QMessageBox::Default);
                            }
                            else
                                if(company == "")
                                {
                                    QMessageBox::critical(0,"错误","请输入元器件“厂商”",QMessageBox::Ok|QMessageBox::Default);
                                }
                                else
                                {
                                    QMessageBox::StandardButton reply;
                                    QString ntnp;
                                    ntnp = QString("是否添加元器件\n单号：%1\n名称：%2\n类型：%3\n品牌：%4\n封装：%5\n库存：%6\n单价：%7\n厂商：%8").arg(ordernum).arg(name).arg(type).arg(make).arg(package).arg(num).arg(price).arg(company);     //提示窗口以及按钮事件
                                    QString done;
                                    done = QString("添加元器件：单号：%1  名称：%2  类型：%3  品牌：%4  封装：%5  库存：%6  单价：%7  厂商：%8").arg(ordernum).arg(name).arg(type).arg(make).arg(package).arg(num).arg(price).arg(company);
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
                                        showtableview("all","","","","");               //重画表格刷新数据
                                        query.prepare("insert into note(时间,用户,操作) values(?,?,?)");
                                        query.addBindValue(time_add);
                                        query.addBindValue(user);
                                        query.addBindValue(done);
                                        query.exec();
                                    }
                                }
}

void MainWindow::onUpdateInfo()//右键菜单  修改*******************************************************************************************************
{
    ui2 = new Dialog(this);
    //ui2->setModal(true);            //此处ture为模态，false为非模态，模态对话框垄断输入
    //ui2->show();                    //跳转到修改界面子窗口
    ui2->exec();
}

void MainWindow::onDelInfo()//右键菜单  删除**********************************************************************************************************
{
    int row= ui->tableView->currentIndex().row();//获取当前行
    QAbstractItemModel *model = ui->tableView->model();
    QString data_ordernum = model->data(model->index(row,1)).toString(); //获取选中行第二列的内容
    QString data_name = model->data(model->index(row,2)).toString();
    QString data_type = model->data(model->index(row,3)).toString();
    QString data_make = model->data(model->index(row,4)).toString();
    QString data_package = model->data(model->index(row,5)).toString();
    QString data_num = model->data(model->index(row,6)).toString();
    QString data_price = model->data(model->index(row,7)).toString();
    QString data_company = model->data(model->index(row,8)).toString();
    QString user = ui->label_user->text();
    QDateTime time = QDateTime::currentDateTime();                                                              //获取系统现在的时间
    QString time_del = time.toString("yyyy-MM-dd hh:mm:ss");
    QMessageBox::StandardButton reply;
    QString delrow;
    delrow = QString("是否删除 单号为%1 名称为%2 的元器件\n").arg(data_ordernum).arg(data_name);     //提示窗口以及按钮事件
    QString done;
    done = QString("删除元器件：单号：%1  名称：%2  类型：%3  品牌：%4  封装：%5  库存：%6  单价：%7  厂商：%8").arg(data_ordernum).arg(data_name).arg(data_type).arg(data_make).arg(data_package).arg(data_num).arg(data_price).arg(data_company);
    reply = QMessageBox::critical(0,"提示",delrow,QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QString str = QString("select * from work where 单号 = '%1'").arg(data_ordernum);   //查找对应内容
        QSqlQuery query;
        query.exec(str);
        QString table_ordernum;
        while(query.next())
        {
            table_ordernum = query.value(1).toString();
        }
        if(table_ordernum == NULL)              //数据库无对应内容
        {
        }
        else                                    //数据库已有对应内容
        {
            QString str =  QString("delete from work where 单号 = '%1'").arg(data_ordernum);
            query.exec(str);                                         //删除对应内容
            showtableview("all","","","","");                        //重画表格
            query.prepare("insert into note(时间,用户,操作) values(?,?,?)");
            query.addBindValue(time_del);
            query.addBindValue(user);
            query.addBindValue(done);
            query.exec();
        }
    }
}

void MainWindow::on_pushButton_choose_clicked()//筛选按钮*********************************************************************************************
{
    QString choose = ui->comboBox_choose->currentText();
    QString order_1 = ui->comboBox_order_1->currentText();
    QString order_2 = ui->comboBox_order_2->currentText();
    if(choose == "全部")
    {
        if(order_2 == "正序")
        {
            showtableview("order","","",order_1,"asc");
        }
        if(order_2 == "倒序")
        {
            showtableview("order","","",order_1,"desc");
        }
        if(order_2 == "无排序")
        {
            showtableview("all","","","","");
        }
    }
    else
    {
        if(order_2 == "正序")
        {
            showtableview("select and order","类型",choose,order_1,"asc");
        }
        if(order_2 == "倒序")
        {
            showtableview("select and order","类型",choose,order_1,"desc");
        }
        if(order_2 == "无排序")
        {
            showtableview("select","类型",choose,"","");
        }
    }
}

void MainWindow::on_pushButton_find_clicked()//查找按钮*********************************************************************************************
{
    QString find = ui->plainTextEdit_find->toPlainText();
    QString find_select = ui->comboBox_find->currentText();
    if(find == "")
    {
        QMessageBox::critical(0,"错误","查找对象不能为空",QMessageBox::Ok|QMessageBox::Default);
    }
    else
    {
        showtableview("select",find_select,find,"","");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)//鼠标按下*********************************************************************************************
{
    if(e->pos().rx()>1100 || e->pos().ry()>630)
        return ;
    last = e->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)//鼠标移动*********************************************************************************************
{
    if(e->pos().rx()>1100 || e->pos().ry()>630)
        return ;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx,y()+dy);
}

void MainWindow::mouseReleaseEvent (QMouseEvent *e)//鼠标释放*******************************************************************************************
{
    if(e->pos().rx()>1100 || e->pos().ry()>630)
        return ;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

void MainWindow::on_pushButton_close_clicked()//关闭窗口********************************************************************************************
{
    emit dbclose();
    close();
}

void MainWindow::on_pushButton_small_clicked()
{
    showMinimized();
}

void MainWindow::showthis()
{
    this->show();
}

void MainWindow::on_pushButton_note_clicked()//日志************************************************************************************************
{
    emit shownote();
}
