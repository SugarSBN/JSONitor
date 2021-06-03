#ifndef PAGE_H
#define PAGE_H
#include<Qsci/qsciscintilla.h>
#include<Qsci/qscilexerjson.h>
#include<Qsci/qsciapis.h>
#include<QSplitter>
#include<QTreeView>
#include"json/json.h"
#include<QTextEdit>
#include<string>
#include<QFile>
#include<QStandardItemModel>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>

using namespace std;

class Page{
public:
    QsciScintilla* get_editor();
    QsciLexerJSON* get_textLexer();
    QFontMetrics* get_fontmetrics();
    QSplitter* get_splitter();
    QTreeView *get_tree_view();
    string get_file_path();
    string get_name();
    QStandardItemModel *get_model();
    Json::Value get_root();

    void set_name(string nname);
    void set_root(Json::Value nroot);
    void set_file_path(string nfile_path);
    bool saved = false;

    Page(QsciScintilla* neditor = NULL,
         QsciLexerJSON* ntextLexer = NULL,
         QFontMetrics* nfontmetrics = NULL,
         QSplitter* nsplitter = NULL,
         QTreeView *ntree_view = NULL,
         string nfile_path = "",
         string context = "");
private:
    QsciScintilla* editor;
    QsciLexerJSON *textLexer;
    QFontMetrics* fontmetrics;
    QSplitter *splitter;
    QTreeView *tree_view;
    QStandardItemModel *model;
    string file_path;
    string name;

    Json::Value root;

};

#endif // PAGE_H
