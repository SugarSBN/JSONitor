/*
 * @Author: SuBonan
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include"mysql_connector.h"
#include"mainwindow.h"
#include"quiwidget.h"
#include<iostream>
#include<QVariant>
#include<QHeaderView>
#include<QEvent>
#include<ctime>

using namespace std;

extern MysqlConnector *mysql_connector;
extern MainWindow *w;

QUIWidget* MysqlConnector::get_page(){
    return page;
}

QStandardItemModel* MysqlConnector::get_model(){
    return model;
}

QTableView* MysqlConnector::get_table_view(){
    return table_view;
}

MysqlConnector::MysqlConnector(const QString &ip_address, const int &port, const QString &username, const QString &password, const QString &database_name){
    this->ip_address = ip_address;
    this->port = port;
    this->username = username;
    this->password = password;
    this->database_name = database_name;

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(ip_address);
    db.setPort(port);
    db.setUserName(username);
    db.setPassword(password);
    db.setDatabaseName(database_name);

    if(!db.open()){
        cout << "failed!"<<endl;
        return;
    }
    cout << "successfull linked mysql!"<<endl;
}

vector<file> MysqlConnector::mysql_query(){
    QSqlQuery query = QSqlQuery(db);
    vector<file> res;
    query.exec("select * from json_files;");
    while(query.next()){
        QString name = query.value("name").toString();
        QString value = query.value("value").toString();
        QString last_edited_time = query.value("last_edited_time").toString();
        res.push_back(file(name.toStdString(), value.toStdString(), last_edited_time.toStdString()));
    }
    return res;
}

void MysqlConnector::delet(string name, int opt){
    if (opt == 0){
        QSqlQuery query = QSqlQuery(db);
        query.prepare("DELETE FROM json_files WHERE name =:name;");
        query.bindValue(":name", QString(name.data()));
        query.exec();
    }else{
        QSqlQuery query = QSqlQuery(db);
        query.prepare("DELETE FROM json_files WHERE value =:value;");
        query.bindValue(":value", QString(name.data()));
        query.exec();
    }
}

void MysqlConnector::insert(string name, string value){
    QSqlQuery query = QSqlQuery(db);
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string dt = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday);
    query.prepare("INSERT INTO json_files (name, value, last_edited_time) VALUES (:name,:value,:last_edited_time);");
    query.bindValue(":name", QString(name.data()));
    query.bindValue(":value", QString(value.data()));
    query.bindValue(":last_edited_time", QString(dt.data()));
    query.exec();
}

void MysqlConnector::refresh_form(){
    vector<file> files = mysql_query();
    cout << files.size()<<endl;
    for (int i = files.size() - 1;i >= 0;i--){
        QStandardItem *item00 = new QStandardItem(files[i].name.data());
        item00->setEditable(true);
        item00->setTextAlignment(Qt::AlignCenter);
        QStandardItem *item01 = new QStandardItem((to_string(files[i].value.length()) + "字符").data());
        item01->setEditable(false);
        item01->setTextAlignment(Qt::AlignCenter);
        QStandardItem *item02 = new QStandardItem(files[i].last_edited_time.data());
        item02->setEditable(false);
        item02->setTextAlignment(Qt::AlignCenter);
        model->appendRow(QList<QStandardItem*>()<<item00<<item01<<item02);
    }
}

void MainWindow::double_clicked(const QModelIndex &){
    cout<<(mysql_connector->get_model()->index(mysql_connector->get_table_view()->currentIndex().row(), 0).data().toString().toStdString())<< endl;
    vector<file>    res = mysql_connector->mysql_query();
    string value = res[res.size() - 1 - mysql_connector->get_table_view()->currentIndex().row()].value;
    cout<<value<<endl;
    w->new_a_page("*\\" + (mysql_connector->get_model()->index(mysql_connector->get_table_view()->currentIndex().row(), 0).data().toString().toStdString()),
                  value);
    mysql_connector->get_page()->close();
}

void MysqlConnector::init_form(){
    page = new QUIWidget;
    page->setTitle("服务器端文件列表");
    table_view = new QTableView(page);
    model = new QStandardItemModel;

    QStandardItem* header0=new QStandardItem("文件名");
    header0->setSizeHint(QSize(50, 40));
    header0->setTextAlignment(Qt::AlignCenter);
    model->setHorizontalHeaderItem(0, header0);
    QStandardItem *header1 = new QStandardItem("文件大小");
    header1->setSizeHint(QSize(50, 40));
    header1->setTextAlignment(Qt::AlignCenter);
    model->setHorizontalHeaderItem(1, header1);
    QStandardItem *header2 = new QStandardItem("最后修改日期");
    header2->setSizeHint(QSize(50, 40));
    header2->setTextAlignment(Qt::AlignCenter);
    model->setHorizontalHeaderItem(2, header2);
    table_view->horizontalHeader()->setStretchLastSection(true);

    table_view->setSelectionBehavior(QAbstractItemView :: SelectRows);
    table_view->setModel(model);
    table_view->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
    table_view->resize(page->width() - 100, page->height() - 100);
    table_view->move(QPoint((page->width()-table_view->width()) / 2, (page->height() - table_view->height()) / 2 + 30));

    MainWindow::connect(table_view, SIGNAL(doubleClicked(const QModelIndex &)), w, SLOT(double_clicked(const QModelIndex &)));
    table_view->installEventFilter(w);

    table_view->setContextMenuPolicy(Qt::CustomContextMenu);
    w->connect(table_view, SIGNAL(customContextMenuRequested(const QPoint& )), w, SLOT(show_delete_menu(const QPoint&)));

    refresh_form();

    page->show();
}

void MainWindow::show_delete_menu(const QPoint& pos){

    QMenu *menu = new QMenu;


    menu->addAction(QString("删除"), this, SLOT(delete_file()));

    //! 显示该菜单，进入消息循环
    menu->exec(mysql_connector->get_table_view()->mapToGlobal(pos)/*全局位置*/);
}

void MainWindow::delete_file(){
    string name = mysql_connector->get_model()->index(mysql_connector->get_table_view()->currentIndex().row(), 0).data().toString().toStdString();
    vector<file>    res = mysql_connector->mysql_query();
    string value = res[res.size() - 1 - mysql_connector->get_table_view()->currentIndex().row()].value;
    mysql_connector->delet(name, 0);
    mysql_connector->get_model()->removeRow(mysql_connector->get_table_view()->currentIndex().row());
}
