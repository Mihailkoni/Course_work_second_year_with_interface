#pragma once
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include "AVL_Node.h"
#include "Hash_Table.h"
#include "MainWindow.h"
#include "checking _input _data.h"

using namespace std;

int flag_for_message = 0;


bool add_to_tree(Key* key, AVL_Node*& p, bool& h, int max_table_size) {
    AVL_Node* p1;
    AVL_Node* p2;

    if (p == nullptr) {
        p = new AVL_Node(max_table_size);
        p->key = key;
        p->HT->add(key);
        p->left = nullptr;
        p->right = nullptr;
        p->bal = 0;
        h = true;
        return true;
    }
    else if (p->key->car.brand > key->car.brand || (p->key->car.brand == key->car.brand && p->key->car.model > key->car.model)) {
        bool res = add_to_tree(key, p->left, h, max_table_size);
        if (h) {
            if (p->bal == 1) {
                p->bal = 0;
                h = false;
            }
            else if (p->bal == 0) {
                p->bal = -1;
            }
            else {
                p1 = p->left;
                if (p1->bal == -1) {
                    p->left = p1->right;
                    p1->right = p;
                    p->bal = 0;
                    p = p1;
                }
                else {
                    p2 = p1->right;
                    p1->right = p2->left;
                    p2->left = p1;
                    p->left = p2->right;
                    p2->right = p;
                    if (p2->bal == -1) {
                        p->bal = 1;
                    }
                    else {
                        p->bal = 0;
                    }
                    if (p2->bal == 1) {
                        p1->bal = -1;
                    }
                    else {
                        p1->bal = 0;
                    }
                    p = p2;
                }
                p->bal = 0;
                h = false;
            }
        }
        return res;
    }
    else if (p->key->car.brand < key->car.brand || (p->key->car.brand == key->car.brand && p->key->car.model < key->car.model)) {
        bool res = add_to_tree(key, p->right, h, max_table_size);
        if (h) {
            if (p->bal == -1) {
                p->bal = 0;
                h = false;
            }
            else if (p->bal == 0) {
                p->bal = 1;
            }
            else {
                p1 = p->right;
                if (p1->bal == 1) {
                    p->right = p1->left;
                    p1->left = p;
                    p->bal = 0;
                    p = p1;
                }
                else {
                    p2 = p1->left;
                    p1->left = p2->right;
                    p2->right = p1;
                    p->right = p2->left;
                    p2->left = p;
                    if (p2->bal == 1) {
                        p->bal = -1;
                    }
                    else {
                        p->bal = 0;
                    }
                    if (p2->bal == -1) {
                        p1->bal = 1;
                    }
                    else {
                        p1->bal = 0;
                    }
                    p = p2;
                }
                p->bal = 0;
                h = false;
            }
        }
        return res;
    }
    else {
        // проверка на наличие в хеш-таблице и добавление, если нет
        if (p->HT->search(key) == false) {
            Key* key_for_HT = new Key();
            key_for_HT = key;
            p->HT->add(key_for_HT);
            return true;
        }
        else {
            return false;
        }
    }
}



void balanceL(AVL_Node*& p, bool& h) {
    AVL_Node* p1;
    AVL_Node* p2;
    if (p->bal == -1) {
        p->bal = 0;
    }
    else if (p->bal == 0) {
        p->bal = 1;
        h = false;
    }
    else {
        p1 = p->right;
        if (p1->bal >= 0) {
            p->right = p1->left;
            p1->left = p;
            if (p1->bal == 0) {
                p->bal = 1;
                p1->bal = -1;
                h = false;
            }
            else {
                p->bal = 0;
                p1->bal = 0;
            }
            p = p1;
        }
        else {
            p2 = p1->left;
            p1->left = p2->right;
            p2->right = p1;
            p->right = p2->left;
            p2->left = p;
            if (p2->bal == 1) {
                p->bal = -1;
            }
            else {
                p->bal = 0;
            }
            if (p2->bal == -1) {
                p1->bal = 1;
            }
            else {
                p1->bal = 0;
            }
            p = p2;
            p2->bal = 0;
        }
    }
}

void balanceR(AVL_Node*& p, bool& h) {
    AVL_Node* p1;
    AVL_Node* p2;
    if (p->bal == 1) {
        p->bal = 0;
    }
    else if (p->bal == 0) {
        p->bal = -1;
        h = false;
    }
    else {
        p1 = p->left;
        if (p1->bal <= 0) {
            p->left = p1->right;
            p1->right = p;
            if (p1->bal == 0) {
                p->bal = -1;
                p1->bal = 1;
                h = false;
            }
            else {
                p->bal = 0;
                p1->bal = 0;
            }
            p = p1;
        }
        else {
            p2 = p1->right;
            p1->right = p2->left;
            p2->left = p1;
            p->left = p2->right;
            p2->right = p;
            if (p2->bal == -1) {
                p->bal = 1;
            }
            else {
                p->bal = 0;
            }
            if (p2->bal == 1) {
                p1->bal = -1;
            }
            else {
                p1->bal = 0;
            }
            p = p2;
            p2->bal = 0;
        }
    }
}

void del_Right(AVL_Node*& root, AVL_Node*& q, bool& h) {
    if (root->left != nullptr) {
        del_Right(root->left, q, h);
        if (h) {
            balanceL(root, h);
        }
    }
    else {
        q->key = root->key;
        q->HT = root->HT;
        q = root;
        root = root->right;
        h = true;
    }
}



bool Delete_Bool(Key* key, AVL_Node*& root, bool& h) {
    if (root == nullptr) {
        return false;
    }

    // Если ключи совпадают и элемент найден в хеш-таблице, и количество элементов больше 1
    if ((root->key->car.brand == key->car.brand) && (root->key->car.model == key->car.model) && (root->HT->search(key)) && (root->HT->count_elements_HT() > 1)) {
        if (root->HT->delete_node(key)) {
            return true;
        }
        else {
            return false;
        }
    }

    // Если ключи совпадают, но элемент не найден в хеш-таблице
    if ((root->key->car.brand == key->car.brand) && (root->key->car.model == key->car.model) && !(root->HT->search(key))) {
        cout << "The line does not exist" << "\n";
        return false;
    }

    // Если ключ меньше текущего, продолжаем удаление в левом поддереве
    if (root->key->car.brand > key->car.brand || (root->key->car.brand == key->car.brand && root->key->car.model > key->car.model)) {
        bool result = Delete_Bool(key, root->left, h);
        if (h) {
            balanceL(root, h);
        }
        return result;
    }

    // Если ключ больше текущего, продолжаем удаление в правом поддереве
    if (root->key->car.brand < key->car.brand || (root->key->car.brand == key->car.brand && root->key->car.model < key->car.model)) {
        bool result = Delete_Bool(key, root->right, h);
        if (h) {
            balanceR(root, h);
        }
        return result;
    }

    // Если ключи совпадают и количество элементов в хеш-таблице равно 1, удаляем узел AVL-дерева
    if ((root->key->car.brand == key->car.brand) && (root->key->car.model == key->car.model) && (root->HT->search(key)) && (root->HT->count_elements_HT() == 1)) {
        AVL_Node* q = root;
        if (q->right == nullptr) {
            root = q->left;
            h = true;
            delete q;
            return true;
        }
        else if (q->left == nullptr) {
            root = q->right;
            h = true;
            delete q;
            return true;
        }
        else {
            del_Right(q->right, q, h);
            if (h) {
                balanceR(root, h);
            }
            delete q;
            return true;
        }
    }

    return false; // Не должно доходить до сюда, если все условия покрыты
}


AVL_Node* search_AVL_node(AVL_Node* p, Key* key, int& steps) {
    if (p == nullptr) {
        return nullptr;
    }
    else if ((p->key->car.brand == key->car.brand) && (p->key->car.model == key->car.model)) {
        return p;
    }
    else if (p->key->car.brand < key->car.brand || ((p->key->car.brand == key->car.brand) && (p->key->car.model < key->car.model))) {
        steps++;
        return search_AVL_node(p->right, key, steps);
    }
    else {
        steps++;
        return search_AVL_node(p->left, key, steps);
    }
}


int global_search(AVL_Node*& root,Key* key) {
    int steps = 0;
    AVL_Node* found_Node = search_AVL_node(root, key, steps);
    if (found_Node != nullptr) {
        if (found_Node->HT->search_with_steps(key, steps) == true) {
            return steps;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}


void read_from_file(AVL_Node*& root,const string& filename, int max_table_size){
    string line;
    string first_state_letter, second_state_letters, brand, model, date_str;
    int region, state_number;
    long long phone;
    int count = 0;

    ifstream in(filename);
    if (!in.is_open()) {
        cout << "The file could not be opened: " << filename << "\n";
        return;
    }
    
    flag_for_message = 1;
    while (getline(in, line)) {
        count++;
        istringstream iss(line);
        if (!(iss >> first_state_letter >> state_number >> second_state_letters >> region >> phone >> brand >> model >> date_str)) {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Critical);
            Warn.setWindowTitle(" ");
            Warn.setWindowIcon(QIcon("guide_icon.png"));
            Warn.setText("String parsing error. Line:" + QString::fromStdString(to_string(count)));
            Warn.exec();
            continue;
        }
        string state_num_str;
        string region_str;
        if (state_number >= 0 && state_number <= 9) {
            state_num_str = "00" + to_string(state_number);
        }
        else if (state_number >= 10 && state_number <= 99) {
            state_num_str = "0" + to_string(state_number);
        }
        else {
            state_num_str = to_string(state_number);
        }

        if (region >= 1 && region <= 9) {
            region_str = "00" + to_string(region);
        }
        else if (region >= 10 && region <= 99) {
            region_str = "0" + to_string(region);
        }
        else {
            region_str = to_string(region);
        }


        Date date;
        string state = first_state_letter + state_num_str + second_state_letters + region_str;
        int status = CheckCorrectField(state, to_string(phone), brand + " " + model,date_str,date);
        if (status != 1) {
            ErrorHandler_for_file(status, count);
            continue;
        }
        

        
        
        Key* key = new Key();

        key->state_num.letters = first_state_letter + second_state_letters;
        key->state_num.number = state_number;
        key->state_num.region = region;
        key->phone_num = phone;
        key->car.brand = brand;
        key->car.model = model;
        key->date = date;
        bool h = false;
        add_to_tree(key,root,h,max_table_size);
        flag_for_message = 2;
    }
    
    in.close();
}

bool add_for_one_bool(AVL_Node*& root, string letters_state_num, int number_for_state, int region, long long phone_num, string brand, string model, int day, int month, int year,int max_table_size) {
    Key* key = new Key();
    key->state_num.letters = letters_state_num;
    key->state_num.number = number_for_state;
    key->state_num.region = region;
    key->phone_num = phone_num;
    key->date.day = day;
    key->date.month = month;
    key->date.year = year;
    key->car.brand = brand;
    key->car.model = model;
    bool h = false;
    if (add_to_tree(key, root, h, max_table_size) == true) {
        return true;
    }
    else {
        return false;
    }
}

bool delete_for_one_bool(AVL_Node*& root, string letters_state_num, int number_for_state, int region, long long phone_num, string brand, string model, int day, int month, int year) {
    Key* key = new Key();
    key->state_num.letters = letters_state_num;
    key->state_num.number = number_for_state;
    key->state_num.region = region;
    key->phone_num = phone_num;
    key->date.day = day;
    key->date.month = month;
    key->date.year = year;
    key->car.brand = brand;
    key->car.model = model;
    bool h = false;
    if (Delete_Bool(key, root, h) == true) {
        return true;
    }
    else {
        return false;
    }
}