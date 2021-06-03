#ifndef MYSQL_CONNECTOR_H
#define MYSQL_CONNECTOR_H

#endif // MYSQL_CONNECTOR_H
#include<string>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QString>
#include"quiwidget.h"
#include<vector>
#include<QTableView>
#include<QStandardItemModel>
#include<QMenu>

using namespace std;

struct file{
    string name;
    string value;
    string last_edited_time;
    file(){}
    file(string nname, string nvalue, string nlast_edited_time){
        name = nname;value = nvalue;last_edited_time = nlast_edited_time;
    }
};

class MysqlConnector{

public:
    MysqlConnector(const QString &ip_address = "",
                   const int &port = 3306,
                   const QString &username = "",
                   const QString &password = "",
                   const QString &database_name = "");


    vector<file> mysql_query();
    void init_form();
    void refresh_form();
    void insert(string name, string value);
    void delet(string name, int opt);
    QUIWidget *get_page();
    QTableView *get_table_view();
    QStandardItemModel *get_model();

private:
    QString ip_address;
    int port;
    QString username;
    QString password;
    QString database_name;
    QSqlDatabase db;
    QMenu *table_widget_menu;

    QUIWidget *page;
    QTableView *table_view;
    QStandardItemModel *model;
private slots:
};
