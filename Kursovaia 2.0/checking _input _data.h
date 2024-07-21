#ifndef CHECKING_INPUT_DATA_H
#define CHECKING_INPUT_DATA_H
#include <vector>
#include <fstream>
#include <set>
#include <cctype>
#include <QMessageBox>
#include <regex>
#include <regex>
#include <string>
#include "iomanip"
#include <iostream>
#include "AVL_Node.h"



bool isValidState_Number(const std::string& str) {
    std::regex pattern(R"(^[A-Z]\d{3}[A-Z]{2}\d{2,3}$)");
    return std::regex_match(str, pattern);
}


 bool isValidPhone_Number(const std::string& input) {
     std::regex pattern(R"(^[1-9]\d{10}$)");
     return std::regex_match(input, pattern);
 }

 bool isValidCarBrand_And_Model(const std::string& input) {
     std::regex pattern(R"(^[A-Z][a-zA-Z0-9]*(?:-[A-Z0-9][a-zA-Z0-9]*)* [A-Z0-9][a-zA-Z0-9]*(?:-[A-Z0-9][a-zA-Z0-9]*)*$)");
     return std::regex_match(input, pattern);
 }

 bool is_valid_date(int day, int month, int year) {
    if (day < 1 || day > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year < 1885 || year > 9999) return false;
    return true;
}

bool parse_date(const string& date_str, Date& date) {
    if (date_str.size() != 10 || date_str[2] != '.' || date_str[5] != '.') return false;

    // Additional check to ensure 0, 1, 3, 4, 6, 7, 8, 9 are digits
    for (size_t i = 0; i < date_str.size(); ++i) {
        if (i == 2 || i == 5) continue; // Skip the dot positions
        if (!isdigit(date_str[i])) return false;
    }

    date.day = stoi(date_str.substr(0, 2));
    date.month = stoi(date_str.substr(3, 2));
    date.year = stoi(date_str.substr(6, 4));

    return is_valid_date(date.day, date.month, date.year);
}


inline int CheckCorrectField(
    std::string State_Num,
    std::string Phone_Number,
    std::string CarBrand_And_Model,
    std::string date_str,
    Date& date
) {

    if (!isValidState_Number(State_Num)) {
        return -1;
    }
    else if (!isValidPhone_Number(Phone_Number)) {
        return -2;
    }
    else if (!isValidCarBrand_And_Model(CarBrand_And_Model)) {
        return -3;
    }
    else if (!parse_date(date_str,date)) {
        return -4;
    }
    else {
        return 1;
    }
}

inline void ErrorHandler_for_file(int status,int line) {
    QMessageBox Warn;
    Warn.setIcon(QMessageBox::Critical);
    Warn.setWindowTitle(" ");
    Warn.setWindowIcon(QIcon("guide_icon.png"));
    switch (status) {
    case -1:
        Warn.setText("Incorrect state number format in file (A777AA77 or A777AA777). Line:"+QString::fromStdString(to_string(line)));
        break;
    case -2:
        Warn.setText("The phone number format in file is incorrect (11 digits). Line:" + QString::fromStdString(to_string(line)));
        break;
    case -3:
        Warn.setText("Incorrect brand and model format in file (Brand Model). Line:" + QString::fromStdString(to_string(line)));
        break;
    case -4:
        Warn.setText("Incorrect date format in file. Line:" + QString::fromStdString(to_string(line)));
        break;
    }
    Warn.exec();
}

inline void ErrorHandler(int status) {
    QMessageBox Warn;
    Warn.setIcon(QMessageBox::Critical);
    Warn.setWindowTitle(" ");
    Warn.setWindowIcon(QIcon("guide_icon.png"));
    switch (status) {
    case -1:
        Warn.setText("Incorrect state number format in file (A777AA77 or A777AA777).");
        break;
    case -2:
        Warn.setText("The phone number format in file is incorrect (11 digits).");
        break;
    case -3:
        Warn.setText("Incorrect brand and model format in file (Brand Model).");
        break;
    case -4:
        Warn.setText("Incorrect date format in file.");
        break;
    }
    Warn.exec();
}

inline int CheckCorrectField_for_search(
    std::string CarBrand_And_Model,
    std::string date_str,
    Date& date
) {

    if (!isValidCarBrand_And_Model(CarBrand_And_Model)) {
        return -1;
    }
    else if (!parse_date(date_str, date)) {
        return -2;
    }
    else {
        return 1;
    }
}

inline void ErrorHandler_for_search(int status) {
    QMessageBox Warn;
    Warn.setIcon(QMessageBox::Critical);
    Warn.setWindowTitle(" ");
    Warn.setWindowIcon(QIcon("guide_icon.png"));
    switch (status) {
    case -1:
        Warn.setText("Incorrect brand and model format in file (Brand Model).");
        break;
    case -2:
        Warn.setText("Incorrect date format in file.");
        break;
    }
    Warn.exec();
}
#endif