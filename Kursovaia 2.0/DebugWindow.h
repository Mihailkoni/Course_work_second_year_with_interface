#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H
#include <MainWindow.h>
#include <QMainWindow>
#include <QTextEdit>
#include "AVL_Node.h"
#include "ui_DebugWindow.h"
QT_BEGIN_NAMESPACE
namespace Ui {
	class DebugWindow;
}
QT_END_NAMESPACE
class DebugWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit DebugWindow(QWidget *parent = nullptr);
	~DebugWindow();
	void print_debug(AVL_Node* root, int h, QTextEdit* textEdit, bool isRightChild);
	void print_HT_in_tree(AVL_Node* root);
	void addRow_d(const QString& brand_model,const QString cell_status, const QString& state_num, const QString& phone_num, const QString& date);
	void print_HT(Hash_Table* HT, const std::string& brand_model);
public slots:
	void debug_Signal(AVL_Node* root); // ѕубличный слот дл€ получени€ указател€ на корень дерева
private:
	Ui::DebugWindow* ui;
	AVL_Node* d_root; // ”казатель на корень дерева
};
#endif // DEBUGWINDOW_H