#include "mainwindow.h"
#include "login.h"
#include "note.h"
#include <QtGui>
#include <QtSql>
#include <QApplication>
#include <QMessageBox>
#include <QMenu>
#include <QAction>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    login l;
    MainWindow w;
    note n;
    l.show();
    QObject::connect(&l,SIGNAL(showmainwindow()),&w,SLOT(showthis()));
    QObject::connect(&w,SIGNAL(shownote()),&n,SLOT(showthis()));
    QObject::connect(&l,SIGNAL(senduser(QString)),&w,SLOT(showuser(QString)));
    QObject::connect(&w,SIGNAL(dbclose()),&l,SLOT(on_pushButton_close_clicked()));
    return a.exec();
}

