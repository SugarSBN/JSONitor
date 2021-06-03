#include "quiwidget.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qtoolbutton.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qcombobox.h"
#include "qaction.h"
#include "qfile.h"
#include "qevent.h"
#include "qtranslator.h"
#include "qtextcodec.h"
#include "qfontdatabase.h"
#include "qapplication.h"
#include "qdesktopwidget.h"
#include "qtimer.h"
#include "qdebug.h"
#include "mainwindow.h"
#include "qwidget.h"
#include "ui_mainwindow.h"
#include <QTextEdit>
#include<json/json.h>
#include<json/value.h>
#include<json/reader.h>
#include<json/writer.h>
#include<iostream>
#include"mysql_connector.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->button_new, SIGNAL(triggered()), this, SLOT(new_file()));
    connect(ui->button_open, SIGNAL(triggered()), this, SLOT(open_file()));
    connect(ui->button_save, SIGNAL(triggered()), this, SLOT(save_file()));
    connect(ui->button_saveas, SIGNAL(triggered()), this, SLOT(save_as()));
    connect(ui->action_F6, SIGNAL(triggered()), this, SLOT(refresh_left()));
    connect(ui->action_F5, SIGNAL(triggered()), this, SLOT(refresh_right()));
    connect(ui->action_network, SIGNAL(triggered()), this, SLOT(import_from_server()));
    connect(ui->save_server, SIGNAL(triggered()), this, SLOT(save_server()));
    //this->installEventFilter(this);
    //this->grabKeyboard();
    this->resize(QSize(1700, 1300));
}
MainWindow::~MainWindow()
{
    delete ui;
}


