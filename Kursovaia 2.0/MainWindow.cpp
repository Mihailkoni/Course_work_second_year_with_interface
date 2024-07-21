#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QString>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include <QTableWidget>
#include <QStringDecoder>
#include <QDate>
#include "AVL_Node.h"
#include "Hash_Table.h"
#include "Hash_Table_Node.h"
#include "AVL_Tree.h"
#include "checking _input _data.h"
#include "DebugWindow.h"
#include <iostream>
#include <clocale>
#include <list>
#include <fstream>
#include <set>
#include <sstream>





std::string Collection_date_d(Date& date) { //  функция для добавления даты в таблицу
    std::ostringstream oss; 
    oss << std::setfill('0') << std::setw(2) << date.day << "."
        << std::setfill('0') << std::setw(2) << date.month << "."
        << date.year;
    return oss.str();
}

std::string Collection_state_number(State_Number& state) { // функция добавления госномера в таблицу
    std::ostringstream oss;
    oss << state.letters[0] << " " << std::setfill('0') << std::setw(3) << state.number << " " << state.letters[1] << state.letters[2] << " " << std::setfill('0') << std::setw(2) << state.region;
    return oss.str();
}


inline std::string StringTo(Hash_Table_Node p) {
    std::string State_num = Collection_state_number(p.key->state_num);

    std::string Phone_num = to_string(p.key->phone_num);
    std::string Brand_and_model = p.key->car.brand + " " + p.key->car.model;
    std::string Date = Collection_date_d(p.key->date);
    return State_num + " " + Phone_num + " " + Brand_and_model + " " + Date;
}

State_Number State_number_to_string(std::string s) {
    if (s.empty()) {
        throw std::invalid_argument("Input string is empty");
    }
    State_Number state;
    state.letters = s.substr(0, 1);
    state.number = stoi(s.substr(1, 3));
    state.letters += s.substr(4, 2);
    state.region = stoi(s.substr(6));
    return state;
}

Car Brand_and_model_to_string(std::string s) {
    if (s.empty()) {
        throw std::invalid_argument("Input string is empty");
    }
    Car brand_model;
    
    brand_model.brand = s.substr(0, s.find_first_of(" "));
    brand_model.model = s.substr(s.find_first_of(" ") + 1);
    return brand_model;
}


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,DebugWin(new DebugWindow(this))
    
    
    
    
    
{
    ui->setupUi(this);
    
    connect(ui->debugButton,SIGNAL(clicked()), this, SLOT(on_debugButton_clicked));
    connect(this, &MainWindow::ToDebugSignal, DebugWin, &DebugWindow::debug_Signal);

    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(on_findButton_clicked()));


    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(on_loadButton_clicked));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(on_saveButton_clicked));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(on_addButton_clicked));
    connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(on_deleteButton_clicked));
    
    
    ui->Add_HT_Size->setMinimum(1);
    ui->Add_HT_Size->setMaximum(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_size_HT_Button_clicked() {
    ui->size_HT_Button->setEnabled(false);
    ui->Add_HT_Size->setEnabled(false);
}

int count_Of_file;
void MainWindow::on_loadButton_clicked()
{
    if (ui->Add_HT_Size->isEnabled() == false) {
        ui->loadButton->setEnabled(false);
        count_Of_file = ui->Add_HT_Size->value();
        read_from_file(root, "Input.txt", count_Of_file);//,&set_Of_Cars);
        ui->HT_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->HT_table->clearContents();
        ui->HT_table->setRowCount(0);

        MainWindow::Walk_Tree(root);

        if (flag_for_message == 2) {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Information);
            Warn.setWindowTitle(" ");
            Warn.setWindowIcon(QIcon("guide_icon.png"));
            Warn.setText("The download was successful");
            Warn.exec();
        }
        else if (flag_for_message == 0) {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Critical);
            Warn.setWindowTitle(" ");
            Warn.setWindowIcon(QIcon("guide_icon.png"));
            Warn.setText("The database does not exist");
            Warn.exec();
        }
        else if (flag_for_message == 1) {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Warning);
            Warn.setWindowTitle(" ");
            Warn.setWindowIcon(QIcon("guide_icon.png"));
            Warn.setText("The database is empty");
            Warn.exec();
        }
    }
    else {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Warning);
        Warn.setWindowTitle(" ");
        Warn.setWindowIcon(QIcon("guide_icon.png"));
        Warn.setText("Enter the size of the hash table");
        Warn.exec();
    }
}

void MainWindow::add_row(Hash_Table_Node p) { // функция для добавления строки в таблицу
    ui->HT_table->insertRow(ui->HT_table->rowCount());
    int j = ui->HT_table->rowCount() - 1;
    
    QTableWidgetItem* State_Num = new QTableWidgetItem(QString::fromStdString(Collection_state_number(p.key->state_num)));
    ui->HT_table->setItem(j, 0, State_Num);

    QTableWidgetItem* Phone_num = new QTableWidgetItem(QString::fromStdString(to_string(p.key->phone_num)));
    ui->HT_table->setItem(j, 1, Phone_num);

    QTableWidgetItem* Brand_and_Model = new QTableWidgetItem(QString::fromStdString(p.key->car.brand + " " + p.key->car.model));
    ui->HT_table->setItem(j, 2, Brand_and_Model);

    QTableWidgetItem* Date = new QTableWidgetItem(QString::fromStdString(Collection_date_d(p.key->date)));
    ui->HT_table->setItem(j, 3, Date);
}


void MainWindow::Walk_Tree(AVL_Node* root) {
    if (root != nullptr) {
        MainWindow::Walk_Tree(root->left);
        MainWindow::Walk_Tree(root->right);
        MainWindow::Show_hash_table(root->HT);
    }
}

void MainWindow::Show_hash_table(Hash_Table* HT) {

    for (int i = 0; i < HT->size_HT(); i++) {
        Hash_Table_Node node = HT->table[i];
        if (node.status == ONE) {
            MainWindow::add_row(node);
        }
    }
}



void MainWindow::on_saveButton_clicked()
{
    if (ui->Add_HT_Size->isEnabled() == false) {
        QString filePath = "Output.txt";
        QFile fileout(filePath);

        // Проверка существования директории
        QFileInfo fileInfo(filePath);
        QDir dir = fileInfo.absoluteDir();
        if (!dir.exists()) {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Critical);
            Warn.setWindowTitle(" ");
            Warn.setWindowIcon(QIcon("guide_icon.png"));
            Warn.setText("This directory does not exist: " + dir.absolutePath());
            Warn.exec();
            return;
        }

        if (root == nullptr) {
            QMessageBox Warn;
                Warn.setIcon(QMessageBox::Warning);
                Warn.setWindowTitle(" ");
                Warn.setWindowIcon(QIcon("guide_icon.png"));
                Warn.setText("The data base is empty");
                Warn.exec();
                fileout.close();
        } else if (fileInfo.exists() && fileout.open(QIODevice::WriteOnly | QIODevice::Text)) {
                if (root != nullptr) {
                    QTextStream writestream(&fileout);
                    Walk_Tree_To_File(root, writestream);
                    QMessageBox Warn;
                    Warn.setIcon(QMessageBox::Information);
                    Warn.setWindowTitle(" ");
                    Warn.setWindowIcon(QIcon("guide_icon.png"));
                    Warn.setText("The data is saved");
                    Warn.exec();
                    fileout.close();
              }
              else {
                
               }
         }
         else if (!fileInfo.exists()) {
             QMessageBox Warn;
             Warn.setIcon(QMessageBox::Critical);
             Warn.setWindowTitle(" ");
             Warn.setWindowIcon(QIcon("guide_icon.png"));
             Warn.setText("File path is incorrect or file does not exist: " + filePath);
             Warn.exec();
            //qDebug() << "File path is incorrect or file does not exist: " << filePath;
            }
            else {
                QMessageBox Warn;
                Warn.setIcon(QMessageBox::Critical);
                Warn.setWindowTitle(" ");
                Warn.setWindowIcon(QIcon("guide_icon.png"));
               Warn.setText("Failed to open file for writing: " + filePath);
               Warn.exec();
                //qDebug() << "Failed to open file: " << filePath << " Error: " << fileout.errorString();
            }
    }
    else {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Warning);
        Warn.setWindowTitle(" ");
        Warn.setWindowIcon(QIcon("guide_icon.png"));
        Warn.setText("Enter the size of the hash table");
        Warn.exec();
    }
}

void MainWindow::Show_hash_table_To_File(Hash_Table* HT, QTextStream& writestream) {
    for (int i = 0; i < HT->size_HT(); i++) {
        Hash_Table_Node node = HT->table[i];
        if (node.status == ONE) {
            writestream << QString::fromStdString(StringTo(node)) + '\n';
        }
    }
}

void MainWindow::Walk_Tree_To_File(AVL_Node* root, QTextStream& writestream) {
    if (root != nullptr) {
        MainWindow::Walk_Tree_To_File(root->left, writestream);
        MainWindow::Walk_Tree_To_File(root->right, writestream);
        MainWindow::Show_hash_table_To_File(root->HT, writestream);
    }
}



std::string capitalizePart(const std::string& part) {
    std::string result = part;
    result[0] = std::toupper(result[0]);
    std::transform(result.begin() + 1, result.end(), result.begin() + 1, ::tolower);
    return result;
}

// Функция для преобразования слова с дефисами в правильный формат
std::string formatWord(const std::string& word) {
    std::ostringstream formattedWord;
    std::istringstream wordStream(word);
    std::string part;

    bool firstPart = true;
    while (std::getline(wordStream, part, '-')) {
        if (!firstPart) {
            formattedWord << '-';
        }
        formattedWord << capitalizePart(part);
        firstPart = false;
    }

    return formattedWord.str();
}

// Функция для преобразования строки в правильный формат
std::string formatCarMakeAndModel(const std::string& input) {
    std::istringstream iss(input);
    std::ostringstream oss;
    std::string word;
    bool firstWord = true;

    while (iss >> word) {
        if (!firstWord) {
            oss << " ";
        }
        oss << formatWord(word);
        firstWord = false;
    }

    return oss.str();
}



void MainWindow::on_addButton_clicked()
{
    if (ui->Add_HT_Size->isEnabled() == false) {
        count_Of_file = ui->Add_HT_Size->value();
        string Gos_number = ui->lineEditGosNumber->text().toStdString();
        string Phone_number = ui->lineEditPhoneNumber->text().toStdString();
        string Brand_and_model_in = ui->lineEditCarModel->text().toStdString();
        int day = ui->dateEdit->date().day();
        int month = ui->dateEdit->date().month();
        int year = ui->dateEdit->date().year();
        Date date;
        string date_str;

        if (day >= 1 && day <= 9 && month >= 1 && month <= 9) {
            date_str = "0" + to_string(day) + "." + "0" + to_string(month) + "." + to_string(year);
        }
        else if (day >= 1 && day <= 9 && month > 9) {
            date_str = "0" + to_string(day) + "." + to_string(month) + "." + to_string(year);
        }
        else if (month >= 1 && month <= 9 && day > 9) {
            date_str = to_string(day) + "." + "0" + to_string(month) + "." + to_string(year);
        }
        else {
            date_str = to_string(day) + "." + to_string(month) + "." + to_string(year);
        }

        int status = CheckCorrectField(Gos_number, Phone_number, Brand_and_model_in, date_str, date);
        if (status == 1) {
            State_Number state = State_number_to_string(Gos_number);
            string Brand_and_model = formatCarMakeAndModel(Brand_and_model_in);
            Car brand_model = Brand_and_model_to_string(Brand_and_model);
            long long phone = stoll(Phone_number);
            if (add_for_one_bool(root, state.letters, state.number, state.region, phone, brand_model.brand, brand_model.model, day, month, year, count_Of_file) == true) {
                ui->HT_table->clearContents();
                ui->HT_table->setRowCount(0);
                Walk_Tree(root);
                ui->lineEditGosNumber->clear();
                ui->lineEditPhoneNumber->clear();
                ui->lineEditCarModel->clear();
                

                QMessageBox Warn;
                Warn.setIcon(QMessageBox::Information);
                Warn.setWindowTitle(" ");
                Warn.setWindowIcon(QIcon("guide_icon.png"));
                Warn.setText("Data added");
                Warn.exec();
            }
            else {
                QMessageBox Warn;
                Warn.setIcon(QMessageBox::Critical);
                Warn.setWindowTitle(" ");
                Warn.setWindowIcon(QIcon("guide_icon.png"));
                Warn.setText("This combination is already available in the database");
                Warn.exec();
            }
        }
        else {
            ErrorHandler(status);
        }

    }
    else {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Warning);
        Warn.setWindowTitle(" ");
        Warn.setWindowIcon(QIcon("guide_icon.png"));
        Warn.setText("Enter the size of the hash table");
        Warn.exec();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    if (ui->Add_HT_Size->isEnabled() == false) {
        string Gos_number = ui->lineEditGosNumber->text().toStdString();
        string Phone_number = ui->lineEditPhoneNumber->text().toStdString();
        string Brand_and_model = ui->lineEditCarModel->text().toStdString();
        int day = ui->dateEdit->date().day();
        int month = ui->dateEdit->date().month();
        int year = ui->dateEdit->date().year();
        Date date;
        string date_str;

        if (day >= 1 && day <= 9 && month >= 1 && month <= 9) {
            date_str = "0" + to_string(day) + "." + "0" + to_string(month) + "." + to_string(year);
        }
        else if (day >= 1 && day <= 9 && month > 9) {
            date_str = "0" + to_string(day) + "." + to_string(month) + "." + to_string(year);
        }
        else if (month >= 1 && month <= 9 && day > 9) {
            date_str = to_string(day) + "." + "0" + to_string(month) + "." + to_string(year);
        }
        else {
            date_str = to_string(day) + "." + to_string(month) + "." + to_string(year);
        }
        int status = CheckCorrectField(Gos_number, Phone_number, Brand_and_model, date_str, date);
        if (status == 1) {
            State_Number state = State_number_to_string(Gos_number);
            Car brand_model = Brand_and_model_to_string(Brand_and_model);
            long long phone = stoll(Phone_number);
            if (delete_for_one_bool(root, state.letters, state.number, state.region, phone, brand_model.brand, brand_model.model, day, month, year) == true) {
                ui->HT_table->clearContents();
                ui->HT_table->setRowCount(0);
                Walk_Tree(root);
                ui->lineEditGosNumber->clear();
                ui->lineEditPhoneNumber->clear();
                ui->lineEditCarModel->clear();
                QMessageBox Warn;
                Warn.setIcon(QMessageBox::Information);
                Warn.setWindowTitle(" ");
                Warn.setWindowIcon(QIcon("guide_icon.png"));
                Warn.setText("Data deleted");
                Warn.exec();
            }
            else {
                QMessageBox Warn;
                Warn.setIcon(QMessageBox::Critical);
                Warn.setWindowTitle(" ");
                Warn.setWindowIcon(QIcon("guide_icon.png"));
                Warn.setText("This combination is not in the database");
                Warn.exec();
            }
        }
        else {
            ErrorHandler(status);
        }
    }
    else {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Warning);
        Warn.setWindowTitle(" ");
        Warn.setWindowIcon(QIcon("guide_icon.png"));
        Warn.setText("Enter the size of the hash table");
        Warn.exec();
    }
}

void MainWindow::on_debugButton_clicked()
{
    if (ui->Add_HT_Size->isEnabled() == false) {
        emit ToDebugSignal(root);
        DebugWin->setFixedSize(1280, 720);
        DebugWin->show();
    }
    else {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Warning);
        Warn.setWindowTitle(" ");
        Warn.setWindowIcon(QIcon("guide_icon.png"));
        Warn.setText("Enter the size of the hash table");
        Warn.exec();
    }
}


void MainWindow::on_findButton_clicked()
{
    if (ui->Add_HT_Size->isEnabled() == false) {
        string Brand_and_model = ui->lineEditCarModel->text().toStdString();
        int day = ui->dateEdit->date().day();
        int month = ui->dateEdit->date().month();
        int year = ui->dateEdit->date().year();
        string date_str;
        Date date;
        if (day >= 1 && day <= 9 && month >= 1 && month <= 9) {
            date_str = "0" + to_string(day) + "." + "0" + to_string(month) + "." + to_string(year);
        }
        else if (day >= 1 && day <= 9 && month > 9) {
            date_str = "0" + to_string(day) + "." + to_string(month) + "." + to_string(year);
        }
        else if (month >= 1 && month <= 9 && day > 9) {
            date_str = to_string(day) + "." + "0" + to_string(month) + "." + to_string(year);
        }
        else {
            date_str = to_string(day) + "." + to_string(month) + "." + to_string(year);
        }
        int status = CheckCorrectField_for_search(Brand_and_model, date_str, date);
        if (status == 1) {
            Key* k = new Key();
            Car brand_model = Brand_and_model_to_string(Brand_and_model);
            k->car = brand_model;
            k->date.day = day;
            k->date.month = month;
            k->date.year = year;
            int steps = global_search(root, k);
            if (steps >= 0) {
                ui->lineEditGosNumber->clear();
                ui->lineEditPhoneNumber->clear();
                ui->lineEditCarModel->clear();
                QMessageBox Warn;
                Warn.setIcon(QMessageBox::Information);
                Warn.setWindowTitle(" ");
                Warn.setWindowIcon(QIcon("guide_icon.png"));
                Warn.setText("Data found");
                Warn.setInformativeText("Steps:" + QString::fromStdString(to_string(steps)));
                Warn.exec();
            }
            else {
                QMessageBox Warn;
                Warn.setIcon(QMessageBox::Critical);
                Warn.setWindowTitle(" ");
                Warn.setWindowIcon(QIcon("guide_icon.png"));
                Warn.setText("This data is not in the database");
                Warn.exec();
            }
        }
        else {
            ErrorHandler_for_search(status);
        }
    }
    else {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Warning);
        Warn.setWindowTitle(" ");
        Warn.setWindowIcon(QIcon("guide_icon.png"));
        Warn.setText("Enter the size of the hash table");
        Warn.exec();
    }
    
}


