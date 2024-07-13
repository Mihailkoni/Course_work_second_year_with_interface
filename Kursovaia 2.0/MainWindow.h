#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>
#include <QDebug>
#include "ui_MainWindow.h"
#include "Hash_Table_Node.h"
#include "AVL_Node.h"
class DebugWindow;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_debugButton_clicked();
    void on_findButton_clicked();
    void on_size_HT_Button_clicked();
public:
    void add_row(Hash_Table_Node p);
    void Walk_Tree(AVL_Node* root);
    void Show_hash_table(Hash_Table* HT);
    void Walk_Tree_To_File(AVL_Node* root , QTextStream& writestream);
    void Show_hash_table_To_File(Hash_Table* HT, QTextStream& writestream);
signals:
    void ToDebugSignal(AVL_Node* root);
    void ToFindSignal(AVL_Node* root);
private:
    Ui::MainWindow* ui;
    AVL_Node* root;
    DebugWindow* DebugWin;

    
};
#endif // MAINWINDOW_H
