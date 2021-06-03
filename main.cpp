/*
 * @Author: SuBonan
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include "mainwindow.h"
#include "quiwidget.h"
#include "mysql_connector.h"
#include <QApplication>

MainWindow *w;
QUIWidget *qui;
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    qui = new QUIWidget;
    w = new MainWindow;
    qui->installEventFilter(w);
    qui->setStyle();
    qui->setMainWidget(w);
    qui->setAlignment(Qt::AlignCenter);
    //qui.setVisible(QUIWidget::BtnMenu,true);
    qui->show();

    return a.exec();
}
