/*
 * @Author: SuBonan
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include"quiwidget.h"
#include"mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QTextStream>
#include<QSplitter>
#include<json/json.h>
#include<json/value.h>
#include<json/reader.h>
#include<json/writer.h>
#include<QPushButton>
#include"page.h"
#include<QDebug>
#include<QModelIndex>
#include<QListView>
#include<QSignalMapper>
#include<QTextEdit>
#include<iostream>
#include<fstream>
#include<QPainter>
#include<string>
#include<QMenu>
#include<QAction>
#include<Qsci/qsciscintilla.h>
#include<Qsci/qscilexerjson.h>
#include<Qsci/qsciapis.h>
#include<string>
#include<vector>
#include<QSqlDatabase>
#include<QSqlQuery>
#include"mysql_connector.h"

using namespace std;

extern MainWindow *w;
extern QUIWidget *qui;
extern MysqlConnector *mysql_connctor;

void new_file();
void new_a_page(string file_path);
void delete_a_page(const int &index);
void refresh_open_pages();
void refresh_according_to_tree_view();
void refresh_according_to_editor();
void save_as();

QTabWidget *qTabWidget;
vector<Page> open_pages;
Page *now_page;


MysqlConnector *mysql_connector = new MysqlConnector("123.56.12.187", 3306, "root", "llysd123", "jsons");

void QUIMessageBox::on_btnOk_clicked(){
    close();
    w->save_as();
}

void MainWindow::new_file(){
    new_a_page(" ");
}

void MainWindow::import_from_server(){
    mysql_connector->init_form();
}

void MainWindow::open_file(){
    QString fileName = QFileDialog::getOpenFileName(w,
                                                        QObject::tr("打开"),
                                                        "",
                                                        QObject::tr("json文件(*.json)"));
   QFile *now_file = new QFile(fileName);
   if (!now_file->open(QIODevice::ReadOnly|QIODevice::Text)){
       cout<<"open failed"<<endl;
       return;
   }
   new_a_page(fileName.toStdString());
   refresh_according_to_editor();
}

void MainWindow::save_server(){
    now_page = &open_pages[qTabWidget->currentIndex()];
    cout << 1 << endl;
    cout << now_page->get_file_path()<<endl;
    if (now_page->get_file_path() != " "){
        now_page->set_file_path("*\\" + now_page->get_name());
        save_file();
    }else{
        now_page->set_file_path("*\\*.json");
        save_file();
    }
}

void MainWindow::save_file(){
    if (now_page == NULL)   return;
    if (now_page->get_file_path()[0] == '*'){
        mysql_connector->delet(now_page->get_name(), 0);
        mysql_connector->insert(now_page->get_name(), now_page->get_editor()->text().toStdString());
    }else
    if (now_page->get_file_path() == " "){
        save_as();
    }else{
        cout << now_page->get_file_path()<<endl;
        ofstream write;
        write.open(now_page->get_file_path());
        write << (now_page->get_editor()->text().toStdString());
        write.close();
        now_page->saved = true;
        qTabWidget->setTabText(qTabWidget->currentIndex(), now_page->get_name().data());
    }
}

void MainWindow::save_as(){
   now_page = &open_pages[qTabWidget->currentIndex()];
   QString fileName = QFileDialog::getSaveFileName(w,
                                                        QObject::tr("保存"),
                                                        "",
                                                        QObject::tr("json文件(*.json)"));
   string file_path = fileName.toStdString();
   cout<<file_path<<endl;
   if (file_path == "") return;
   for (int i = 0;i < file_path.length();i++)  if (file_path[i] == '/')    file_path = file_path.substr(0, i) + "\\\\" + file_path.substr(i + 1);
   now_page->set_file_path(file_path);
   save_file();
}

void MainWindow::new_a_page(string file_path, string context){
    if (file_path != " "){
        for (int i = 0;i < file_path.length();i++)  if (file_path[i] == '/')    file_path = file_path.substr(0, i) + "\\\\" + file_path.substr(i + 1);
        for (int i = 0;i < open_pages.size();i++) if (file_path == open_pages[i].get_file_path()){
            qTabWidget->setCurrentIndex(i);
            return;
        }
    }
    if (context == "")
        open_pages.push_back(Page(new QsciScintilla,
                          new QsciLexerJSON,
                          new QFontMetrics(QFont("Courier", 10, 62)),
                          new QSplitter(Qt::Horizontal, 0),
                          new QTreeView,
                          file_path));
    else
        open_pages.push_back(Page(new QsciScintilla,
                          new QsciLexerJSON,
                          new QFontMetrics(QFont("Courier", 10, 62)),
                          new QSplitter(Qt::Horizontal, 0),
                          new QTreeView,
                          file_path,
                          context));
    refresh_open_pages();
}

void QUIWidget::close_all(){
    for (int i = open_pages.size() - 1;i >= 0;i--)   w->close_a_tab(i);
}

void MainWindow::close_a_tab(const int &index){
    if (open_pages[index].saved == false){
        qTabWidget->setCurrentIndex(index);
        qui->showMessageBoxQuestion(QString("是否保存？"));
    }
    open_pages.erase(open_pages.begin() + index);
    refresh_open_pages();
}

void refresh_open_pages(){
    if (open_pages.size() == 0){
        QLabel *bg = new QLabel;
        QImage *img=new QImage;
        img->load(":/image/json.png");
        bg->setPixmap(QPixmap::fromImage(*img));
        bg->setAlignment(Qt::AlignCenter);
        w->setCentralWidget(bg);
        bg->show();
        return;
    }
    qTabWidget = new QTabWidget;
    for (int i = 0;i < open_pages.size();i++)    qTabWidget->addTab(open_pages[i].get_splitter(), open_pages[i].get_name().data());
    w->setCentralWidget(qTabWidget);
    qTabWidget->setTabsClosable(true);
    qTabWidget->setCurrentIndex(open_pages.size() - 1);
    QObject::connect(qTabWidget, SIGNAL(tabCloseRequested(int)), w, SLOT(close_a_tab(int)));
    qTabWidget->show();
}

QStandardItem *now_item;

void MainWindow::refresh_left(){
    refresh_according_to_editor();
}

void MainWindow::refresh_right(){
    refresh_according_to_tree_view();
}

void MainWindow::new_a_subobject(){
    QStandardItem *item00 = new QStandardItem("对象");
    item00->setEditable(false);
    QStandardItem *item01 = new QStandardItem("/");
    item01->setEditable(false);
    QStandardItem *item02 = new QStandardItem("/");
    item02->setEditable(false);
    now_item->appendRow(QList<QStandardItem*>()<<item00);
    now_item->setChild(item00->index().row(), 1, item01);
    now_item->setChild(item00->index().row(), 2, item02);

}

void MainWindow::new_a_subarray(){
    QStandardItem *item00 = new QStandardItem("数组");
    item00->setEditable(false);
    QStandardItem *item01 = new QStandardItem("/");
    item01->setEditable(false);
    QStandardItem *item02 = new QStandardItem("/");
    item02->setEditable(false);
    now_item->appendRow(QList<QStandardItem*>()<<item00);
    now_item->setChild(item00->index().row(), 1, item01);
    now_item->setChild(item00->index().row(), 2, item02);
}

void MainWindow::new_a_subkeyvalue(){
    QStandardItem *item00 = new QStandardItem("键值对");
    item00->setEditable(false);
    QStandardItem *item01 = new QStandardItem("");
    item01->setEditable(true);
    QStandardItem *item02 = new QStandardItem("null");
    item02->setEditable(true);
    now_item->appendRow(QList<QStandardItem*>()<<item00);
    now_item->setChild(item00->index().row(), 1, item01);
    now_item->setChild(item00->index().row(), 2, item02);
}

void MainWindow::new_a_value(){
    QStandardItem *item00 = new QStandardItem("值");
    item00->setEditable(false);
    QStandardItem *item01 = new QStandardItem("/");
    item01->setEditable(false);
    QStandardItem *item02 = new QStandardItem("null");
    item02->setEditable(true);
    now_item->appendRow(QList<QStandardItem*>()<<item00);
    now_item->setChild(item00->index().row(), 1, item01);
    now_item->setChild(item00->index().row(), 2, item02);
}

void MainWindow::set_value_object(){
    QStandardItem *item00 = new QStandardItem("对象");
    item00->setEditable(false);
    QStandardItem *item01 = new QStandardItem("/");
    item01->setEditable(false);
    QStandardItem *item02 = new QStandardItem("/");
    item02->setEditable(false);
    if (now_item->parent() == NULL){
        while(now_item->hasChildren())  now_item->removeRow(0);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 0, item00);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 1, item00);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 2, item00);
        return;
    }
    now_item->parent()->child(now_item->index().row(), 2)->setText("对象");
    now_item->parent()->child(now_item->index().row(), 2)->setEditable(false);

    now_item->setChild(0, 0, item00);
    now_item->setChild(0, 1, item01);
    now_item->setChild(0, 2, item02);
}

void MainWindow::set_value_array(){
    QStandardItem *item00 = new QStandardItem("数组");
    item00->setEditable(false);
    QStandardItem *item01 = new QStandardItem("/");
    item01->setEditable(false);
    QStandardItem *item02 = new QStandardItem("/");
    item02->setEditable(false);
    if (now_item->parent() == NULL){
        while(now_item->hasChildren())  now_item->removeRow(0);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 0, item00);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 1, item00);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 2, item00);
        return;
    }
    now_item->parent()->child(now_item->index().row(), 2)->setText("数组");
    now_item->parent()->child(now_item->index().row(), 2)->setEditable(false);
    now_item->setChild(0, 0, item00);
    now_item->setChild(0, 1, item01);
    now_item->setChild(0, 2, item02);
}

void MainWindow::set_value_basic(){
    if (now_item->parent() == NULL){
        while(now_item->hasChildren())  now_item->removeRow(0);
        QStandardItem *item00 = new QStandardItem("值");
        item00->setEditable(false);
        QStandardItem *item01 = new QStandardItem("/");
        item01->setEditable(false);
        QStandardItem *item02 = new QStandardItem("null");
        item02->setEditable(true);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 0, item00);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 1, item01);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 2, item02);
        return;
    }
    if (now_item->hasChildren())    now_item->removeRow(0);
    now_item->parent()->child(now_item->index().row(), 2)->setText("null");
    now_item->parent()->child(now_item->index().row(), 2)->setEditable(true);
}

void MainWindow::delete_this(){
    if (now_item->parent() == NULL) return;
    if (now_item->parent()->text() == QString("键值对"))   return;
    now_item->parent()->removeRow(now_item->index().row());
}

void MainWindow::show_context_menu(const QPoint& pos){

    QMenu *menu = new QMenu;

    QModelIndex cur_line = open_pages[qTabWidget->currentIndex()].get_tree_view()->indexAt(pos);
    QModelIndex cur_line_first_element = cur_line.sibling(cur_line.row(), 0);

    now_item = open_pages[qTabWidget->currentIndex()].get_model()->itemFromIndex(cur_line);

    if (cur_line_first_element.isValid()){
        if (now_item->parent() == NULL){
            menu->addAction(QString("设置值为对象类型"), this, SLOT(set_value_object()));
            menu->addSeparator();
            menu->addAction(QString("设置值为数组类型"), this, SLOT(set_value_array()));
        }
        if (now_item->text() == QString("对象")){
            menu->addAction(QString("添加键值对"), this, SLOT(new_a_subkeyvalue()));
        }
        if (now_item->text() == QString("数组")){
            menu->addAction(QString("添加值"), this, SLOT(new_a_value()));
            menu->addSeparator();
            menu->addAction(QString("添加对象"), this, SLOT(new_a_subobject()));
            menu->addSeparator();
            menu->addAction(QString("添加数组"), this, SLOT(new_a_subarray()));
        }
        if (now_item->text() == QString("键值对")){
            menu->addAction(QString("设置值为对象类型"), this, SLOT(set_value_object()));
            menu->addSeparator();
            menu->addAction(QString("设置值为数组类型"), this, SLOT(set_value_array()));
            menu->addSeparator();
            menu->addAction(QString("设置值为基本数据类型"), this, SLOT(set_value_basic()));
        }
        menu->addSeparator();
        menu->addAction(QString("删除"), this, SLOT(delete_this()));
    }


    //! 显示该菜单，进入消息循环
    menu->exec(open_pages[qTabWidget->currentIndex()].get_tree_view()->mapToGlobal(pos)/*全局位置*/);
    refresh_according_to_tree_view();
}

Json::Value tmp;

Json::Value dfs(QStandardItem *u){
    Json::Value res;
    if (u->text() == QString("对象")){
        if (!u->hasChildren()){
           res["a"] = "1";  res.removeMember("a");
        }else{
            for (int i = 0;i < u->rowCount();i++){
                if (u->child(i, 2)->text() == QString("对象") || u->child(i, 2)->text() == QString("数组")){
                    res[u->child(i, 1)->text().toStdString()] = dfs(u->child(i, 0)->child(0, 0));
                }else{
                    if (u->child(i, 2)->text().toStdString() == "null") res[u->child(i, 1)->text().toStdString()] = Json::nullValue;
                    else res[u->child(i, 1)->text().toStdString()] = u->child(i, 2)->text().toStdString();
                }
            }
        }
        return res;
    }

    if (u->text() == QString("数组")){
        if (!u->hasChildren()){
            res.resize(0);
        }else{
            for (int i = 0;i < u->rowCount();i++){
                if (u->child(i, 0)->text() == QString("值")){
                    if (u->child(i, 2)->text().toStdString() == "null") res.append(Json::nullValue);
                    else res.append(u->child(i, 2)->text().toStdString());
                }
                else res.append(dfs(u->child(i, 0)));
            }
        }
        return res;
    }


    return res;
}

void refresh_according_to_tree_view(){
    tmp.clear();
    QStandardItem *root = open_pages[qTabWidget->currentIndex()].get_model()->item(0, 0);
    open_pages[qTabWidget->currentIndex()].set_root(dfs(root));
    cout << dfs(root).toStyledString() << endl;
    open_pages[qTabWidget->currentIndex()].get_editor()->setText(QString::fromStdString(dfs(root).toStyledString()));
}

void dfs(QStandardItem *u, Json::Value r){
    if (r.isObject()){
        Json::Value::Members mem = r.getMemberNames();
        for (int i = 0;i < mem.size();i++){
            string key = mem[i];
            Json::Value value = r[key];
            if (value.isObject()){
                QStandardItem *item00 = new QStandardItem("键值对");
                item00->setEditable(false);
                QStandardItem *item01 = new QStandardItem(QString::fromStdString(key));
                item01->setEditable(true);
                QStandardItem *item02 = new QStandardItem("对象");
                item02->setEditable(false);
                u->appendRow(QList<QStandardItem*>()<<item00);
                u->setChild(item00->index().row(), 1, item01);
                u->setChild(item00->index().row(), 2, item02);

                QStandardItem *item10 = new QStandardItem("对象");
                item10->setEditable(false);
                QStandardItem *item11 = new QStandardItem("/");
                item11->setEditable(false);
                QStandardItem *item12 = new QStandardItem("/");
                item12->setEditable(false);
                item00->appendRow(QList<QStandardItem*>()<<item10);
                item00->setChild(item10->index().row(), 1, item11);
                item00->setChild(item10->index().row(), 2, item12);
                dfs(item10, r[key]);
            }else
            if (value.isArray()){
                QStandardItem *item00 = new QStandardItem("键值对");
                item00->setEditable(false);
                QStandardItem *item01 = new QStandardItem(QString::fromStdString(key));
                item01->setEditable(true);
                QStandardItem *item02 = new QStandardItem("数组");
                item02->setEditable(false);
                u->appendRow(QList<QStandardItem*>()<<item00);
                u->setChild(item00->index().row(), 1, item01);
                u->setChild(item00->index().row(), 2, item02);

                QStandardItem *item10 = new QStandardItem("数组");
                item10->setEditable(false);
                QStandardItem *item11 = new QStandardItem("/");
                item11->setEditable(false);
                QStandardItem *item12 = new QStandardItem("/");
                item12->setEditable(false);
                item00->appendRow(QList<QStandardItem*>()<<item10);
                item00->setChild(item10->index().row(), 1, item11);
                item00->setChild(item10->index().row(), 2, item12);
                dfs(item10, r[key]);
            }else{
                QStandardItem *item00 = new QStandardItem("键值对");
                item00->setEditable(false);
                QStandardItem *item01 = new QStandardItem(QString::fromStdString(key));
                item01->setEditable(true);
                QStandardItem *item02 = new QStandardItem(QString::fromStdString(value.asString()));
                item02->setEditable(false);
                u->appendRow(QList<QStandardItem*>()<<item00);
                u->setChild(item00->index().row(), 1, item01);
                u->setChild(item00->index().row(), 2, item02);
            }
        }
    }else
    if (r.isArray()){
        for (int i = 0;i < r.size();i++){
            if (r[i].isObject()){
                QStandardItem *item00 = new QStandardItem("对象");
                item00->setEditable(false);
                QStandardItem *item01 = new QStandardItem("/");
                item01->setEditable(false);
                QStandardItem *item02 = new QStandardItem("/");
                item02->setEditable(false);
                u->appendRow(QList<QStandardItem*>()<<item00);
                u->setChild(item00->index().row(), 1, item01);
                u->setChild(item00->index().row(), 2, item02);
                dfs(item00, r[i]);
            }else
                if (r[i].isArray()){
                    QStandardItem *item00 = new QStandardItem("数组");
                    item00->setEditable(false);
                    QStandardItem *item01 = new QStandardItem("/");
                    item01->setEditable(false);
                    QStandardItem *item02 = new QStandardItem("/");
                    item02->setEditable(false);
                    u->appendRow(QList<QStandardItem*>()<<item00);
                    u->setChild(item00->index().row(), 1, item01);
                    u->setChild(item00->index().row(), 2, item02);
                    dfs(item00, r[i]);
                }else{
                    QStandardItem *item00 = new QStandardItem("值");
                    item00->setEditable(false);
                    QStandardItem *item01 = new QStandardItem("/");
                    item01->setEditable(false);
                    QStandardItem *item02 = new QStandardItem(QString::fromStdString(r[i].asString()));
                    item02->setEditable(true);
                    u->appendRow(QList<QStandardItem*>()<<item00);
                    u->setChild(item00->index().row(), 1, item01);
                    u->setChild(item00->index().row(), 2, item02);
                }
        }
    }
}

void refresh_according_to_editor(){
    Json::Value root = open_pages[qTabWidget->currentIndex()].get_root();
    cout<<root.toStyledString()<<endl;
    if (root.isArray()){
        QStandardItem *item00 = new QStandardItem("数组");
        item00->setEditable(false);
        QStandardItem *item01 = new QStandardItem("/");
        item01->setEditable(false);
        QStandardItem *item02 = new QStandardItem("/");
        item02->setEditable(false);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 0, item00);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 1, item01);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 2, item02);
        dfs(item00, root);
    }else
    if (root.isObject()){
        QStandardItem *item00 = new QStandardItem("对象");
        item00->setEditable(false);
        QStandardItem *item01 = new QStandardItem("/");
        item01->setEditable(false);
        QStandardItem *item02 = new QStandardItem("/");
        item02->setEditable(false);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 0, item00);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 1, item01);
        open_pages[qTabWidget->currentIndex()].get_model()->setItem(0, 2, item02);
        dfs(item00, root);
    }
    open_pages[qTabWidget->currentIndex()].get_tree_view()->expandAll();
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event){
    //括号补全
    if(open_pages.size() != 0 && watched == open_pages[qTabWidget->currentIndex()].get_editor() && event->type() == QEvent::KeyPress){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->modifiers() == (Qt::ShiftModifier)){
            if(e->key() == Qt::Key_BraceLeft){
                open_pages[qTabWidget->currentIndex()].get_editor()->insert("}");
            }else
            if(e->key() == Qt::Key_ParenLeft){
                open_pages[qTabWidget->currentIndex()].get_editor()->insert(")");
            }
        }else
        if (e->modifiers() == (Qt::NoModifier)){
            if (e->key() == Qt::Key_BracketLeft){
                open_pages[qTabWidget->currentIndex()].get_editor()->insert("]");
            }
        }
        Json::Reader reader;
        Json::Value root;
        reader.parse(open_pages[qTabWidget->currentIndex()].get_editor()->text().toStdString(), root);
        open_pages[qTabWidget->currentIndex()].set_root(root);
        refresh_according_to_editor();
        open_pages[qTabWidget->currentIndex()].saved = false;
    }

    if (open_pages.size() != 0 && watched == open_pages[qTabWidget->currentIndex()].get_tree_view() && event->type() == QEvent::KeyRelease){
        refresh_according_to_tree_view();
        open_pages[qTabWidget->currentIndex()].saved = false;
    }

    if(event->type() == QEvent::KeyPress){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->key() == Qt::Key_F5){
            refresh_according_to_tree_view();
        }
    }

    if(event->type() == QEvent::KeyPress){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->key() == Qt::Key_F6){
            refresh_according_to_editor();
        }
    }

    if(event->type() == QEvent::KeyPress){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->modifiers() == (Qt::ControlModifier)){
            if(e->key() == Qt::Key_N) MainWindow::new_file();
        }
    }

    if(event->type() == QEvent::KeyPress){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)){
            if(e->key() == Qt::Key_S) MainWindow::save_as();
        }
    }

    if(event->type() == QEvent::KeyPress){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->modifiers() == (Qt::ControlModifier)){
            if(e->key() == Qt::Key_O){
                open_file();
            }
        }
    }

    if(event->type() == QEvent::KeyPress){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->modifiers() == (Qt::ControlModifier)){
            if(e->key() == Qt::Key_S){
                now_page = &open_pages[qTabWidget->currentIndex()];
                save_file();
                open_pages[qTabWidget->currentIndex()].saved = true;
            }
        }
    }

    if(event->type() == QEvent::KeyPress){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)){
            if(e->key() == Qt::Key_R){
                char c[7] = {115, 117, 98, 111, 110, 97, 110};
                qui->showMessageBoxQuestion(c);
            }
        }
    }

    if (watched == mysql_connector->get_table_view() && event->type() == QEvent::KeyRelease){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if (e->key() == Qt::Key_Return){
            string name = mysql_connector->get_model()->index(mysql_connector->get_table_view()->currentIndex().row(), 0).data().toString().toStdString();
            vector<file>    res = mysql_connector->mysql_query();
            string value = res[res.size() - 1 - mysql_connector->get_table_view()->currentIndex().row()].value;
            mysql_connector->delet(value, 1);
            mysql_connector->insert(name, value);
            cout << "change name!" << endl;
        }
    }
}
