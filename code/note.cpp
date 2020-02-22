#include "note.h"
#include "ui_note.h"
#include <QtGui>
#include <QtSql>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QDebug>

note::note(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::note)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("电子元器件管理系统"));                               // 设置窗体标题
    this->setWindowFlags((Qt::Dialog | Qt::FramelessWindowHint));              // 无边框,模态
    this->setAttribute(Qt::WA_TranslucentBackground, true);                    // 透明背景
    ui->dateEdit_1->setDisplayFormat("yyyy-MM-dd");             //设置输出格式 ; 开始时间格式为2000/01/01 样式
    ui->dateEdit_2->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit_1->setDateTime(QDateTime::currentDateTime().addDays(-1));  //设置默认时间
    ui->dateEdit_2->setDateTime(QDateTime::currentDateTime().addDays(1));
    ui->dateEdit_1->setCalendarPopup(true);                     //设置下拉日历
    ui->dateEdit_2->setCalendarPopup(true);

    showtableview("2000-01-01","2999-12-31");
}

note::~note()
{
    delete ui;
}

void note::paintEvent(QPaintEvent *)//绘制背景图************************************************************************************************
{
    QPainter p2(this);
    p2.save();
    QBrush brush(QColor(0,0,0));
    p2.setBrush(brush);
    background.load(":/back3.png");                                  //背景图片路径
    p2.drawPixmap(0,0,790,480,background);                          //背景图片坐标大小
    p2.restore();
}

void note::showtableview(QString date_1,QString date_2)//存数据，画表格*******************************************************************************
{
    QString select;
    select = QString("select * from note where 时间 >= '%1' and 时间 <= '%2'").arg(date_1).arg(date_2);
    static QSqlQueryModel *mode = new QSqlQueryModel(ui->tableView);                                            //建立一个Model
    mode->setQuery(QString("%1").arg(select)) ;                                                                 //从数据库读取数据
    ui->tableView->setModel(mode);
    ui->tableView->verticalHeader()->hide();                                                                   //隐藏行号方法
    ui->tableView->horizontalHeader()->setStretchLastSection(true);                                            //最后一列填满表
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);                                       //设置选择模式,表示只能选中单个
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{color:black; padding-left:4px; border:1px solid rgb(225,225,225,0);}");
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);                            //固定列宽（列宽必须在setModel之后设置才有效）
    ui->tableView->setColumnWidth(0, 130); //时间                                                               //设置列宽（列宽必须在setModel之后设置才有效）
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView->setColumnWidth(1, 100); //用户

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);                                                //可弹出右键菜单
    tableviewmenu = new QMenu(ui->tableView);
    tableviewmenu->addAction("查看详情",this,SLOT(info()));                                                 //设置菜单项,并连接槽函数
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));
}

void note::slotContextMenu(QPoint pos)//右键菜单槽函数*******************************************************************************************
{
    auto index = ui->tableView->indexAt(pos);
    if (index.isValid())
    {
        tableviewmenu->exec(QCursor::pos());                                                                          // 菜单出现的位置为当前鼠标的位置
    }
}

void note::info()//右键菜单  详情*******************************************************************************************************
{
    int row= ui->tableView->currentIndex().row();
    QString information;
    QAbstractItemModel *model = ui->tableView->model();
    information = model->data(model->index(row,2)).toString();
    QMessageBox::critical(0,"详情",information,QMessageBox::Ok|QMessageBox::Default);
}

void note::on_pushButton_find_clicked()//查找按钮****************************************************************************************
{
    QDate date_1 = ui->dateEdit_1->date();
    QDate date_2 = ui->dateEdit_2->date();
    QString strdate_1 = date_1.toString("yyyy-MM-dd");
    QString strdate_2 = date_2.toString("yyyy-MM-dd");
    showtableview(strdate_1,strdate_2);
}

void note::mousePressEvent(QMouseEvent *e)//鼠标按下********************************************************************************************
{
    if(e->pos().rx()>790 || e->pos().ry()>480)
        return ;
    last = e->globalPos();
}

void note::mouseMoveEvent(QMouseEvent *e)//鼠标移动********************************************************************************************
{
    if(e->pos().rx()>790 || e->pos().ry()>480)
        return ;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx,y()+dy);
}

void note::mouseReleaseEvent (QMouseEvent *e)//鼠标释放****************************************************************************************
{
    if(e->pos().rx()>790 || e->pos().ry()>480)
        return ;
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}

void note::showthis()
{
    this->exec();
}

void note::on_pushButton_close_clicked()
{
    close();
}
