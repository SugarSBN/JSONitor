#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QLabel>
#include<QKeyEvent>
#include<string>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *getUI(){
        return ui;
    }
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::MainWindow *ui;
public slots:
    void new_file();
    void open_file();
    void close_a_tab(const int& index);
    void show_context_menu(const QPoint& pos);
    void show_delete_menu(const QPoint&);
    void new_a_subobject();
    void new_a_subarray();
    void new_a_subkeyvalue();
    void new_a_value();
    void set_value_object();
    void set_value_array();
    void set_value_basic();
    void delete_this();
    void save_file();
    void save_as();
    void refresh_left();
    void refresh_right();
    void import_from_server();
    void double_clicked(const QModelIndex &);
    void new_a_page(string file_path, string context = "");
    void save_server();
    void delete_file();
};

#endif // MAINWINDOW_H
