#include "DebugWindow.h"
#include "ui_DebugWindow.h"
#include <iomanip>
#include <fstream>
#include <sstream>

DebugWindow::DebugWindow(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::DebugWindow)
{
	ui->setupUi(this);
}

DebugWindow::~DebugWindow(){
	delete ui;
}



std::string Collection_state_number_debug(State_Number& state) { // функция добавления госномера в таблицу
    
    std::ostringstream oss;
    oss << state.letters[0] << std::setfill('0') << std::setw(3) << state.number << state.letters[1] << state.letters[2] << std::setfill('0') << std::setw(2) << state.region;
    return oss.str();
}

std::string Collection_date_debug(Date& date) { //  функция для добавления даты в таблицу
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << date.day << "."
        << std::setfill('0') << std::setw(2) << date.month << "."
        << date.year;
    return oss.str();
}


void DebugWindow::debug_Signal(AVL_Node* root) {
	this->d_root = root;
    ui->text_AVLEdit->setReadOnly(true);
    ui->table_HT_Widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->text_AVLEdit->clear();
    print_debug(root, 0, ui->text_AVLEdit,false);
    ui->table_HT_Widget->setRowCount(0);
    print_HT_in_tree(root);

}


void DebugWindow::print_debug(AVL_Node* root, int h, QTextEdit* textEdit, bool isRightChild) {
    if (root != nullptr) {
        // Печать левого поддерева
        print_debug(root->left, h + 10, textEdit, false);

        // Печать текущего узла с отступом
        QString indentation(h, ' ');
        QString nodeText = (QString::fromStdString(root->key->car.brand) + " " + QString::fromStdString(root->key->car.model) + "\n");
        if (isRightChild) {
            textEdit->append(indentation + "└── " + nodeText);  // Стрелка для правого поддерева
        }
        else {
            textEdit->append(indentation + "┎── " + nodeText);  // Если нет правого поддерева
        }

        // Печать правого поддерева
        print_debug(root->right, h + 10, textEdit, true);
    }
}


void DebugWindow::addRow_d(const QString& brand_model,const QString cell_status,const QString& state_num,const QString& phone_num,const QString& date,const QString& first_hash) {
    ui->table_HT_Widget->insertRow(ui->table_HT_Widget->rowCount());
    int j = ui->table_HT_Widget->rowCount() - 1;

    QTableWidgetItem* item_brand_model = new QTableWidgetItem(brand_model);
    ui->table_HT_Widget->setItem(j, 0, item_brand_model);

    if (!cell_status.isEmpty()) {
        QTableWidgetItem* item_cell_status = new QTableWidgetItem(cell_status);
        ui->table_HT_Widget->setItem(j, 1, item_cell_status);
    }

    if (!state_num.isEmpty()) {
        QTableWidgetItem* item_state_num = new QTableWidgetItem(state_num);
        ui->table_HT_Widget->setItem(j, 2, item_state_num);
    }

    if (!phone_num.isEmpty()) {
        QTableWidgetItem* item_phone_num = new QTableWidgetItem(phone_num);
        ui->table_HT_Widget->setItem(j, 3, item_phone_num);
    }

    if (!date.isEmpty()) {
        QTableWidgetItem* item_date = new QTableWidgetItem(date);
        ui->table_HT_Widget->setItem(j, 4, item_date);
    }
    
    if (!first_hash.isEmpty()) {
        QTableWidgetItem* item_first_hash = new QTableWidgetItem(first_hash);
        ui->table_HT_Widget->setItem(j, 5, item_first_hash);
    }

}

void DebugWindow::print_HT(Hash_Table* HT, const std::string& brand_model) {
    for (int i = 0; i < HT->size_HT(); i++) {
        Hash_Table_Node node = HT->table[i];
        
        if (node.status == ONE) {
            DebugWindow::addRow_d(QString::fromStdString(to_string(i)), QString::fromStdString(Collection_state_number_debug(node.key->state_num)),"ONE", QString::fromStdString(to_string(node.key->phone_num)), QString::fromStdString(Collection_date_debug(node.key->date)), QString::fromStdString(to_string(node.key->first_hash_key)));
            
        }
        else if(node.status == ZERO){
            DebugWindow::addRow_d(QString::fromStdString(to_string(i)), "ZERO", "", "", "", "");
        }
        else {
            DebugWindow::addRow_d(QString::fromStdString(to_string(i)), "TWO", "", "", "","");
        }
    }
}


void DebugWindow::print_HT_in_tree(AVL_Node* root) {
    if (root != nullptr) {
        print_HT_in_tree(root->left);
        print_HT_in_tree(root->right);
        std::string brand_model_str = root->key->car.brand + " " + root->key->car.model;
        QString brand_model_Q = QString::fromStdString(root->key->car.brand + " " + root->key->car.model);
        DebugWindow::addRow_d(brand_model_Q, "", "", "", "","");
        print_HT(root->HT, brand_model_str);
    }
}