/*
 * @Author: SuBonan
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include"page.h"
#include"mainwindow.h"
#include "ui_mainwindow.h"
#include<QStandardItemModel>
#include<QStandardItem>
#include<QString>
#include<QList>
#include<fstream>
#include<QSize>
#include<QHeaderView>
#include<iostream>

using namespace std;

extern MainWindow *w;

QsciScintilla* Page::get_editor(){
    return editor;
}

QFontMetrics* Page::get_fontmetrics(){
    return fontmetrics;
}

QSplitter* Page::get_splitter(){
    return splitter;
}

QsciLexerJSON* Page::get_textLexer(){
    return textLexer;
}


QTreeView* Page::get_tree_view(){
    return tree_view;
}

string Page::get_file_path(){
    return file_path;
}

string Page::get_name(){
    return name;
}

QStandardItemModel* Page::get_model(){
    return model;
}

Json::Value Page::get_root(){
    return root;
}

extern MainWindow *w;

void Page::set_root(Json::Value nroot){
    root = nroot;
}

void Page::set_name(string nname){
    name = nname;
}

void Page::set_file_path(string nfile_path){
    file_path = nfile_path;
    if (file_path != " ")    {
        for (int i = file_path.length() - 1;i >= 0;i--) if (file_path[i] == '\\')   {
            name = file_path.substr(i + 1);
            break;
        }
    }
}

Page::Page(QsciScintilla *neditor, QsciLexerJSON *ntextLexer, QFontMetrics *nfontmetrics, QSplitter *nsplitter, QTreeView *ntree_view, string nfile_path, string context){
    editor = neditor;
    textLexer = ntextLexer;
    fontmetrics = nfontmetrics;
    splitter = nsplitter;
    tree_view = ntree_view;
    file_path = nfile_path;
    saved = false;

    if (file_path != " ")    {
        for (int i = file_path.length() - 1;i >= 0;i--) if (file_path[i] == '\\')   {
            name = file_path.substr(i + 1);
            break;
        }
    }
    else name = "*.json";
    editor->setLexer(textLexer);
    editor->setMarginType(0,QsciScintilla::NumberMargin);
    editor->setMarginLineNumbers(0,true);
    editor->setMarginWidth(0, fontmetrics->width("00000"));
    editor->setAutoIndent(true);
    editor->setAutoCompletionFillupsEnabled(true);
    editor->setLexer(new QsciLexerJSON);
    editor->setCaretLineBackgroundColor(Qt::lightGray);
    editor->setFolding(QsciScintilla::BoxedTreeFoldStyle);
    editor->setFoldMarginColors(Qt::gray,Qt::lightGray);
    editor->installEventFilter(w);

    model = new QStandardItemModel(tree_view);
    tree_view->setModel(model);
    tree_view->setIndentation(20);

    tree_view->installEventFilter(w);

    QStandardItem* header0=new QStandardItem("数据类型");
    header0->setSizeHint(QSize(5, 40));
    header0->setTextAlignment(Qt::AlignCenter);
    model->setHorizontalHeaderItem(0, header0);
    QStandardItem *header1 = new QStandardItem("键");
    header1->setSizeHint(QSize(50, 40));
    header1->setTextAlignment(Qt::AlignCenter);
    model->setHorizontalHeaderItem(1, header1);
    QStandardItem *header2 = new QStandardItem("值");
    header2->setSizeHint(QSize(50, 40));
    header2->setTextAlignment(Qt::AlignCenter);
    model->setHorizontalHeaderItem(2, header2);


    QStandardItem *item00 = new QStandardItem("对象");
    item00->setEditable(false);
    model->setItem(0, 0, item00);
    QStandardItem *item01 = new QStandardItem("/");
    item01->setEditable(false);
    model->setItem(0, 1, item01);
    QStandardItem *item02 = new QStandardItem("/");
    item02->setEditable(false);
    model->setItem(0, 2, item02);

    tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    w->connect(tree_view, SIGNAL(customContextMenuRequested(const QPoint& )), w, SLOT(show_context_menu(const QPoint&)));
    if (file_path[0] == '*'){
        editor->setText(QString(context.data()));
    }else
    if (file_path != " "){
        ifstream in(file_path, ios::binary);
        cout<<file_path<<endl;
        if (!in.is_open()) {
            cout << "Error opening file\n";
            return;
        }
        Json::Reader reader;
        if(reader.parse(in, root)){
        }else{
            cout<<reader.getFormatedErrorMessages()<<endl;
        }
        string str = root.toStyledString();
        editor->setText(QString::fromStdString(str));
    }

    splitter->addWidget(tree_view);
    splitter->addWidget(editor);
    splitter->setWindowTitle(QObject::tr("split"));
    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 3);
}



